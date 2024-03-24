#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>


// Function to check if a file exists
int file_exists(char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

// Function to compute the SHA-1 hash of a file
void compute_hash(char *filename, unsigned char *result) {
    FILE *file = fopen(filename, "rb");
    if (!file) return;

    SHA_CTX shaContext;
    SHA1_Init(&shaContext);

    char buf[1024];
    int bytesRead = 0;
    while ((bytesRead = fread(buf, 1, 1024, file))) {
        SHA1_Update(&shaContext, buf, bytesRead);
    }

    SHA1_Final(result, &shaContext);

    fclose(file);
}


int status(){
    // Where HEAD is pointing, whether that is a branch or a commit (this is where you are "checked out" to)
    // If you have any changed files in your current directory that have not yet been committed
    // If changed files are staged or not

    // Read the HEAD file
    FILE *file = fopen(".chas/HEAD", "r");

    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    char buffer[100];
    char currentBranch[100];

    while (fgets(buffer, 100, file) != NULL) {
        char *token = strtok(buffer, ":");
        if(strcmp(token, "currentBranch") == 0){
            char *branch = strtok(NULL, ":");
            strcpy(currentBranch, branch);
        }
    }

    fclose(file);

    // Read the staging.txt file
    file = fopen(".chas/staging.txt", "r");

    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    printf("Staged files:\n");

    while (fgets(buffer, 100, file) != NULL) {
        char *token = strtok(buffer, ",");
        printf("%s\n", token);
    }

    fclose(file);

    // For each of the files in staging.txt in .chas root, check if the file exists in the current directory
    // if it does check if the hashes are different. If they are then note that a file has been changed.


    file = fopen(".chas/staging.txt", "r");
    if (!file) {
        printf("Could not open staging.txt\n");
        return 1;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char *filename = strtok(line, ",");
        // In order to get next token and to continue with the same string NULL is passed as
        // first argument since strtok maintains a static pointer to your previous passed string:
        char *stored_hash = strtok(NULL, ", ");
        // Remove the newline character from the end of the string
        stored_hash[strcspn(stored_hash, "\n")] = 0;

        if (file_exists(filename)) {
            unsigned char hash[SHA_DIGEST_LENGTH];
            compute_hash(filename, hash);

            char computed_hash[SHA_DIGEST_LENGTH*2+1];
            for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
                sprintf(computed_hash + i*2, "%02x", hash[i]);
            }

            printf("stored hash: %s, computed hash: %s\n", stored_hash, computed_hash);

            if (strcmp(stored_hash, computed_hash) != 0) {
                printf("File %s has been changed. Use command 'chas add' to track these changes.\n", filename);
            }
        }
    }

    fclose(file);

    // check if there are any untracked files in the current working directory
    // - get all the files in the current directory
    // - check if they are in the staging.txt file
    // - if they are not then they are untracked files
    
    // get the current working directory
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        printf("Error getting current working directory\n");
        return 1;
    }

    // open the current working directory
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(cwd)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            // check if the file is a regular file
            if (ent->d_type == DT_REG) {
                // check if the file is in the staging.txt file
                file = fopen(".chas/staging.txt", "r");
                if (!file) {
                    printf("Could not open staging.txt\n");
                    return 1;
                }

                int found = 0;
                while (fgets(line, sizeof(line), file)) {
                    char *filename = strtok(line, ",");
                    if (strcmp(filename, ent->d_name) == 0) {
                        found = 1;
                        break;
                    }
                }

                fclose(file);

                if (!found) {
                    printf("File %s is untracked. Use command chas add' to track this file.\n", ent->d_name);
                }
            }
        }
        closedir(dir);
    } else {
        printf("Error opening current working directory\n");
        return 1;
    }

    return 0;

}