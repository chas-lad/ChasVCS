#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>
#include <time.h>

#include "add.h"
#include "commit.h"
#include "checkout.h"
#include "branch.h"
#include "status.h"

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
        if(argc > 3 || argc < 3){
            printf("Please provide a branch name\n");
            return 1;
        }

        return branch(argv[2]);
        
    }

    if(strcmp(argv[1], "merge") == 0){
        printf("merge\n");
    }

    if(strcmp(argv[1], "diff") == 0){
        printf("diff\n");
    }

    if(strcmp(argv[1], "status") == 0){
        
        return status();

    }

    if(strcmp(argv[1], "log") == 0){
        printf("log\n");
    }



    return 0;
}