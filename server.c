#include "lib.h"
/*
file size
Number of characters
Number of Lines
Number of words
*/

int main(int argc, char * argv[]) {
    char fileNameBuffer[NAME_SIZE] = {0};
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in address;
    socklen_t size = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_port = htons(atoi(argv[1]));
    address.sin_addr.s_addr = INADDR_ANY;
    assert(sock_fd >= 0);
    assert(bind(sock_fd, (struct sockaddr *)&address, size) >= 0);
    assert(listen(sock_fd, 3) >= 0);
    printf("Waiting for connection...\n");
    int client_sock_fd = accept(sock_fd, (struct sockaddr *)&address, &size);
    assert(client_sock_fd >= 0);
    printf("Connected. Host: %d\n", atoi(argv[1]));
    while (1) {
        char read_buffer = 0;
        int is_word_started = 0;
        recv(client_sock_fd, &fileNameBuffer, NAME_SIZE, 0);
        fileNameBuffer[strlen(fileNameBuffer) - 1] = '\0';
        if (!strcmp(fileNameBuffer, "exit")) {
            printf("Exiting...\n");
            break;
        }
        info_t info;
        info.file_size = 0;
        info.line_count = 0;
        info.word_count = 0;
        int fd = open(fileNameBuffer, O_RDONLY);
        if (fd == -1) {
            printf("No such file.\n");
            send(client_sock_fd, &info, sizeof(info_t), 0);
            continue;
        }
        info.file_size = lseek(fd, 0, SEEK_END);
        int current_position = 0;
        lseek(fd, 0, SEEK_SET);
        while (current_position != info.file_size) {
            read(fd, &read_buffer, 1);
            if (is_word_started && read_buffer == ' ') {
                is_word_started = 0;
                ++info.word_count;
            } else if (is_word_started && read_buffer == '\n') {
                is_word_started = 0;
                ++info.word_count;
                ++info.line_count;
            } else if (!is_word_started && read_buffer == '\n') {
                ++info.line_count;
            } else if (!is_word_started && read_buffer == ' ') {
                continue;
            } else {
                is_word_started = 1;
            }
            ++current_position;
        }
        close(fd);
        send(client_sock_fd, &info, sizeof(info_t), 0);
        printf("%s: Done.\n", fileNameBuffer);
        memset(fileNameBuffer, 0, NAME_SIZE);
    }
    close(client_sock_fd);
    return 0;
}