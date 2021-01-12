#pragma once

#include <QObject>
#include <QFile>
#include <QFileInfo>
#include <sshsftpcommand.h>

class SshSFtp;

#define SshSftpCommandSend_BUFFER_SIZE 4096

class SshSftpCommandSend: public SshSftpCommand
{
    Q_OBJECT

    QString m_dest;
    bool m_error;

    QFile m_localfile;
    char m_buffer[SFTP_BUFFER_SIZE];
    char *m_begin {nullptr};
    size_t m_nread {0};
    LIBSSH2_SFTP_HANDLE *m_sftpfile {nullptr};

public:
    SshSftpCommandSend(const QString &source, QString dest, SshSFtp &parent);
    void process() override;
};

