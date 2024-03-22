#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <time.h>

#define BUFFER_SIZE 4096

int status(){
    // get the current branch from HEAD file

    FILE* file;
    file = fopen(".chas/HEAD", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    char currentBranch[100];
    char currentCommitHash[100];

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        char *token = strtok(buffer, ":");
        if(strcmp(token, "currentBranch") == 0){
            char *branch = strtok(NULL, ":");
            strcpy(currentBranch, branch);
        }
        if(strcmp(token, "currentHeadLocation") == 0){
            char *commitHash = strtok(NULL, ":");
            strcpy(currentCommitHash, commitHash);
        }
    }

    fclose(file);

    // remove the trailing newline from the currentBranch
    currentBranch[strcspn(currentBranch, "\n")] = 0;
    currentCommitHash[strcspn(currentCommitHash, "\n")] = 0;

    // get the commit hash from the branchInfo.txt file
    char branchInfoFilePath[100];
    sprintf(branchInfoFilePath, ".chas/branches/%s/branchInfo.txt", currentBranch);

    file = fopen(branchInfoFilePath, "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    printf("Current Branch: %s\n", currentBranch);
    printf("Current Commit: %s\n\n", currentCommitHash);

    printf("Commits:\n");
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        char *message = strtok(buffer, ",");
        char *commitHash = strtok(NULL, ",");
        char *time = strtok(NULL, ",");

        printf("Message: %s, Commit: %s, Time: %s\n", message, commitHash, time);
    }

    fclose(file);

    return 0;
}