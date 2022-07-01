#include <stdio.h>

int main(int argc, char *argv[]) {
    printf("Number of args: %u\n", argc);

    for (int i=0; i < argc; i++) {
        printf("arg #%d - [%s]\n", i, argv[i]);
    }
    return 0;
}
