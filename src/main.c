#include "scanner.h"

int main(int argc, char *argv[]) {
    char ip_address[16];

    if (argc > 1) {
        strncpy(ip_address, argv[1], 15);
        ip_address[15] = '\0';
    } else {
        printf("Usage: %s <IP_ADDRESS>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    run_threaded_scan(ip_address);

    return 0;
}