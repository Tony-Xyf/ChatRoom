#include "../../include/myhead.h"

int my_accept(int sfd)//
{
    struct sockaddr_in clientaddr;

    bzero(&clientaddr, sizeof(clientaddr));
    int addrlen = sizeof(struct sockaddr);

    int cfd = accept(sfd, (struct sockaddr *)&clientaddr, &addrlen);//返回通信描述符

    if(cfd == -1)
    {
        perror("accept error");
	close(sfd);
	exit(1);
    }

    printf("%s %d success connect...\n",inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));//ip地址转为字符串，网络字节序转为本机字节序

    return cfd;
}
