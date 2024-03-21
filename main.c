#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <time.h>

#include "add.h"

#define BUFFER_SIZE 4096

int init(){
    // Check if the directory already exists
    struct stat st = {0};
    if (stat(".chas", &st) == -1) {
        // Directory doesn't exist, so create it
        if (mkdir(".chas", 0700) == -1) {
            // Failed to create directory
            perror("mkdir");
            return 1;
        }
        printf(".chas directory created successfully.\n");


        if (mkdir(".chas/branches", 0700) == -1) {
            // Failed to create directory
            perror("mkdir");
            return 1;
        }
        printf(".chas/branches directory created successfully.\n");

        if (mkdir(".chas/branches/master", 0700) == -1) {
            // Failed to create directory
            perror("mkdir");
            return 1;
        }
        printf(".chas/branches/master directory created successfully.\n");


        FILE *file = fopen(".chas/HEAD", "w");

        if (file == NULL) {
            printf("Error opening file!\n");
            return 1;
        }

        fprintf(file, "currentBranch:master");

        fclose(file);

        file = fopen(".chas/staging.txt", "w");

        if (file == NULL) {
            printf("Error opening file!\n");
            return 1;
        }

        fclose(file);

    } else {
        printf(".chas directory already exists.\n");
    }

    return 0;
}

int commit(char* currentBranch, char* message[]) {
    // Open staging.txt file
    FILE* file;
    file = fopen(".chas/staging.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Define variables
    char buffer[BUFFER_SIZE];
    unsigned char totalHash[SHA_DIGEST_LENGTH]; // This will store the combined hash of all files
    SHA_CTX sha_ctx;
    SHA1_Init(&sha_ctx);

    // Iterate through staging.txt to compute total hash
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        // Extract file name and its hash from the line
        char *token = strtok(buffer, ",");
        char *fileHash = strtok(NULL, ",");

        // Print file info (optional)
        printf("File: %s, Hash: %s\n", token, fileHash);

        // Update total hash with the hash of the current file
        SHA1_Update(&sha_ctx, fileHash, strlen(fileHash));
    }

    // Finalize total hash
    SHA1_Final(totalHash, &sha_ctx);

    // Convert totalHash to hexadecimal string (optional)
    char totalHashStr[SHA_DIGEST_LENGTH * 2 + 1];
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(&totalHashStr[i * 2], "%02x", totalHash[i]);
    }
    totalHashStr[SHA_DIGEST_LENGTH * 2] = '\0'; // Null-terminate the string
    printf("Total hash: %s\n", totalHashStr);

    // Close staging.txt file
    fclose(file);

    // Check if the commit directory with the total hash already exists
    struct stat st = {0};
    char commitFolderName[100];
    sprintf(commitFolderName, ".chas/branches/master/%s", totalHashStr);

    if (stat(commitFolderName, &st) == 0) {
        // Directory already exists
        printf("Nothing to commit. Add files using 'git add'\n");
        return 1;
    }

    // create commit directory with the total hash

    printf("Commit directory: %s\n", commitFolderName);

    // create commit directory with the total hash

    if (mkdir(commitFolderName, 0700) == -1) {
        // Failed to create directory
        perror("mkdir");
        return 1;
    }

    printf("Commit directory created successfully.\n");

    // zip the files from the staging area to the commit directory into a file called hash.zip 
    
    // create a zip file called hash.zip in the commit directory consisting of all the files listed in staging.txt
    char zipFileName[100];
    sprintf(zipFileName, "%s.zip", totalHashStr);

    char zipFilePath[150];
    sprintf(zipFilePath, "%s/%s", commitFolderName, zipFileName);

    char zipCommand[1000]; // might need to increase this or dynamically allocate memory for this if we are committing a lot of files
    sprintf(zipCommand, "zip %s ", zipFilePath);

    file = fopen(".chas/staging.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        char *token = strtok(buffer, ",");
        char *fileHash  = strtok(NULL, ",");
        printf("File: %s, Hash: %s\n", token, fileHash );
        sprintf(zipCommand, "%s %s", zipCommand, token);
    }

    system(zipCommand);

    // save the commit message in a file called message.txt in the commit directory
    char messageFilePath[100];
    sprintf(messageFilePath, "%s/message.txt", commitFolderName);

    file = fopen(messageFilePath, "w");

    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    fprintf(file, "%s", message[0]);

    fclose(file);

    // update the branchInfo.txt file with the total hash depending on the branch as well as the time of the commit
    char branchInfoFilePath[100];
    sprintf(branchInfoFilePath, ".chas/branches/%s/branchInfo.txt", currentBranch);

    printf("Branch info file path: %s\n", branchInfoFilePath);

    file = fopen(branchInfoFilePath, "a");

    if (file == NULL) {
        perror("Error opening file!\n");
        return 1;
    }

    // get the current time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    fprintf(file, "%s, %s, %d-%d-%d %d:%d:%d\n", message[0], totalHashStr, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

    fclose(file);

    // update the HEAD file with the current branch and the total hash (current commit)
    file = fopen(".chas/HEAD", "w");

    if (file == NULL) {
        perror("Error opening file!\n");
        return 1;
    }
    // write the current branch
    fprintf(file, "currentBranch:%s", currentBranch);
    
    // write the current commit hash
    fprintf(file, "\ncurrentHeadLocation:%s", totalHashStr);

    fclose(file);

    return 0;
}

int checkout(char* branchName, char* commitHash){
   
    // check if the branch exists
    struct stat st = {0};
    char branchFolderName[100];
    sprintf(branchFolderName, ".chas/branches/%s", branchName);

    if (stat(branchFolderName, &st) == -1) {
        // Directory doesn't exist
        printf("Branch does not exist\n");
        return 1;
    }

    // check if the commit hash exists in the branch
    char commitFolderName[100];
    sprintf(commitFolderName, ".chas/branches/%s/%s", branchName, commitHash);

    if (stat(commitFolderName, &st) == -1) {
        // Directory doesn't exist
        printf("Commit does not exist\n");
        return 1;
    }

    // unzip the files from the commit directory to the working directory
    char zipFileName[100];
    sprintf(zipFileName, "%s.zip", commitHash);

    char zipFilePath[150];
    sprintf(zipFilePath, "%s/%s", commitFolderName, zipFileName);

    char unzipCommand[150]; 

    // temp here just for testing purposes remove this later and change to .
    sprintf(unzipCommand, "unzip %s -d ./temp/", zipFilePath);

    system(unzipCommand);

    // update the HEAD file with the current branch and the total hash (current commit)
    FILE *file = fopen(".chas/HEAD", "w");

    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    // write the current branch
    fprintf(file, "currentBranch:%s", branchName);

    // write the current commit hash
    fprintf(file, "\ncurrentHeadLocation:%s", commitHash);

    fclose(file);

    return 0;
}


int main(int argc, char* argv[]){

    if(argc == 1){
        printf("No arguments\n");
        return 1;
    }

    if(strcmp(argv[1], "init") == 0){
        return init();
    }

    if(strcmp(argv[1], "add") == 0){
        if(argc == 2){
            printf("No files to add\n");
            return 1;
        }
        // send all the files to the add function
        return add(argc - 2, &argv[2]);

    }

    if(strcmp(argv[1], "commit") == 0){
        if(argc > 3 || argc < 3){
            printf("Please provide one message\n");
            return 1;
        }
        // get the current branch from HEAD file

        FILE* file;
        file = fopen(".chas/HEAD", "r");
        if (file == NULL) {
            printf("Error opening file!\n");
            return 1;
        }

        char buffer[BUFFER_SIZE];
        char currentBranch[100];

        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            char *token = strtok(buffer, ":");
            if(strcmp(token, "currentBranch") == 0){
                char *branch = strtok(NULL, ":");
                strcpy(currentBranch, branch);
            }
        }

        fclose(file);

        // remove the trailing newline from the currentBranch
        currentBranch[strcspn(currentBranch, "\n")] = 0;

        return commit(currentBranch, &argv[2]);
    }

    if(strcmp(argv[1], "checkout") == 0){
        // argv[2] == branch name, argv[3] == commit hash
        if(argc > 4 || argc < 4){
            printf("Please provide a branch name and a commit hash \n");
            return 1;
        }

        return checkout(argv[2], argv[3]);
    }

    if(strcmp(argv[1], "branch") == 0){
        printf("branch\n");
    }

    if(strcmp(argv[1], "merge") == 0){
        printf("merge\n");
    }

    if(strcmp(argv[1], "diff") == 0){
        printf("diff\n");
    }

    if(strcmp(argv[1], "status") == 0){
        printf("status\n");
    }

    if(strcmp(argv[1], "log") == 0){
        printf("log\n");
    }



    return 0;
}