#include "../../include/myhead.h"

    /*  在一个 CLIENT/SERVER模型的网络应用中，客户端的调用序列大致如下：

        socket -> connect -> recv/send -> close
    */

int my_connect(const char *ip, int port)
{
    int sfd;
    
    struct sockaddr_in serveraddr;
    //创建socket
    if((sfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error");
	exit(1);
    }

    /*socket主要是创建一个套接字用于与服务端交换数据，并且通常它会迅速返回，
    此时并没有数据通过网卡发送出去，而紧随其后的connect函数则会产生网络数据的发送，
    TCP的三次握手也正是在此时开始,connect会先发送一个SYN包给服务端，
    并从最初始的CLOSED状态进入到SYN_SENT状态，在此状态等待服务端的确认包，
    通常情况下这个确认包会很快到达，以致于我们根本无法使用netstat命令看到SYN_SENT状态的存在，
    不过我们可以做一个极端情况的模拟，
    让客户端去连接一个随意指定服务器（如IP地址为88.88.88.88），
    因为该服务器很明显不会反馈给我们SYN包的确认包(SYN ACK)，
    客户端就会在一定时间内处于SYN_SENT状态，
    并在预定的超时时间（比如3分钟）之后从connect函数返回，
    connect调用一旦失败(没能到达ESTABLISHED状态)这个套接字便不可用，
    若要再次调用connect函数则必须要重新使用socket函数创建新的套接字。
    */

    bzero(&serveraddr, sizeof(serveraddr));//将字符串清空，初始化
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(port);
    serveraddr.sin_addr.s_addr = inet_addr(ip);

    if(connect(sfd, (struct sockaddr *)&serveraddr, sizeof(struct sockaddr)) == -1)
    {
        perror("connect error");
	close(sfd);
	exit(1);
    }

    return sfd;
}
