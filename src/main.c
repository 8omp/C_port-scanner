#include "scanner.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    char ip_address[16];
    char input[256];

    if (argc > 1) {
        strncpy(input, argv[1], sizeof(input) - 1);
        input[sizeof(input) - 1] = '\0';
    } else {
        printf("Usage: %s <IP_ADDRESS>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if(resolve_hostname(input, ip_address, sizeof(ip_address)) != 0){
        exit(EXIT_FAILURE);
    }

    printf("Target: %s (%s)\n", input, ip_address);
    run_threaded_scan(ip_address);

    return 0;
}