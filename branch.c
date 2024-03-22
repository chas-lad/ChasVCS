#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <time.h>


int branch(char* branchName){
    // check if the branch already exists
    struct stat st = {0};
    char branchFolderName[100];
    sprintf(branchFolderName, ".chas/branches/%s", branchName);

    if (stat(branchFolderName, &st) == 0) {
        // Directory already exists
        printf("Branch already exists\n");
        return 1;
    }

    // create the branch directory
    if (mkdir(branchFolderName, 0700) == -1) {
        // Failed to create directory
        perror("mkdir");
        return 1;
    }

    printf("Branch directory created successfully.\n");

    // create a branchInfo.txt file in the branch directory
    char branchInfoFilePath[100];
    sprintf(branchInfoFilePath, "%s/branchInfo.txt", branchFolderName);

    FILE *file = fopen(branchInfoFilePath, "w");

    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    fclose(file);

    // update the HEAD file with the current branch
    file = fopen(".chas/HEAD", "w");

    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    // write the current branch
    fprintf(file, "currentBranch:%s", branchName);

    fclose(file);

    return 0;
}