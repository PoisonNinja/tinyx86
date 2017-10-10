#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <tinyx86.h>
#include <unistd.h>

tinyx86_file_t tinyx86_file_open(const char* path, char* flags)
{
    tinyx86_file_t file = fopen(path, flags);
    return file;
}

int tinyx86_file_close(tinyx86_file_t file)
{
    int ret = fclose(file);
    if (ret) {
        return -errno;
    }
    return 0;
}

ssize_t tinyx86_file_read(tinyx86_file_t file, uint8_t* buffer, size_t size)
{
    size_t ret = fread(buffer, 1, size, file);
    if (ret < size) {
        if (ferror(file)) {
            return -errno;
        }
    }
    return ret;
}

size_t tinyx86_file_size(tinyx86_file_t file)
{
    size_t start = ftell(file);
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, start, SEEK_SET);
    return size;
}

void tinyx86_exit(int code)
{
    exit(code);
}
