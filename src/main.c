#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

#define PS_CONNECT 0   /* ポートに接続できた場合 */
#define PS_NOCONNECT 1 /* ポートに接続できなかった場合 */
#define PS_ERROR -1     /* 接続エラー */

// タイムアウトを設定したソケットを作成する
static int create_timeout_socket(){

    int sock = 0;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("socket");
        return -1;
    }

    //送信タイムアウト設定
    struct timeval send_tv;
    send_tv.tv_sec = 10;
    send_tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &send_tv, sizeof(send_tv));  

    //受信タイムアウト設定
    struct timeval recv_tv;
    recv_tv.tv_sec = 10;
    recv_tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &recv_tv, sizeof(recv_tv));

    return sock;

}

// 指定ポートに接続を試みる
static int connect_to_port(char *ipaddr, int n_port){

    struct sockaddr_in dest;
    int sock = 0;
    int rc = 0;
    int ret_code = 0;

    sock = create_timeout_socket();
    if(sock < 0){
        return PS_ERROR;
    }

    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port   = htons(n_port);
    if (inet_pton(AF_INET, ipaddr, &dest.sin_addr) <= 0) {
        fprintf(stderr, "Invalid IP address: %s\n", ipaddr);
        close(sock);
        return PS_ERROR;
    }

    errno = 0;
    rc = connect(sock, (struct sockaddr*)&dest, sizeof(dest));
    
    if(rc == 0){
        ret_code = PS_CONNECT;
    }else if(errno == ECONNREFUSED){
        ret_code = PS_NOCONNECT;
    }else{
        perror("connect");
        ret_code = PS_ERROR;
    }

    close(sock);
    return ret_code;

}

//ポート番号に対応するサービス名を出力する
static void print_portname(int port_num){

    struct servent *se = NULL;

    se = getservbyport(htons(port_num), "tcp");

    if(se == NULL){
        fprintf(stdout, "port = %5d, unknown\n", port_num);
    }else{
        fprintf(stdout, "port = %5d, service = %s\n", port_num, se->s_name);
    }

}