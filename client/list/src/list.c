#include "../../include/myhead.h"

int list(Msg *msg)//从服务器获取在线人员列表，打印出来
{
    printf("\t\t%s[在线]\n",msg->name);

    return SUCCESS;
}
