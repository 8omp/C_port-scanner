#include "scanner.h"

/**
 * TCP Connectスキャンで指定ポートをスキャン
 * @param ip 対象IPアドレス
 * @param port 対象ポート番号
 * @return 1: オープン, 0: クローズ, -1: エラー
 */
int scan_port_connect(const char *ip, int port) {
    int sock;
    struct sockaddr_in target;

    // ソケット作成
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    // ターゲットアドレスの設定
    memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    inet_pton(AF_INET, ip, &target.sin_addr);

    // 接続試行
    int result = connect(sock, (struct sockaddr *)&target, sizeof(target));
    close(sock);

    if (result == 0) {
        return 1; // 接続成功 = ポートオープン
    } else {
        return 0; // 接続失敗 = ポートクローズ
    }
}