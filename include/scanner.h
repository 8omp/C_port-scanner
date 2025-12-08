#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <pthread.h>

// 定数
#define NUM_THREADS 100
#define MIN_PORT 1
#define MAX_PORT 65535

// スレッド引数用構造体
typedef struct {
    char ip[16];
    int start_port;
    int end_port;
} ThreadArgs;

// scan_connect.c - TCP Connectスキャン
int scan_port_connect(const char *ip, int port);

// thread_pool.c - スレッド管理
void *scan_ports_thread(void *args);
int run_threaded_scan(const char *ip_address);

#endif // SCANNER_H