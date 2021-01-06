#include "sshprocess.h"
#include "sshclient.h"


Q_LOGGING_CATEGORY(logsshprocess, "ssh.process", QtWarningMsg)

SshProcess::SshProcess(const QString &name, SshClient *client)
    : SshChannel(name, client)
{

}

SshProcess::~SshProcess()
{
    qCDebug(sshchannel) << "free Channel:" << m_name;
}

void SshProcess::close()
{
    setChannelState(ChannelState::Close);
    sshDataReceived();
}

QByteArray SshProcess::result()
{
    return m_result;
}

bool SshProcess::isError()
{
    return m_error;
}

void SshProcess::runCommand(const QString &cmd)
{

    m_cmd = cmd;
    sshDataReceived();

}

void SshProcess::sshDataReceived()
{

    qCDebug(logsshprocess) << "Channel "<< m_name << "State:" << channelState();
    switch(channelState())
    {
    case Openning:
    {

        libssh2_session_set_blocking(m_sshClient->session(), 0);
        while((m_sshChannel = libssh2_channel_open_session(m_sshClient->session())) == nullptr
              && libssh2_session_last_error(m_sshClient->session(), nullptr, nullptr, 0) == LIBSSH2_ERROR_EAGAIN)
        {
            int fd = static_cast<int>(m_sshClient->m_socket.socketDescriptor());

            waitsocket(fd, m_sshClient->m_session);

        }
        if (m_sshChannel == nullptr)
        {

            int ret = libssh2_session_last_error(m_sshClient->session(), nullptr, nullptr, 0);
            if(ret == LIBSSH2_ERROR_EAGAIN)
            {
                return;
            }
            if(!m_error)
            {
                m_error = true;
                emit failed();
            }
            setChannelState(ChannelState::Error);
            qCWarning(logsshprocess) << "Channel session open failed";
            return;
        }
        qCDebug(logsshprocess) << "Channel session opened";

        setChannelState(ChannelState::Exec);
    }

        FALLTHROUGH; case Exec:
    {


        if(m_cmd.size() == 0)
        {
            /* Nothing to process */
            return;
        }
        qCDebug(logsshprocess) << "runCommand(" << m_cmd << ")";

        int ret =0;
        while((ret = libssh2_channel_exec(m_sshChannel, m_cmd.toStdString().c_str())) == LIBSSH2_ERROR_EAGAIN)
        {

            int fd = static_cast<int>(m_sshClient->m_socket.socketDescriptor());

            waitsocket(fd, m_sshClient->m_session);
        }
        if(ret != 0)
        {
            if(!m_error)
            {
                m_error = true;
                emit failed();
                qCWarning(logsshprocess) << "Failed to run command" << ret;
            }
            setChannelState(ChannelState::Close);
            sshDataReceived();
            return;
        }

        setChannelState(ChannelState::Ready);
        /* OK, next step */
    }

        FALLTHROUGH; case Ready:
    {

        for(;;) {
            auto retsz=0;

            do{
                char buffer[16*1024];
                retsz = libssh2_channel_read(m_sshChannel,buffer, sizeof(buffer));
                if(retsz >0){
                    int i;
                    fprintf(stderr, "We read:\n");
                    for(i = 0; i < retsz; ++i)
                        fputc(buffer[i], stderr);
                    fprintf(stderr, "\n");

                    m_result.append(buffer, static_cast<int>(retsz));
                }else {
                    if(retsz != LIBSSH2_ERROR_EAGAIN)
                        fprintf(stderr, "libssh2_channel_read returned %d\n", retsz);
                }

            }while(retsz >0);


            if(retsz == LIBSSH2_ERROR_EAGAIN)
            {
                int fd = static_cast<int>(m_sshClient->m_socket.socketDescriptor());
                waitsocket(fd, m_sshClient->m_session);
            }else{

                setChannelState(Close);
                break;
            }

        }

    }


        FALLTHROUGH; case Close:
    {
        qCDebug(logsshprocess) << "closeChannel:" << m_name;
        int ret = libssh2_channel_close(m_sshChannel);
        if(ret == LIBSSH2_ERROR_EAGAIN)
        {

            return;
        }
        if(ret < 0)
        {

            if(!m_error)
            {
                m_error = true;
                emit failed();
                qCWarning(logsshprocess) << "Failed to channel_close: " << sshErrorToString(ret);
            }
        }
        setChannelState(ChannelState::WaitClose);
    }

        FALLTHROUGH; case WaitClose:
    {
        char *exitsignal = (char *)"none";
        qCDebug(logsshprocess) << "Wait close channel:" << m_name;
        int ret = 0;
        while((ret = libssh2_channel_close(m_sshChannel)) == LIBSSH2_ERROR_EAGAIN )
        {
            int fd = static_cast<int>(m_sshClient->m_socket.socketDescriptor());
            waitsocket(fd, m_sshClient->m_session);

        }

        if(ret == 0)
        {
            libssh2_channel_get_exit_status(m_sshChannel);

            libssh2_channel_get_exit_signal(m_sshChannel, &exitsignal,
                                            NULL, NULL, NULL, NULL, NULL);
        }

        setChannelState(ChannelState::Freeing);
    }

        FALLTHROUGH; case Freeing:
    {

        qDebug() << "free Channel:" << m_name;

        int ret = libssh2_channel_free(m_sshChannel);

        if(ret < 0)
        {
            if(!m_error)
            {
                m_error = true;
                emit failed();
                qCWarning(logsshprocess) << "Failed to free channel: " << sshErrorToString(ret);
            }
        }
        if(m_error)
        {
            setChannelState(ChannelState::Error);
        }


        setChannelState(ChannelState::Free);
        m_sshChannel = nullptr;
        emit finished();
        QObject::disconnect(m_sshClient, &SshClient::sshDataReceived, this, &SshProcess::sshDataReceived);
        return;
    }

    case Free:
    {
        qDebug() << "Channel" << m_name << "is free";
        return;
    }

    case Error:
    {
        emit failed();
        qCDebug(logsshprocess) << "Channel" << m_name << "is in error state";
        setChannelState(Free);
        sshDataReceived();
        return;
    }
    }
}
