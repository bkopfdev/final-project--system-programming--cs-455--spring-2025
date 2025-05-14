#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define TARGET_FILE "README.md"
#define TARGET_STRING "### Program 3"

char *convertUppercase(char *str, size_t len);

int main() {
    int fileDescriptor = open(TARGET_FILE, O_RDWR);
    if (fileDescriptor < 0) {
        perror("open");
        return 1;
    }

    size_t length = strlen(TARGET_STRING); //length of the string to search for
    char buffer[1024];
    ssize_t bytesRead; //number of bytes already read
    off_t offset = 0; //offset to show where we are in the file (for lseek)

    while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {

        for (ssize_t i = 0; i <= bytesRead - (ssize_t)length; i++) {

            if (memcmp(&buffer[i], TARGET_STRING, length) == 0) { //if string is found
                off_t match_offset = offset + i; //set offset to correct position

                //create the uppercase string
                char uppercaseString[length + 1];
                strncpy(uppercaseString, TARGET_STRING, length);
                convertUppercase(uppercaseString, length);

                lseek(fileDescriptor, match_offset, SEEK_SET); //moves file pointer to correct position
                write(fileDescriptor, uppercaseString, length); //writes uppercase string to file
                close(fileDescriptor); //closes file descriptor

                printf("Replaced \"### Program 3\" with uppercase.\n");

                return 0;
            }
        }

        offset += bytesRead; //continue reading from the file
    }
    
    //if the string wasn't found
    close(fileDescriptor);
    printf("String not found.\n");

    return 0;
}

/**
 * convertUppercase
 * Converts a string to it's uppercase form
 * 
 * @param str The string to convert
 * @param len The length of the string
 * @return The converted string
 */
char *convertUppercase(char *str, size_t len) {
    for (size_t i = 0; i < len; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }

    return str;
}