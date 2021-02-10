#include "deviceicmp.h"


DeviceIcmp::DeviceIcmp(QObject *parent) : QObject(parent)
{
}

#ifdef __linux__
DeviceIcmpUnix::DeviceIcmpUnix(QObject *parent) : DeviceIcmp(parent)
{
}

struct addrinfo* DeviceIcmpUnix::host_serv(const char *host, const char *serv, int family, int socktype)
{
    int				n;
    struct addrinfo	hints, *res;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_CANONNAME;	/* always return canonical name */
    hints.ai_family = family;		/* 0, AF_INET, AF_INET6, etc. */
    hints.ai_socktype = socktype;	/* 0, SOCK_STREAM, SOCK_DGRAM, etc. */

    if ( (n = getaddrinfo(host, serv, &hints, &res)) != 0)
         exit(1);

    return(res);	/* return pointer to first on linked list */
}

unsigned short DeviceIcmpUnix::in_cksum(unsigned short *addr, int len)
{
    int				nleft = len;
    int				sum = 0;
    unsigned short	*w = addr;
    unsigned short	answer = 0;

    while (nleft > 1)  {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1) {
        *(unsigned char *)(&answer) = *(unsigned char *)w ;
        sum += answer;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return(answer);
}

void DeviceIcmpUnix::readloop(Device* dev)
{
    int				size;
    char			recvbuf[BUFSIZE];
    socklen_t		len;
    ssize_t			n;
    struct timeval	tval;
    int res;
    m_sockfd = socket(m_sasend->sa_family, SOCK_RAW, IPPROTO_ICMP);
    setuid(getuid());		/* don't need special permissions any more */

    size = 60 * 1024;		/* OK if setsockopt fails */
    setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    send_v4();

    for ( ; ; ) {
        len = m_salen;
        FD_ZERO(&m_rset);
        FD_SET(m_sockfd,&m_rset);

        FD_ZERO(&m_eset);
        FD_SET(m_sockfd,&m_eset);
        tval.tv_sec = 1;

        res = select(m_sockfd+1,&m_rset,nullptr,&m_eset,&tval);
        if (res == 0)
        {
            qDebug()<<"TIMEOUT!!!";
            send_status(false, dev);
            break;
        }
        if(FD_ISSET(m_sockfd,&m_rset))
        {
            n = recvfrom(m_sockfd, recvbuf, sizeof(recvbuf), 0, m_sarecv, &len);

            if (n < 0) {
                if (errno == EINTR)
                    continue;
                if (errno == EWOULDBLOCK){

                    continue;
                }
                else
                    exit(1);
            }
        }

        if(proc_v4(recvbuf, n))
        {
            send_status(true, dev);
            break;
        }
    }
}

void DeviceIcmpUnix::send_v4()
{
    int			len;
    struct icmp	*icmp;

    icmp = (struct icmp *) m_sendbuf;
    icmp->icmp_type = ICMP_ECHO;
    icmp->icmp_code = 0;
    icmp->icmp_id = m_pid;
    icmp->icmp_seq = m_nsent++;
    gettimeofday((struct timeval *) icmp->icmp_data, NULL);

    len = 8 + m_datalen;		/* checksum ICMP header and data */
    icmp->icmp_cksum = 0;
    icmp->icmp_cksum = in_cksum((u_short *) icmp, len);

    sendto(m_sockfd, m_sendbuf, len, 0, m_sasend, m_salen);
}

void DeviceIcmpUnix::getStatus(const QString & host, Device* dev)
{
    m_pid = getpid();
    m_ai = host_serv(host.toStdString().c_str(), NULL, 0, 0);

    m_sasend = m_ai->ai_addr;
    m_sarecv = static_cast<sockaddr*>(calloc(1, m_ai->ai_addrlen));
    m_salen = m_ai->ai_addrlen;

    readloop(dev);

    freeaddrinfo(m_ai);
    close(m_sockfd);
}

bool DeviceIcmpUnix::proc_v4(char *ptr, ssize_t len)
{
    int				hlen1, icmplen;
    struct ip		*ip;
    struct icmp		*icmp;

    ip = (struct ip *) ptr;		/* start of IP header */
    hlen1 = ip->ip_hl << 2;		/* length of IP header */

    icmp = (struct icmp *) (ptr + hlen1);	/* start of ICMP header */
    if ( (icmplen = len - hlen1) < 8)
         exit(1);

    qDebug()<<icmp->icmp_type<<"type";

    if (icmp->icmp_type == ICMP_HOST_UNREACH)
    {
        qDebug()<<"ICMP_HOST_UNREACH";
    }

    if (icmp->icmp_type == ICMP_ECHOREPLY)
    {
        if (icmp->icmp_id != m_pid) /* not a response to our ECHO_REQUEST */
        {
            return false;
        }
        if (icmplen < 16)
        {
            return false;
        }

        return true;
    }
    return false;
}
#endif

#ifdef _WIN32
DeviceIcmpWin::DeviceIcmpWin(QObject* parent) : DeviceIcmp(parent)
{
}

DeviceIcmpWin::~DeviceIcmpWin()
{
    IcmpCloseHandle(hIcmpFile);
}

void DeviceIcmpWin::getStatus(const QString& str, Device* dev)
{    
    ipaddr = inet_addr(str.toStdString().c_str());
    hIcmpFile = IcmpCreateFile();

    // Выделяем память под буффер ответов
    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*) malloc(ReplySize);

    // Вызываем функцию ICMP эхо запроса
    dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),
                            NULL, ReplyBuffer, ReplySize, 1000);

    if (dwRetVal != 0)
    {
        emit send_status(true, dev);
    }
    else
    {
        emit send_status(false, dev);
    }

    free(ReplyBuffer); // Освобождаем память
}
#endif
