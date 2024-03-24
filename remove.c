#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>

#define BUFFER_SIZE 4096

int removeFiles(int fileCount, char* files[]){
    // For each file provided in the arguments we remove the file from the staging area we
    // can do this by creating a temporary file and copying all the files except the ones
    // we want to remove to the temporary file. Then we can rename the temporary file to
    // the original file.
    FILE *stagingFile = fopen(".chas/staging.txt", "r");
    FILE *tempFile = fopen(".chas/temp.txt", "w");

    if (stagingFile == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    if (tempFile == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    char buffer[BUFFER_SIZE];

    while (fgets(buffer, BUFFER_SIZE, stagingFile) != NULL) {
        char *token = strtok(buffer, ",");
        char *fileHash = strtok(NULL, ", ");
        fileHash[strcspn(fileHash, "\n")] = 0;  // Remove trailing newline from fileHash

        int removeFile = 0;
        for (int i = 0; i < fileCount; i++) {
            if (strcmp(files[i], token) == 0) {
                removeFile = 1;
                break;
            }
        }

        if (!removeFile) {
            fprintf(tempFile, "%s, %s\n", token, fileHash);
        }
    }

    fclose(stagingFile);
    fclose(tempFile);

    remove(".chas/staging.txt");
    rename(".chas/temp.txt", ".chas/staging.txt");

    return 0;
}