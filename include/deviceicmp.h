#pragma once

#include <QObject>
#include <QString>
#include <QDebug>

#ifdef __linux__
#include	<netinet/in_systm.h>
#include	<netinet/ip.h>
#include	<netinet/ip_icmp.h>

#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<sys/time.h>	/* timeval{} for select() */

#include	<errno.h>
#include	<netdb.h>
#include	<signal.h>
#include	<unistd.h>
#endif

#ifdef _WIN32
#include "iphlpapi.h"
#include "icmpapi.h"
#include <winsock2.h>
#endif

class Device;

class DeviceIcmp : public QObject
{
    Q_OBJECT
public:
    explicit DeviceIcmp(QObject *parent = nullptr);

  public slots:
    virtual void getStatus(const QString&, Device*) = 0;
};


#ifdef __linux__
class DeviceIcmpUnix : public DeviceIcmp
{
    Q_OBJECT
private:
    enum {BUFSIZE=1500};
    char	 m_recvbuf[BUFSIZE];
    char	 m_sendbuf[BUFSIZE];

    int		 m_datalen= 56;			/* #bytes of data, following ICMP header */
    char	*m_host;
    int		 m_nsent;				/* add 1 for each sendto() */
    pid_t	 m_pid;				/* our PID */
    int		 m_sockfd;
    struct addrinfo	*m_ai;

    fd_set m_rset;
    fd_set m_eset;

    struct sockaddr  *m_sasend;	/* sockaddr{} for send, from getaddrinfo */
    struct sockaddr  *m_sarecv;	/* sockaddr{} for receiving */
    size_t	    m_salen;		/* length of sockaddr{}s */

    void readloop(Device*);
    unsigned short in_cksum(unsigned short *addr, int len);
    void send_v4(void);
    bool proc_v4(char *ptr, ssize_t len);
    struct addrinfo* host_serv(const char *host, const char *serv, int family, int socktype);

public:
    explicit DeviceIcmpUnix(QObject *parent = nullptr);

public slots:
    void getStatus(const QString&, Device*) override;

signals:
    void send_status(bool, Device*);
};
#endif


#ifdef _WIN32
class DeviceIcmpWin : public DeviceIcmp
{
    Q_OBJECT
public:
    DeviceIcmpWin(QObject* = nullptr);
    HANDLE hIcmpFile;
    unsigned long ipaddr = INADDR_NONE;
    DWORD dwRetVal = 0;
    char SendData[32];
    LPVOID ReplyBuffer = NULL;
    DWORD ReplySize = 0;
public slots:
    void getStatus(const QString&, Device*) override;

signals:
    void send_status(bool,Device*);
};
#endif

