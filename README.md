# ChasVCS
- Version control system I am building. 
- Basing it off Git staging

![Git stages](./assets/git_stages.png)

## Features I am building first (local repo)
- Initialization: Initialize an empty repository.
- Add: Add files to the repository.
- Commit: Save a snapshot of changes made to files.
- Checkout: Used to:
    - 'checkout' a specific commit on a specific branch using commit_hash 
- Branch: Create and manage branches.
- Merge: Merge changes from one branch into another.
- Diff: Show differences between versions of files.

## Features I will build once I have completed the above (remote repo - probably use)
- Push to remote repo 
- Pull from remote repo

## How does it work?
- The code is written in C, and when compiled produces a `chas` executable
- This executable will need to be put in the directory we are using the chas commands in.
- Example usage: `./chas init`,`./chas add file.txt`

## Usage

- `./chas init`:  initialise a .chas repo in your current working directory
- `./chas branch <branch name>`: create a branch with specified name
- `./chas checkout <branch name> <commit hash>`: checkout a specific commit on a specific branch (both must be specified). Note any uncommited changes to your current working directory will be lost!
- `.chas status`: prints out information of modified or untracked files in the current working directory by comparing to the commit HEAD is pointing to.


## TODO
- Need ability to track/commit directories. Should be trivial, just need to store the path.
- Change functionality of initialising .chas repo, so that it is created in the calling directory
(not in directory that chas executable is located)
- Package C files into a seperate build folder (will help with testing when trying to checkout and 
checking untracked files etc.)