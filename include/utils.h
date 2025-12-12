#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <stdbool.h>

// DNS関連のユーティリティ関数群
int resolve_hostname(const char *hostname, char *ip_out, size_t ip_out_size);
bool is_valid_ipv4(const char *str);

#endif // UTILS_H
