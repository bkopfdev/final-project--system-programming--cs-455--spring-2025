#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <limits.h>
#include <ctype.h>

#define PROC_FILE_PATH "/proc"
#define MAX_PATH_LENGTH 4096
#define TARGET_PROCESS "top"

int is_numeric(const char *str);
int read_ppid(const char *pid);
int check_process_name(const char *pid);

int main(int argc, char *argv[]) {
    DIR *proc_dir = opendir(PROC_FILE_PATH);
    if (proc_dir == NULL) {
        perror("Failed to open /proc");

        return EXIT_FAILURE;
    }

    struct dirent *entry; //structure that holds directory entries (in this case, all the files/PIDs in /proc)
    int processFound = 0;

    while ((entry = readdir(proc_dir)) != NULL) {
        if (!is_numeric(entry->d_name)) { //if the directory name is not a PID/numerical, then skip it
            
            continue;
        }

        int ppid = read_ppid(entry->d_name);
        if (ppid == -1) { //if an error occurs when reading the PPID, skip it
            printf("Failed to read PPID for PID %s\n", entry->d_name);
            
            continue;
        }

        if (check_process_name(entry->d_name)) { //if the process name matches TARGET_PROCESS, break from loop
            printf("PPID of '%s' is: %d\n", TARGET_PROCESS, ppid);
            processFound = 1;

            break;
        }
    }

    closedir(proc_dir);
    
    if(processFound) {
        printf("The '%s' process was successfully found.\n", TARGET_PROCESS);
        
        return 0;
    } else {
        printf("The '%s' process was NOT found.\n", TARGET_PROCESS);
        
        return 1;
    }
}

/**
 * is_numeric
 * Checks if a string is numbers. For this script, it checks if a string is a PID.
 * 
 * @param str The string to check.
 * @return 1 if the string is numeric, 0 otherwise.
 */
int is_numeric(const char *str) {
    while (*str) {
        if (!isdigit(*str)) {
            return 0; // Not numeric
        }
        str++;
    }
    return 1; // Numeric
}

/**
 * read_ppid
 * Takes in a PID, and returns that process's PPID (parent PID)
 * 
 * @param pid The PID of the process.
 * @return The PPID of the process, or -1 on failure.
 */
int read_ppid(const char *pid) {
    char filePath[MAX_PATH_LENGTH];
    snprintf(filePath, sizeof(filePath), "%s/%s/status", PROC_FILE_PATH, pid);

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Failed to open status file");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) { //goes through /proc/PID/status line by line
        if (strncmp(line, "PPid:", 5) == 0) { //if found the ppid line
            fclose(file);
            return atoi(line + 5); //converts the ppid value into an integer
        }
    }

    fclose(file);

    return -1; //returns -1 if ppid is not found
}

/**
 * check_process_name
 * Checks if the process name of a given PID matches the TARGET_PROCESS.
 * 
 * @param pid The PID of the process.
 * @return Returns 1 if the process name matches TARGET_PROCESS, 0 if it doesnt, and -1 on error.
 */
int check_process_name(const char *pid) {
    char filePath[MAX_PATH_LENGTH];
    snprintf(filePath, sizeof(filePath), "%s/%s/status", PROC_FILE_PATH, pid);

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Failed to open status file");
        return -1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) { //goes through /proc/PID/status line by line
        if (strncmp(line, "Name:", 5) == 0) { //if found the Name line
            char process_name[256];

            if (sscanf(line + 5, "%s", process_name) == 1) { //if sscanf was successful (also removes whitespace)
                fclose(file);
                return strcmp(process_name, TARGET_PROCESS) == 0;
            }
        }
    }

    fclose(file);
    return 0; //if name is not found
}