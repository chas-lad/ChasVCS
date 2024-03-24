#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>

#define BUFFER_SIZE 4096

int dealWithChangedFile(char* changedFile, char* newHash){
    // Create a temp file to store all entries except the entry we want to update

    FILE* tempFile = fopen(".chas/temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    FILE* file;
    file = fopen(".chas/staging.txt", "r");

    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    char buffer[BUFFER_SIZE];

    // Copy all entries from staging.txt to temp.txt except the entry we want to update
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        char *token = strtok(buffer, ",");
        char *fileHash = strtok(NULL, ", ");
        fileHash[strcspn(fileHash, "\n")] = 0;  // Remove trailing newline from fileHash

        if ( strcmp(changedFile, token) != 0 ){
            printf("Adding %s, %s to temp.txt\n", token, fileHash);
            fprintf(tempFile, "%s, %s\n", token, fileHash);
        }
    }

    // Add the updated entry with new hash to temp.txt
    fprintf(tempFile, "%s, %s\n", changedFile, newHash);

    fclose(file);
    fclose(tempFile);

    // Remove staging.txt and rename temp.txt to staging.txt
    remove(".chas/staging.txt");
    rename(".chas/temp.txt", ".chas/staging.txt");

    return 0;
}

char* calculateHash(FILE* file){
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha_ctx;
    int bytes_read;
    unsigned char buffer[BUFFER_SIZE];
    char* hash_string = (char*)malloc((SHA_DIGEST_LENGTH * 2 + 1) * sizeof(char));

    if (!hash_string) {
        fprintf(stderr, "Memory allocation error\n");
        return NULL;
    }

    if (!SHA1_Init(&sha_ctx)) {
        fprintf(stderr, "Error initializing SHA-1 context\n");
        free(hash_string);
        return NULL;
    }

    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) != 0) {
        if (!SHA1_Update(&sha_ctx, buffer, bytes_read)) {
            fprintf(stderr, "Error updating SHA-1 context\n");
            free(hash_string);
            return NULL;
        }
    }

    if (!SHA1_Final(hash, &sha_ctx)) {
        fprintf(stderr, "Error finalizing SHA-1 hash\n");
        free(hash_string);
        return NULL;
    }

    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(&(hash_string[i * 2]), "%02x", hash[i]);
    }
    hash_string[SHA_DIGEST_LENGTH * 2] = '\0'; // Null-terminate the string

    return hash_string;
}

int add(int fileCount, char* files[]){

    FILE *file;

    for (int i = 0; i < fileCount; i++) {

        file = fopen(files[i], "rb");

        if (file == NULL) {
            printf("Error opening file!\n");
            return 1;
        }

        char* hash = calculateHash(file);

        // check if hash exists in staging.txt file already
        file = fopen(".chas/staging.txt", "r");

        if (file == NULL) {
            printf("Error opening file!\n");
            return 1;
        }

        char buffer[BUFFER_SIZE];
        int found = 0;
        int changed = 0;
        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            char *token = strtok(buffer, ",");
            char *fileHash = strtok(NULL, ", ");
            fileHash[strcspn(fileHash, "\n")] = 0;  // Remove trailing newline from fileHash
            hash[strcspn(hash, "\n")] = 0;  // Remove trailing newline from hash

            // Check if file with same hash and filePath already exists in staging.txt
            // Important we check filePath as well, as two files can have the same hash
            // if they have the same contents. So by checking filePath, we ensure that
            // we are still allowed to have two files with the same contents but different
            // filepaths in our current working directory.
            if ( strcmp(files[i], token) == 0 && strcmp(hash, fileHash) == 0 ){
                printf("File already exists in staging area and is unchanged.\n");
                found = 1;
                changed = 0;
            }
            else if ( strcmp(files[i], token) == 0 && strcmp(hash, fileHash) != 0 ){
                found = 1;
                changed = 1;
            }
        }
        // If filepath in our staging.txt and is unchanged, skip to next file
        if (found && !changed){
            continue;
        }

        // If filepath in our staging.txt and has changed delete the entry from staging.txt
        // and add the new entry with the new updated hash
        if (found && changed){
            dealWithChangedFile(files[i], hash);
            continue;
        }


        fclose(file);

        // add entry to staging.txt file

        file = fopen(".chas/staging.txt", "a");

        if (file == NULL) {
            printf("Error opening file!\n");
            return 1;
        }

        fprintf(file, "%s, %s\n", files[i], hash);

        free(hash);
    }
    fclose(file);

    return 0;
}