#pragma once

#include "sshchannel.h"
#include <QLoggingCategory>

static int waitsocket(int socket_fd, LIBSSH2_SESSION *session)
{
    struct timeval timeout;
    int rc;
    fd_set fd;
    fd_set *writefd = NULL;
    fd_set *readfd = NULL;
    int dir;

    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    FD_ZERO(&fd);

    FD_SET(socket_fd, &fd);

    dir = libssh2_session_block_directions(session);

    if(dir & LIBSSH2_SESSION_BLOCK_INBOUND)
        readfd = &fd;

    if(dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
        writefd = &fd;

    rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);
    return rc;
}


Q_DECLARE_LOGGING_CATEGORY(logsshprocess)

class SshProcess : public SshChannel
{
    Q_OBJECT

protected:
    explicit SshProcess(const QString &name, SshClient *client);
    friend class SshClient;

public:
    virtual ~SshProcess() override;
    void close() override;
    QByteArray result();
    bool isError();

public slots:
    void runCommand(const QString &cmd);
    void sshDataReceived() override;




private:
    QString m_cmd;
    LIBSSH2_CHANNEL *m_sshChannel {nullptr};
    QByteArray m_result;
    bool m_error {false};

signals:
    void finished();
    void failed();
};
