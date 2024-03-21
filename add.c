#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>

#define BUFFER_SIZE 4096

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

int add(int fileCount, char* argv[]){

    FILE *file;

    for (int i = 0; i < fileCount; i++) {
        printf("File: %s\n", argv[i]);

        file = fopen(argv[i], "rb");

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
        while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
            char *token = strtok(buffer, ",");
            char *fileHash = strtok(NULL, ", ");
            fileHash[strcspn(fileHash, "\n")] = 0;  // Remove trailing newline from fileHash
            hash[strcspn(hash, "\n")] = 0;  // Remove trailing newline from hash
            if (strcmp(hash, fileHash) == 0) {
                printf("File already exists in staging area\n");
                found = 1;
            }
        }
        // If file already exists in staging.txt, skip to next file
        if (found){
            continue;
        }


        fclose(file);

        // add entry to staging.txt file

        file = fopen(".chas/staging.txt", "a");

        if (file == NULL) {
            printf("Error opening file!\n");
            return 1;
        }

        fprintf(file, "%s, %s\n", argv[i], hash);

        free(hash);
    }
    fclose(file);

    return 0;
}