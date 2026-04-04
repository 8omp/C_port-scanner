#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/ip.h>   // IPヘッダ用
#include <netinet/tcp.h>  // TCPヘッダ用
#include <arpa/inet.h>
#include <netinet/in.h>

//チェックサム計算
uint16_t checksum(uint16_t *buf, int size) {
    unsigned long sum = 0;
    // 16ビット毎に計算
    while( size > 1 ) {
        sum += *buf;
        buf++;
        size -= 2;
    }
    if( size == 1 ) { sum += *(uint8_t *)buf; }
    // 反転と溢れ処理
    sum = (sum & 0xffff) + (sum >> 16);
    sum = (sum & 0xffff) + (sum >> 16);
    return ~sum;
}

int main(int argc, char *argv[]) {

    char target_ip[16];
    fgets(target_ip, sizeof(target_ip), stdin);

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    if(sock < 0){
        perror("Socket error");
        exit(1);
    }

    char datagram[4096];
    memset(datagram, 0, 4096);



    return 0;
}