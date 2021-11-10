#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
FILE *fopen(const char *file_name, const char *mode_of_operation);
#define CHUNK_CHECK "-c"
#define SIZE_CHECK "-s"
#define SHARED_MEMORY_SIZE 1024