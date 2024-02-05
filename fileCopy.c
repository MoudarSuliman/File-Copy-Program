#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>

#define BUFFER_SIZE 4096

void printHelp(const char* programName);
int copyFile(const char* src, const char* dest, int mmapMode);

int main(int argc, char* argv[]) {
    char* src = NULL;
    char* dest = NULL;
    int mmapMode = 0;

    int opt;
    while ((opt = getopt(argc, argv, "hmi:o:")) != -1) {
        switch (opt) {
            case 'h':
                printHelp(argv[0]);
                return 0;
            case 'm':
                mmapMode = 1;
                break;
            case 'i':
                src = optarg;
                break;
            case 'o':
                dest = optarg;
                break;
            default:
                fprintf(stderr, "Invalid option: -%c\n", optopt);
                printHelp(argv[0]);
                return 1;
        }
    }

    if (src != NULL && dest != NULL) {
        if (copyFile(src, dest, mmapMode) != 0) {
            fprintf(stderr, "File copy failed.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Missing source file or destination file.\n");
        printHelp(argv[0]);
        return 1;
    }

    printf("File copied successfully.\n");
    return 0;
}

void printHelp(const char* programName) {
    printf("Usage: %s [-h] [-m] -i <source_file> -o <dest_file>\n", programName);
    printf("Options:\n");
    printf("  -h                 Print help information\n");
    printf("  -m                 Use mmap mode for file copying\n");
    printf("  -i <source_file>   Specify the input source file\n");
    printf("  -o <dest_file>     Specify the output destination file\n");
}

int copyFile(const char* src, const char* dest, int mmapMode) {
    int source_fd, dest_fd;

    source_fd = open(src, O_RDONLY);
    if (source_fd == -1) {
        perror("Failed to open the source file");
        return 1;
    }

    dest_fd = open(dest, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (dest_fd == -1) {
        perror("Failed to open the destination file");
        close(source_fd);
        return 1;
    }

    off_t source_size;
    struct stat source_stat;
    if (fstat(source_fd, &source_stat) == -1) {
        perror("Failed to get file size");
        close(source_fd);
        close(dest_fd);
        return 1;
    }
    source_size = source_stat.st_size;

    if (mmapMode) {
        if (ftruncate(dest_fd, source_size) == -1) {
            perror("Failed to resize the destination file");
            close(source_fd);
            close(dest_fd);
            return 1;
        }

        void* source_addr = mmap(NULL, source_size, PROT_READ, MAP_PRIVATE, source_fd, 0);
        void* dest_addr = mmap(NULL, source_size, PROT_WRITE, MAP_SHARED, dest_fd, 0);

        memcpy(dest_addr, source_addr, source_size);

        munmap(source_addr, source_size);
        munmap(dest_addr, source_size);
    } else {
        char buffer[BUFFER_SIZE];
        ssize_t bytesRead;

        while ((bytesRead = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
            if (write(dest_fd, buffer, bytesRead) == -1) {
                perror("Failed to write to the destination file");
                close(source_fd);
                close(dest_fd);
                return 1;
            }
        }

        if (bytesRead == -1) {
            perror("Failed to read from the source file");
            close(source_fd);
            close(dest_fd);
            return 1;
        }
    }

    close(source_fd);
    close(dest_fd);
    return 0;
}
