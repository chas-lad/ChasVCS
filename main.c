#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/sha.h>

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

        fclose(file);

    } else {
        printf(".chas directory already exists.\n");
    }

    return 0;
}

int commit(){
    // Commiting: - You would need to create a commit object that captures the state of the repository at that point in time. This commit object would contain metadata such as the commit message, author, timestamp, and a reference to the parent commit (if any).
    // - For each file staged in the index, you would retrieve its content from its original location, compute its hash, and compare it with the hash stored in the index. If the hash differs, it means the file has been modified, and you would update its entry in the repository accordingly.
    // - You would then update the index file to reflect the new state of the repository after the commit.Updating the index file with the new state of the repository after performing a commit involves ensuring that the index accurately reflects the files staged for the next commit and their corresponding hashes. When you commit changes in a version control system (VCS), you are essentially capturing the state of the repository at that point in time. After the commit, you want the index file to reflect any changes made to the staged files or their metadata.
    // - Finally, you would create a commit object containing the metadata and a reference to the root tree object representing the state of the repository at that commit.
    return 0;
}

int checkout(){
    // Checkout: - You would need to retrieve the state of the repository at that commit. This involves retrieving the root tree object associated with the commit.
    // - You would traverse the tree object recursively to retrieve the content of each file at that commit. Since the content is not stored in the repository, you would need to locate the files in their original locations and copy them to the working directory.
    // - You would also update the index file to reflect the state of the repository after checking out the commit.

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

int add(int fileCount, char* argv[]){

    // Calculate File Hashes: For each file being added, calculate a unique identifier (such as a SHA-1 hash) based on its content.

    // Store the actual content of each file in the refs/objects/ directory, with the filename being the hash of the file content. This ensures that each file's content is uniquely identified by its hash.
    // Organize Files by Hash:

    // Create a directory structure within refs/objects/ to organize the files based on their hashes. Typically, the first two characters of the hash are used as a subdirectory name to prevent having too many files in a single directory.
    // Save Files to the Repository:

    // After calculating the hash of a file's content, store the file in the appropriate location within refs/objects/ using its hash as the filename.
    // Reference in Index and Commits:

    // Instead of storing the actual file content in the index or commits, store references to the hashes of the file contents. This ensures that the index and commits remain lightweight and efficient.



    // afterwards we would have commits and then checking out a specific commit. this would be done by:

    
    // index file stores information about what is currently staged. 
    // check index file exists, if not then make one
    FILE *file = fopen(".chas/index", "w");

    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
    fclose(file);


    for (int i = 0; i < fileCount; i++) {
        printf("File: %s\n", argv[i]);

        FILE *file = fopen(argv[i], "rb");

        if (file == NULL) {
            printf("Error opening file!\n");
            return 1;
        }

        char* hash = calculateHash(file);

        fclose(file);

        printf("hash: %s", hash);

        free(hash);
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



    return 0;
}