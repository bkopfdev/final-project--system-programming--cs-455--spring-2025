#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
    const char *shell = NULL;
    const char *path = NULL;
    const char *username = NULL;

    for (int i = 0; envp[i] != NULL; i++) {
        if (strncmp(envp[i], "SHELL=", 6) == 0) {
            shell = envp[i] + 6;
        }

        if (strncmp(envp[i], "PATH=", 5) == 0) {
            path = envp[i] + 5;
        }

        if (strncmp(envp[i], "USERNAME=", 9) == 0) {
            username = envp[i] + 9;
        }
    }

    printf("SHELL: %s\n", shell ? shell : "was not found");
    printf("PATH: %s\n", path ? path : "was not found");
    printf("USERNAME: %s\n", username ? username : "was not found");
}