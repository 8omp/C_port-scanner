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

// 接続できるか確認する
static int check_connect(int sock, struct sockaddr* dest, size_t dest_size){

    int rc = 0;

    

}

// 指定ポートに接続を試みる
static int connect_to_port(char *ipaddr, int n_port){

    struct sockaddr_in dest;
    int sock = 0;
    int rc = 0;

    sock = create_timeout_socket();
    if(sock < 0){
        return PS_ERROR;
    }

    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(ipaddr);

    errno = 0;
    rc = connect(sock, (struct sockaddr*)&dest, sizeof(dest));
    if(rc == 0){
        return PS_CONNECT;
    }

    if(errno == ECONNREFUSED){
        return PS_NOCONNECT;
    }

    if(errno != 0){
        perror("connect");
        return PS_ERROR;
    }

    return PS_NOCONNECT;

    close(sock);
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