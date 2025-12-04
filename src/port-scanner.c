#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // close
#include <sys/time.h>   // struct timeval
#include <sys/socket.h> // socket, connect
#include <netinet/in.h> // struct sockaddr_in, AF_INET, htons
#include <arpa/inet.h>  // inet_pton, inet_addr
#include <netdb.h>      // getaddrinfo
#include <errno.h>      // errno
#include <pthread.h>    // pthreads

const int NUM_THREADS = 100;
const int MIN_PORT = 1;
const int MAX_PORT = 65535;

typedef struct{
    char ip[16];
    int start_port;
    int end_port;
} ThreadArgs;

int scan_port(const char *ip, int port);
void *scan_ports_thread(void *args);

int main(int argc, char *argv[]){

    char ip_address[16];

    if(argc > 1){
        strncpy(ip_address, argv[1], 15);
        ip_address[15] = '\0'; // null終端を保証
    }else{
        printf("Usage: %s <IP_ADDRESS>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Scanning ports on %s with %d threads...\n", ip_address, NUM_THREADS);

    pthread_t threads[NUM_THREADS];
    ThreadArgs thread_args[NUM_THREADS];
    int ports_per_thread = MAX_PORT / NUM_THREADS;

    for(int i = 0; i < NUM_THREADS; i++){
        strcpy(thread_args[i].ip, ip_address);
        thread_args[i].start_port = i * ports_per_thread + 1;
        if(i == NUM_THREADS - 1){
            thread_args[NUM_THREADS - 1].end_port = MAX_PORT;
        }else{
            thread_args[i].end_port = (i + 1) * ports_per_thread;
        }

        // pthread_create(1: スレッドIDのポインタ, 2: スレッド属性(NULLでデフォルト), 3: スレッド関数, 4: スレッド関数への引数)
        if(pthread_create(&threads[i], NULL, scan_ports_thread, &thread_args[i]) != 0){
            perror("pthread_create");
            continue;
        } 
    }

    for(int i = 0; i < NUM_THREADS; i++){
        pthread_join(threads[i], NULL);// pthread_join(1: スレッドID, 2: 戻り値を受け取るポインタ)
    }

    printf("\n====== Scan Complete ======\n");

    return 0;
}

// void * : 戻り値が任意の型のポインタであることを示す
void *scan_ports_thread(void *args){
    ThreadArgs *thread_args = (ThreadArgs *)args;//voidポインタをThreadArgsポインタにキャスト

    // -> : 構造体ポインタが指すメンバにアクセスするための演算子
    // (*thread_args).start_port と同じ意味
    for(int port = thread_args -> start_port; port <= thread_args -> end_port; port++){
        if(scan_port(thread_args -> ip, port) == 1){
            printf("Port %d: OPEN\n", port);
        }
    }
}

int scan_port(const char *ip, int port){
    
    int sock;
    struct sockaddr_in target; // sockaddr_inは、AF_INET、ポート番号、IPアドレスを格納するための構造体

    // making socket
    sock = socket(AF_INET, SOCK_STREAM, 0);// AF_INET :IPv4, SOCK_STREAM: TCP, 0: プロトコル自動選択
    if(sock < 0){
        perror("socket");
        return -1;
    }

    // setting target address
    memset(&target, 0, sizeof(target));// 構造体の初期化
    target.sin_family = AF_INET; 
    target.sin_port = htons(port);//接続先ポート番号をネットワークバイトオーダーに変換
    inet_pton(AF_INET, ip, &target.sin_addr); // IPアドレスの変換

    // connecting to target
    int result = connect(sock, (struct sockaddr *)&target , sizeof(target));
    close(sock);

    //(struct sockaddr *)&target : sockaddr_in構造体をsockaddr構造体にキャスト
    //close(sock)しないと、ファイルディスクリプタがリークして、新しいソケットが作成できなくなる可能性がある

    if(result == 0){
        return 1;//接続成功
    }else{
        return 0;
    }


}