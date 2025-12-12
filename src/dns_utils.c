#define _POSIX_C_SOURCE 200112L // getaddrinfoのために必要

#include "utils.h"
#include <stdbool.h>
#include <sys/socket.h>  // struct sockaddr_in
#include <netinet/in.h>  // struct sockaddr_in
#include <arpa/inet.h>   // inet_pton, inet_ntop
#include <netdb.h>       // getaddrinfo, gai_strerror
#include <stdio.h>       // fprintf
#include <string.h>      // strncpy, memset

/**
 * 文字列がIPv4アドレス形式かどうかを判定
 * @param str チェックする文字列
 * @return true: IPv4形式, false: それ以外
 * inet_pton: IPv4/IPv6アドレスの文字列表現をバイナリ形式に変換
 */

bool is_valid_ipv4(const char *str){
    struct sockaddr_in sa;
    return inet_pton(AF_INET, str, &(sa.sin_addr)) == 1;
}

/**
 * ホスト名をIPv4アドレスに解決
 * @param hostname ホスト名またはIPアドレス
 * @param ip_out 解決したIPアドレスを格納するバッファ
 * @param ip_out_size バッファサイズ（最低16バイト推奨）
 * @return 0: 成功, -1: 失敗
 * getaddrinfo: ホスト名をIPアドレスに解決
 * inet_ntop: バイナリ形式のIPアドレスを文字列表現に変換
 * freeaddrinfo: getaddrinfoで確保したメモリを解放
 */

int resolve_hostname(const char *hostname, char *ip_out, size_t ip_out_size){

    if(is_valid_ipv4(hostname)){
        strncpy(ip_out, hostname, ip_out_size - 1);
        ip_out[ip_out_size - 1] = '\0';
        return 0;
    }

    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(hostname, NULL, &hints, &res);
    if(status != 0){
        fprintf(stderr, "ERROR: DNS resolution failed: %s\n", gai_strerror(status));
        return -1;
    }

    //IPv4にキャストを行い、文字列形式に変換
    struct sockaddr_in *ipv4 = (struct sockaddr_in *)res -> ai_addr;
    inet_ntop(AF_INET, &(ipv4 -> sin_addr), ip_out, ip_out_size);

    freeaddrinfo(res);
    return 0;

}
