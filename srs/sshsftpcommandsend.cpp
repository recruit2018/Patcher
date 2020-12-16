#include "sshsftpcommandsend.h"
#include "sshclient.h"

SshSftpCommandSend::SshSftpCommandSend(const QString &source, QString dest, SshSFtp &parent)
    : SshSftpCommand(parent)
    , m_dest(dest)
    , m_localfile(source)
{
    setName(QString("send(%1, %2)").arg(source).arg(dest));
}

void SshSftpCommandSend::process()
{


    switch(m_state)
    {
    case Openning:
        m_sftpfile = libssh2_sftp_open(
                    sftp().getSftpSession(),
               m_dest.toStdString().c_str(),LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,
                    LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
                    LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);

        if(!m_sftpfile)
        {
            char *emsg;
            int size;
            int ret = libssh2_session_last_error(sftp().sshClient()->session(), &emsg, &size, 0);
            if(ret == LIBSSH2_ERROR_EAGAIN)
            {
                return;
            }
            qCDebug(logsshsftp) << "Can't open SFTP file " << m_localfile.fileName() << QString(emsg);

            m_error = true;
            setState(CommandState::Error);
            return;
        }

        if(!m_localfile.open(QIODevice::ReadOnly))
        {

            qCWarning(logsshsftp) << "Can't open local file " << m_localfile.fileName();
            m_error = true;
            setState(CommandState::Closing);
            break;
        }
        setState(CommandState::Exec);
        FALLTHROUGH;
    case Exec:

        int rc;
        if (!m_localfile.isOpen())
        qDebug() << "Файла не существует";

         do {
                 //m_nread = fread(m_buffer, 1, sizeof(m_buffer), m_localfile.handle());
           m_nread = m_localfile.read(m_buffer,sizeof(m_buffer));
                 if(m_nread <= 0) {
                     /* end of file */
                     break;
                 }
                 m_begin = m_buffer;

                 do {
                     /* write data in a loop until we block */
                     rc = libssh2_sftp_write(m_sftpfile, m_begin,  m_nread);

                     if(rc < 0){

                         break;}
                     m_begin += rc;
                     m_nread -= rc;
                 } while(m_nread);

             } while(rc > 0);
            setState(CommandState::Closing);


    case Closing:
    {

        if(m_localfile.isOpen())
        {
            m_localfile.close();
        }

        setState(CommandState::Terminate);
    }
    case Terminate:
        break;

    case Error:
        break;
    }
}
