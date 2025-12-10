#include "utils.h"
#include <sys/socket.h>  // struct sockaddr_in
#include <netinet/in.h>  // struct sockaddr_in
#include <arpa/inet.h>   // inet_pton, inet_ntop
#include <netdb.h>       // getaddrinfo, gai_strerror
#include <stdio.h>       // fprintf
#include <string.h>      // strncpy, memset

/**
 * 文字列がIPv4アドレス形式かどうかを判定
 * @param str チェックする文字列
 * @return 1: IPv4形式, 0: それ以外
 */

int is_valid_ipv4(const char *str){
    struct sockaddr_in sa;
    return inet_pton(AF_INET, str, &(sa.sin_addr)) == 1;
}

/**
 * ホスト名をIPv4アドレスに解決
 * @param hostname ホスト名またはIPアドレス
 * @param ip_out 解決したIPアドレスを格納するバッファ
 * @param ip_out_size バッファサイズ（最低16バイト推奨）
 * @return 0: 成功, -1: 失敗
 */

int resolve_hostname(const char *hostname, char *ip_out, size_t ip_out_size){
    if(is_valid_ipv4(hostname)){
        strncpy(ip_out, hostname, ip_out_size - 1);
        
    }
}
