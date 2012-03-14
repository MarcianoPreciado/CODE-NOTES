
#include	<sys/socket.h>
int socket(int domain, int type, int Protocol);

/*
 * protocol通常是0，表示按给定的域和套接字类型选择默认协议。
 *
 * SOCK_DGRAM：长度固定的，无连接的不可靠报文传递。与对方通信时不需要逻辑连接
 * ，只需送出一个报文，地址是一个对方进程所使用的套接字。
 * SOCK_RAW：IP协议的数据报接口。用于直接访问下面的网络层，需要ROOT权限。
 * SOCK_SEQPACKET：长度固定、有序、可靠的面向连接的报文传递。顺序数据包。
 * SOCK_STREAM：有序，可靠、双向的面向连接字节流。应用程序意识不到报文界限。
 *
 * 数据报是一种自包含报文。类似于寄信，可以邮寄很多信，但不能保证投递的次序。
 * 面向连接的协议通信就像与对方打电话。建立连接后，每个连接是端对端的通信信道
 * 。会话中不包括地址信息，连接本身暗含特定的源和目的地。
 */

int shutdown(int sockfd, int how);
/*
 * 如果how是SHUT_RD，则无法从套接字读取数据。
 * 如果how是SHUT_WR，则无法使用套接字发送数据。
 * SHUT_RDWR，无法读取和发送。
 *
 * close只有在最后一个活动引用被关闭时才释放网络端点。而shutdown允许使一个套接
 * 一个套接字处于不活动状态，无论引用它的文件描述符数目有多少。
 */


#include	<arpa/inet.h>
uint32_t htonl(uint32_t hostint32);
uint16_t htons(uint16_t hostint16);
uint32_t ntohl(uint32_t netint32);
uint16_t ntohl(uint16_t netint32);

/*
 * struct sockaddr，套接字实现可以自由的添加额外的成员并且定义sa_data成员的大
 * 成员的大小。
 *
 * struct sockaddr_in {
              sa_family_t     sin_family   AF_INET.
              in_port_t       sin_port     Port number.
              struct in_addr  sin_addr     IP address.
	      }
 */

#include	<arpa/inet.h>
/* 网络字节序 */
const char *inet_ntop(int domain, const void *restrict addr,
                      char *restrict str, socklen_t size);
/* 成功则返回地址字符串指针，失败NULL */
/* INET_ADDRSTRLEN定义了足够大的空间来存放表示IPV4地址的文本字符串 */

int inet_pton(int domain, const char *restrict str, void *restrict addr);

/* 获取地址函数成功返回指针，失败NULL
 * 所有的端口都为网络字节序，需调用ntohs来转换
 */

#include	<netdb.h>

/* 主机文件 /etc/hosts，返回主机名和IP地址等 */
struct hostent *gethostent(void);
void sethostent(int stayopen);
void endhostent(void);

struct hostent {
        char *h_name;
        char **h_aliases;
        int h_addrtype;
        int h_length;
        char **h_addr_list;
}

/* 从/etc/networks文件获取信息 */
struct netent *getnetbyaddr(uint32_t net, int type);
struct netent *getnetbyname(const char *name);
struct netent *getnetent(void);

void setnetent(int stayopen);
void endnetent(void);

struct netent {
        char *n_name; /* network name */
        char **n_aliases; /* 指向其他网络名的指针数组 */
        int  n_addrtype; /* AF_INET, AF_INET6 */
        uint32_t n_net;
}

/* 从/etc/protocols 协议数据库文件获取协议信息 */
struct protoent *getprotoent(void);
struct protoent *getprotobyname(const char *name);
struct protoent *getprotobynumber(int proto);
void setprotoent(int stayopen);

void endprotoent(void);
struct protoent {
        char  *p_name;       /* official protocol name */
        char **p_aliases;    /* alias list */
        int    p_proto;      /* protocol number */
}

/* 从/etc/services 服务数据库文件获取服务信息 */
struct servent *getservent(void);
struct servent *getservbyname(const char *name, const char *proto);
struct servent *getservbyport(int port, const char *proto);
void setservent(int stayopen);

void endservent(void);

struct servent {
        char  *s_name;       /* official service name */
        char **s_aliases;    /* alias list */
        int    s_port;       /* port number */
        char  *s_proto;      /* protocol to use */
}

#include	<sys/socket.h>
int getaddrinfo(const char *node, const char *service,
                const struct addrinfo *hints, /* 注意，是指向地址指针的数组 */
                struct addrinfo **res);
void freeaddrinfo(struct addrinfo *res);

const char *gai_strerror(int errcode);
/* 需要提供主机名字，服务名字或者两者都提供。如果只提供一个，另外一个必须是NULL。
 * node可以是一个节点名也可一是点分十进制表示的主机地址。
 * hint十一个用来过滤地址的模板，仅使用ai_family，ai_flags，ai_protocol，
 * ai_socktype字段。剩余的整数字段必须为0，并且指针字段为NULL）。
 *
 * AI_ADDRCONFIG 查询配置的地址类型（IPV4或IPV6）
 * AI_ALL 查找IPV4和IPV6地址（仅用于AI_V4MAPPED）
 * AI_CANONNAME 规范名
 * AI_NUMERICHOST 以数字格式返回主机地址
 * AI_NUMERICSERV 以端口号返回服务
 * AI_PASSIVE 监听绑定端口号
 * AI_V4MAPPED 如果没有找到IPV5地址，则返回映射到IPV6格式的IPV4地址。
 *
       If the AI_PASSIVE flag is specified in hints.ai_flags, and node is
       NULL,  then  the  returned  socket  addresses  will  be  suitable  for
       bind(2)ing a socket that will accept(2) connections.  The returned
       socket address will contain the "wildcard address" (INADDR_ANY for IPv4
       addresses, IN6ADDR_ANY_INIT for IPv6 address).  The wildcard address is
       used by applications (typically servers)  that  intend  to  accept
       connections on any of the hosts's network addresses.  If node is not
       NULL, then the AI_PASSIVE flag is ignored.

       If  the  AI_PASSIVE  flag  is  not  set  in  hints.ai_flags,  then the
       returned socket addresses will be suitable for use with connect(2),
       sendto(2), or sendmsg(2).  If node is NULL, then the network address
       will be set to the loopback interface  address  (INADDR_LOOPBACK  for
       IPv4 addresses, IN6ADDR_LOOPBACK_INIT for IPv6 address); this is used
       by applications that intend to communicate with peers running on the
       same host.

 */
#include <sys/socket.h>
#include <netdb.h>

int getnameinfo(const struct sockaddr *sa, socklen_t salen,
                char *host, size_t hostlen,
                char *serv, size_t servlen, int flags);
/*
NI_NAMEREQD If set, then an error is returned if the hostname cannot be
determined.

NI_DGRAM If set, then the service is datagram (UDP) based rather than stream
(TCP) based.  This is required for the few ports (512-514) that have different
services for UDP and TCP.

NI_NOFQDN If set, return only the hostname part of the fully qualified domain
name for local hosts.

NI_NUMERICHOST If set, then the numeric form of the hostname is returned.
(When not set, this will still happen in case the node's name cannot be
determined.)

NI_NUMERICSERV If set, then the numeric form of the service address is
returned.  (When not set, this will still happen in case the service's name
cannot be determined.)
 */

#include	<sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr, socklen_t len); /* 端口号不小于1024，除非进程有超级用户特权 */
int getsockname(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict alenp);

