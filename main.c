#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


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

        if (mkdir(".chas/objects", 0700) == -1) {
            // Failed to create directory
            perror("mkdir");
            return 1;
        }
        printf(".chas/objects directory created successfully.\n");

        //The pack subfolder contains compressed snapshots of the codebase
        if (mkdir(".chas/objects/pack", 0700) == -1) {
            // Failed to create directory
            perror("mkdir");
            return 1;
        }
        printf(".chas/objects/pack directory created successfully.\n");

        // The info subfolder contains metadata about the snapshots in the codebase
        if (mkdir(".chas/objects/info", 0700) == -1) {
            // Failed to create directory
            perror("mkdir");
            return 1;
        }
        printf(".chas/objects/info directory created successfully.\n");

        if (mkdir(".chas/refs", 0700) == -1) {
            // Failed to create directory
            perror("mkdir");
            return 1;
        }
        printf(".chas/refs directory created successfully.\n");

        if (mkdir(".chas/refs/heads", 0700) == -1) {
            // Failed to create directory
            perror("mkdir");
            return 1;
        }
        printf(".chas/refs/heads directory created successfully.\n");


        FILE *file = fopen(".chas/HEAD", "w");

        if (file == NULL) {
            printf("Error opening file!\n");
            return 1;
        }

        fprintf(file, "refs: refs/heads/master\n");

    } else {
        printf(".chas directory already exists.\n");
    }

    return 0;
}


int main(int argc, char* argv[]){

    while(1){

        if(argc == 1){
            printf("No arguments\n");
            return 1;
        }

        if(strcmp(argv[1], "init") == 0){
            init();
            break;
        }

        if(strcmp(argv[1], "add") == 0){
            printf("add\n");
        }

        if(strcmp(argv[1], "commit") == 0){
            printf("commit\n");
        }

        if(strcmp(argv[1], "checkout") == 0){
            printf("checkout\n");
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

    }

    return 0;
}