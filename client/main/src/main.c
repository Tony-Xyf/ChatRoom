#include "../../include/myhead.h"

int main(int argc, char **argv)//客户端主程序，负责显示界面与调用其他子函数
{
    int flag = 0;
    int sfd;
    
	char menu[MAX_SIZE];
    char str[MAX_SIZE];
	char temp[MAX_SIZE];

    memset(str,0,sizeof(str));

    pthread_t id;
    
    Msg *msg = (Msg *)malloc(sizeof(Msg));
    
    if(argc != 2)//如果用户只允许程序，没有输入ip地址，提醒，关闭程序
    {
        printf("Please input server IP!\n");
		exit(1);
    }

    sfd = my_connect(argv[1], PORT);//调用连接函数，传入ip地址与端口号，返回创建套接字时的返回值
    /*Socket()调用返回一个整型socket描述符，你可以在后面的调用使用它。
	 Socket描述符是一个指向内部数据结构的指针，它指向描述符表入口。*/

    interface_login();//加载欢迎界面
    
    while(1)//循环，直到用户选择功能
    {
        printf("\t\t请输入您要选择的功能(reg/login/exit):");
		scanf("%s",menu);
	    
		if(strcmp(menu,"reg") == 0)
	    {
	        reg(sfd, msg);//调用注册函数,传入socket描述符与Msg结构体
	    }
	    else if(strcmp(menu,"login") == 0)
	    {
			if(login(sfd, msg, str) == SUCCESS)//登录函数
			{
				flag = 1;//登录成功
			}
			else
			{
			    flag = 0;//登录失败
			}
	   	}
	    else if(strcmp(menu,"exit") == 0)
	    {
	        exit(1);
	    }
		else
		{
	        printf("\t\t您输入的格式不正确!\n\n");
		}
		
		if(flag == 1)//如果用户选择操作，退出循环
		{
		    break;
		}
	}

	if(strcmp("admin",str) == 0)//如果登录用户是管理员
	{
	    interface_chat();//打开聊天室主界面
		printf("\t\t|您是超级用户,可以使用以下功能:                       |\n");
		printf("\t\t|                                                     |\n");
		printf("\t\t|                    *踢出某人(out)                   |\n");
		printf("\t\t|                    *禁言某人(ban)                   |\n");
		printf("\t\t|                    *解除某人禁言(rmban)             |\n");
		printf("\t\t|                                                     |\n");
		printf("\t\t+=====================================================+\n");
	}
	else
	{
	    interface_chat();//只打开聊天室主界面
	}
	printf("\t\t当前用户为:%s\n",str);
	pthread_create(&id, NULL, pthread_read, (void *)&sfd);//用户线程

/*	参数

　　	第一个参数为指向线程标识符的指针。

　　	第二个参数用来设置线程属性。

　　	第三个参数是线程运行函数的地址。

　　	最后一个参数是运行函数的参数。

	返回值：

		若成功则返回0，否则返回出错编号
*/

	
    printf("\t\t请输入您要选择的功能命令(如:online):\n");
	while(1)
	{
		scanf("%s",menu);
	    
	    if(strcmp(menu,"online") == 0)
        {
	        bzero(msg, sizeof(Msg));
		    msg->action = LIST;//改变结构体标志位
		    write(sfd, msg, sizeof(Msg));//发送信息
		}
	    else if(strcmp(menu,"chatone") == 0)
			{
		    	bzero(msg, sizeof(Msg));
		    	msg->action = CHAT;
		    	strcpy(msg->name, str);
		    	printf("请输入您要唠嗑的对象:");
		    	scanf("%s",msg->toname);
		    	printf("请输入您要发的消息:");
		    	scanf("%s",msg->msg);
		    
		    	write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"chatall") == 0)
			{
		    	bzero(msg, sizeof(Msg));
		    	msg->action = STOALL;
		    	strcpy(msg->name, str);
				strcpy(msg->toname, "all");
		    	printf("请输入您要群发的消息:");
		    	scanf("%s",msg->msg);

		    	write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"quit") == 0)
			{
			    bzero(msg, sizeof(Msg));
				msg->action = OFFLINE;
				strcpy(msg->name, str);

				write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"changepwd") == 0)
			{
			    bzero(msg, sizeof(Msg));
				msg->action = CHANGEPWD;
				printf("请输入新密码:\n");
				scanf("%s",msg->password);
				strcpy(msg->name, str);

				write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"changename") == 0)
			{
			    bzero(msg, sizeof(Msg));
				msg->action = CHANGENAME;
				printf("请输入新昵称:\n");
				scanf("%s",temp);
				strcpy(msg->name, str);
				strcpy(msg->toname, temp);

				write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"help") == 0)
			{
				if(strcmp("admin",str) == 0)
				{
	                interface_chat();
		            printf("\t\t|您是超级用户,可以使用以下功能:                       |\n");
		            printf("\t\t|                                                     |\n");
		            printf("\t\t|                    *踢出某人(out)                   |\n");
		            printf("\t\t|                    *禁言某人(ban)                   |\n");
		            printf("\t\t|                    *解除某人禁言(rmban)             |\n");
		            printf("\t\t|                                                     |\n");
		            printf("\t\t+=====================================================+\n");
				}
				else
				{
	    			interface_chat();
				}
				printf("\t\t当前用户为:%s\n",str);
			}
	    else if(strcmp(menu,"viewmsg") == 0)
			{
			    printf("***查看聊天记录***\n");
				bzero(msg, sizeof(Msg));
				msg->action = VIEWMSG;
				strcpy(msg->name, str);
            	
				write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"offmsg") == 0)
			{
			    printf("***发送离线消息***\n");
				bzero(msg, sizeof(Msg));
				printf("请输入您要发离线消息的对象:\n");
				scanf("%s",msg->toname);
				printf("请输入离线消息的内容:\n");
				scanf("%s",msg->msg);
				strcpy(msg->name, str);
				msg->action = OFFLINEMSG;

				write(sfd, msg, sizeof(Msg));
			}
	    else if(strcmp(menu,"exit") == 0)
			{
			    printf("\t\t正在退出......\n");
			    bzero(msg, sizeof(Msg));
				msg->action = OFFLINE;
				strcpy(msg->name, str);

				write(sfd, msg, sizeof(Msg));
				exit(1);
			}
	    else if(strcmp(menu,"out") == 0)
			{
			    bzero(msg, sizeof(Msg));
				printf("请输入您要踢出人的姓名:\n");
				scanf("%s",msg->name);
				msg->action = OUT;
                write(sfd, msg, sizeof(Msg));

				printf("%s即将从聊天室踢出!\n",msg->name);
			}
	    else if(strcmp(menu,"ban") == 0)
			{
			    bzero(msg, sizeof(Msg));
				printf("请输入您要禁言人的姓名:\n");
				scanf("%s",msg->name);
				msg->action = BAN;
				write(sfd, msg, sizeof(Msg));

			}
	    else if(strcmp(menu,"rmban") == 0)
			{
			    bzero(msg, sizeof(Msg));
				printf("请输入您要解禁人的姓名:\n");
				scanf("%s",msg->name);
				msg->action = RMBAN;

				write(sfd, msg, sizeof(Msg));
			}
		else
			{
			    printf("\t\tPlease input legal order!\n");
			}
    }
    close(sfd);
    return 0;
}
