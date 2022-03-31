#include <stdio.h>


union byte {
    short value;
    char bytes[sizeof(short)];
};

int main() {
    byte test;
    test.value = 0x0102;
    if((test.bytes[0] == 1) && (test.bytes[1] == 2)) {
        printf("大端字节序\n");
    } else if((test.bytes[0] == 2) && (test.bytes[1] == 1)) {
        printf("小端字节序\n");
    } else {
        printf("未知\n");
    }
    return 0;
}