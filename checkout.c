#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <time.h>

// Function to checkout a specific commit on a specific branch (both must be specified)
int checkout(char* branchName, char* commitHash){

    // Delete all the files/folders in the current working directory except the .chas folder

    char deleteCommand[100];
    // This won't delete the .chas folder, * by default doesn't delete hidden files
    //sprintf(deleteCommand, "rm -rf *");

    sprintf(deleteCommand, "rm -rf ./temp/*");

    system(deleteCommand);
   
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

    // ./temp/ here just for testing purposes remove this later and change to .
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

    // copy the staging.txt file from the commit directory to the staging.txt file in the .chas folder
    char stagingFilePath[100];
    sprintf(stagingFilePath, "%s/staging.txt", commitFolderName);

    char copyCommand[1000];
    sprintf(copyCommand, "cp %s %s", stagingFilePath, ".chas/staging.txt");

    system(copyCommand);

    return 0;
}