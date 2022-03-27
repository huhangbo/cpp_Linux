#include <cstdio>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("%s filename\n", argv[0]);
        return -1;
    }
    struct stat st{};
    int ret = stat(argv[1], &st);
    if (ret == -1) {
        perror("stat");
        return -1;
    }
    //获取文件类型和权限
    char perm[13] = {0};
    switch (st.st_mode & S_IFMT) {
        case S_IFLNK:
            perm[0] = 1;
    }

    return 0;
}