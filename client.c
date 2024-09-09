#include "lib.h"

int main(int argc, char * argv[]) {
    int sock_fd = 0;
    char sendBuffer[NAME_SIZE] = {0};
    struct sockaddr_in address;
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock_fd >= 0);
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[1]));
    assert(inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) > 0);
    assert(connect(sock_fd, (struct sockaddr *)&address, sizeof(address)) >= 0);
    while(1) {
        printf("--> ");
        fgets(sendBuffer, NAME_SIZE - 2, stdin);
        send(sock_fd, sendBuffer, strlen(sendBuffer) + 1, 0);
        if (!strcmp(sendBuffer, "exit\n")) {
            printf("Exiting...\n");
            break;
        }
        info_t info;
        recv(sock_fd, &info, sizeof(info), 0);
        if (info.file_size == 0) {
            printf("No such file.\n");
        } else {
            printf("File Size: %d\nWord Count: %d\nLine Count: %d\nSymbol Count: %d\n", info.file_size, info.word_count, info.line_count, info.file_size);
        }
    }
}