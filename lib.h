#ifndef LIB
#define LIB

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <signal.h>

#define NAME_SIZE 128

typedef struct info_t {
    int file_size;
    int word_count;
    int line_count;
} info_t;

#endif