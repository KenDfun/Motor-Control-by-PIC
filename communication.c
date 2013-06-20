
#include <string.h>

#define MAX_CH_BUF 15
#define MAX_CMD_NUM 15

void sendUartStr(char *str);

char ChBuf[MAX_CH_BUF];
int BufCnt=0;

char CmdBuf[MAX_CMD_NUM][MAX_CH_BUF]={
    "ON:LED1",
    "ON:LED2",
    "ON:LED3",
    "OFF:LED1",
    "OFF:LED2",
    "OFF:LED3",
    "UP:PWM1",
    "DOWN:PWM1",
    ""
};


void clearBuf(void)
{
    BufCnt=0;
    ChBuf[0]='\0';
}

void putBuf(char ch)
{
    ChBuf[BufCnt++]=ch;
    if(BufCnt>=(MAX_CH_BUF-2)){
        clearBuf();
        return;
    }
    ChBuf[BufCnt]='\0';

    return;
}



int chkBuf(void)
{
    int i;

//    sendUartStr(ChBuf);
    for(i=0;i<MAX_CMD_NUM;i++){
        if(strcmp(CmdBuf[i],ChBuf)==0){
            break;
        }
    }

    if(i>=MAX_CMD_NUM){
        return (-1);
    }

    return(i);
}

int addChkBuf(char ch)
{
    int result;
    
    putBuf(ch);
    result=chkBuf();

    if(result!=-1){
        clearBuf();
    }

    return (result);
}