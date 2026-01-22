#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#define PS_CONNECT 0   /* ポートに接続できた場合 */
#define PS_NOCONNECT 1 /* ポートに接続できなかった場合 */
#define PS_ERROR -1     /* 接続エラー */

/*!
 * @brief   タイムアウトを設定したソケットを作成する
 * @return  ソケットディスクリプタ
 * @note    受信設定をしないと正常にタイムアウトしない。（仕様？）
 */
static int
create_timeout_socket()
{
    int sock = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("socket");
        return(-1);
    }

    /* 送信タイムアウトを設定する */
    struct timeval send_tv;
    send_tv.tv_sec  = 10;
    send_tv.tv_usec = 0; //μs単位
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &send_tv, sizeof(send_tv));

    /* 受信タイムアウトを設定する */
    struct timeval recv_tv;
    recv_tv.tv_sec = 10;
    recv_tv.tv_sec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &recv_tv, sizeof(recv_tv));

    return(sock);
}

int main(void){
    return 0;
}