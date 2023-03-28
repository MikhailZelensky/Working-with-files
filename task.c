#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define DEFAULT_BLOCK_SIZE 4096

int main(int argc, char* argv[]) {
    int input, output;
    int block_size = DEFAULT_BLOCK_SIZE;
    off_t offset = 0;
    ssize_t bytes_read;

    if (argc == 2) {
        output = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (output == -1) {
            fprintf(stderr, "Error opening output file %s: %s\n", argv[1], strerror(errno));
            exit(1);
        }
        input = STDIN_FILENO;
    } else if (argc == 3 || argc == 4) {
        input = open(argv[1], O_RDONLY);
        if (input == -1) {
            fprintf(stderr, "Error opening input file %s: %s\n", argv[1], strerror(errno));
            exit(1);
        }
        output = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (output == -1) {
            fprintf(stderr, "Error opening output file %s: %s\n", argv[2], strerror(errno));
            exit(1);
            }
        if (argc == 4) {
        	block_size = atoi(argv[3]);
        }
    } else {
        fprintf(stderr, "Usage: %s input_file output_file\n", argv[0]);
        exit(1);
    }
    
    char buffer[block_size];
    void* zeros = calloc(block_size, 1);
    while ((bytes_read = read(input, buffer, block_size)) > 0) {
        if (memcmp(buffer, zeros, bytes_read) == 0) {
            if (lseek(output, block_size, SEEK_CUR) == -1) {
                fprintf(stderr, "Error seeking in output file: %s\n", strerror(errno));
                exit(1);
            }
        } else {
            write(output, buffer, bytes_read);
        }
        offset += bytes_read;
    }
    
    if (bytes_read == -1) {
        fprintf(stderr, "Error reading from input file: %s\n", strerror(errno));
        exit(1);
    }

	if (ftruncate(output, offset) == -1) {
        	fprintf(stderr, "Error truncating output file: %s\n", strerror(errno));
        	exit(1);
    }
    
    free(zeros);

    if (input != STDIN_FILENO) {
        close(input);
    }
    close(output);

    return 0;
}
