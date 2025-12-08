#include "scanner.h"

/**
 * スレッドワーカー関数
 * 割り当てられたポート範囲をスキャン
 */
void *scan_ports_thread(void *args) {
    ThreadArgs *thread_args = (ThreadArgs *)args;

    for (int port = thread_args->start_port; port <= thread_args->end_port; port++) {
        if (scan_port_connect(thread_args->ip, port) == 1) {
            printf("Port %d: OPEN\n", port);
        }
    }
    return NULL;
}

/**
 * マルチスレッドでポートスキャンを実行
 * @param ip_address 対象IPアドレス
 * @return 0: 成功
 */
int run_threaded_scan(const char *ip_address) {
    pthread_t threads[NUM_THREADS];
    ThreadArgs thread_args[NUM_THREADS];
    int ports_per_thread = MAX_PORT / NUM_THREADS;

    printf("Scanning ports on %s with %d threads...\n", ip_address, NUM_THREADS);

    // スレッド作成
    for (int i = 0; i < NUM_THREADS; i++) {
        strncpy(thread_args[i].ip, ip_address, 15);
        thread_args[i].ip[15] = '\0';
        thread_args[i].start_port = i * ports_per_thread + 1;

        if (i == NUM_THREADS - 1) {
            thread_args[i].end_port = MAX_PORT;
        } else {
            thread_args[i].end_port = (i + 1) * ports_per_thread;
        }

        if (pthread_create(&threads[i], NULL, scan_ports_thread, &thread_args[i]) != 0) {
            perror("pthread_create");
            continue;
        }
    }

    // 全スレッドの完了を待機
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\n====== Scan Complete ======\n");
    return 0;
}