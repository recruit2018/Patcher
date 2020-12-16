#include <QFile>
#include <QFileInfo>
#include <QCryptographicHash>

#include "sshsftpcommandsend.h"
#include "sshsftp.h"
#include "sshclient.h"

Q_LOGGING_CATEGORY(logsshsftp, "ssh.sftp", QtWarningMsg)

#define DEBUGCH qCDebug(logsshsftp) << m_name

SshSFtp::SshSFtp(const QString &name, SshClient *client):
    SshChannel(name, client)

{
    QObject::connect(client, &SshClient::sshDataReceived, this, &SshSFtp::sshDataReceived, Qt::QueuedConnection);
    QObject::connect(this, &SshSFtp::sendEvent, this, &SshSFtp::_eventLoop, Qt::QueuedConnection);
}

SshSFtp::~SshSFtp()
{

}

void SshSFtp::close()
{
    if(channelState() != ChannelState::Error)
    {
        setChannelState(ChannelState::Close);
    }
    emit sendEvent();
}




void SshSFtp::sshDataReceived()
{
    // Nothing to do
    _eventLoop();
}

void SshSFtp::_eventLoop()
{

    switch(channelState())
    {

        case Openning:
        {


           libssh2_session_set_blocking(m_sshClient->session(), 1);
            m_sftpSession = libssh2_sftp_init(m_sshClient->session());

            if(m_sftpSession == nullptr)
            {

                char *emsg;
                int size;
                int ret = libssh2_session_last_error(m_sshClient->session(), &emsg, &size, 0);
                if(ret == LIBSSH2_ERROR_EAGAIN)
                {
                    return;
                }
                if(!m_error)
                {
                    qCWarning(logsshsftp) << "Create sftp session failed " << QString(emsg);
                    m_error = true;
                }
                setChannelState(ChannelState::Error);
                qCWarning(logsshsftp) << "Channel session open failed";
                return;
            }

            DEBUGCH << "Channel session opened";
            setChannelState(ChannelState::Exec);
        }

        FALLTHROUGH; case Exec:
        {

            setChannelState(ChannelState::Ready);
        }


        FALLTHROUGH; case Ready:
        {
        if(m_currentCmd == nullptr && m_cmd.size() > 0)
            {
            m_currentCmd = m_cmd.first();

                m_cmd.pop_front();
            }
            if(m_currentCmd)
            {
                m_currentCmd->process();

                if(m_currentCmd->state() == SshSftpCommand::CommandState::Terminate || m_currentCmd->state() == SshSftpCommand::CommandState::Error)
                {
                    m_currentCmd = nullptr;
                    emit sendEvent();
                     qDebug()<<"!!!!CLOSING!!!OK NEXTSTAP!!!";
                }
            }

            qDebug()<<"!!!!CLOSING!!!OK NEXTSTAP!!!BREAK!!!!";
            setChannelState(ChannelState::Close);

        }

        case Close:
        {
            qDebug() << "closeChannel";
            DEBUGCH << "closeChannel";
            libssh2_sftp_shutdown(m_sftpSession) ;
            setChannelState(ChannelState::WaitClose);
       }

        FALLTHROUGH; case WaitClose:
        {
            setChannelState(ChannelState::Freeing);
        }

        FALLTHROUGH; case Freeing:
        {
        qDebug() << "free Channel";
        emit finish();
        DEBUGCH << "free Channel";
            setChannelState(ChannelState::Free);
            setChannelState(Free);
            QObject::disconnect(m_sshClient, &SshClient::sshDataReceived, this, &SshSFtp::sshDataReceived);
            return;
        }

        case Free:
        {
            DEBUGCH << "Channel is free";
             emit cmdEvent();
            return;
        }

        case Error:
        {
            DEBUGCH << "Channel is in error state";
            setChannelState(Free);
            return;
        }
    }
}

LIBSSH2_SFTP *SshSFtp::getSftpSession() const
{
    return m_sftpSession;
}

bool SshSFtp::processCmd(SshSftpCommand *cmd)
{

    QEventLoop wait(this);
    QObject::connect(this, &SshSFtp::stateChanged, &wait, &QEventLoop::quit);
    QObject::connect(this, &SshSFtp::cmdEvent, &wait, &QEventLoop::quit);

    m_cmd.push_back(cmd);
    emit sendEvent();
    while(channelState() == ChannelState::Ready && cmd->state() != SshSftpCommand::CommandState::Terminate && cmd->state() != SshSftpCommand::CommandState::Error)
    {

        wait.exec();
    }

    return (cmd->state() == SshSftpCommand::CommandState::Terminate);
}


