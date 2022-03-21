//
// Created by HHB on 2022/2/26.
//

#include <cstdio>

void byteorder() {
    union {
        short value;
        char union_bytes[sizeof(value)];
    } test{};
    test.value = 0x0102;
    if (test.union_bytes[0] == 1 && test.union_bytes[1] == 2) {
        printf("big endian\n");
    } else if (test.union_bytes[0] == 2 && test.union_bytes[1] == 1) {
        printf("little endian\n");
    } else {
        printf("unknown...\n");
    }
}
