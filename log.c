#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <time.h>

#define BUFFER_SIZE 4096

// Log is a built in function in C hence I named this getLog() to prevent issues in method overriding.
int getLog(char* branchName){
    
    // open the branchInfo.txt file in the relevant commit directory 
    char commitFileName[100];
    sprintf(commitFileName, ".chas/branches/%s/branchInfo.txt", branchName);

    FILE *file = fopen(commitFileName, "r");

    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // read the contents of the file and print it to the console in a nice format of commit message, commit hash, timestamp
    
    char buffer[BUFFER_SIZE];

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        // separate the buffer into commit message, commit hash, timestamp

        // Probably best to start using some heap memory as program grows
        // below method is using stack memory and this is used across the entire program
        char* commitMessage = strtok(buffer, ",");
        char* commitHash = strtok(NULL, ",");
        char* timestamp = strtok(NULL, ",");

        printf("Commit Message: %s\n", commitMessage);
        printf("Commit Hash   : %s\n", commitHash);
        printf("Timestamp     : %s\n", timestamp);

    }


    fclose(file);
    
    return 0;
}