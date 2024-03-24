# ChasVCS
- Version control system I am building. 
- Basing it off Git staging

![Git stages](./assets/git_stages.png)

## Features I am building first (local repo)
- Initialisation: Initialize an empty repository.
- Add: Add files to the repository.
- Commit: Save a snapshot of changes made to files.
- Checkout: Used to 'checkout' a specific commit on a specific branch using commit_hash.
- Branch: Create and manage branches.
- Status: Get the information, concerning modified or untracked files in the current working directory by comparing to the commit that HEAD it is pointing to. 
- Log: Get the history of all commits in the branch.
- Merge: Merge changes from one branch into another.
- Diff: Show differences between versions of files.

## Features I will build once I have completed the above (remote repo - probably use)
- Push to remote repo 
- Pull from remote repo

## How does it work / steps to use it as a VCS on your system?
- Here are the steps to get it working as a command in your terminal!:

    -  Compile the C program: Ensure that your chas program is compiled and produces a working executable named `chas`.

    - Place the executable in a directory in your PATH: You need to place the chas executable in a directory that is part of your system's PATH environment variable. This way, when you type `chas` in the terminal, the system knows where to find and execute it.

    - Common directories for user-specific binaries include `~/bin` (or `$HOME/bin`). If this directory doesn't exist, you can create it using `mkdir -p ~/bin`

    - Then, move your chas executable to this directory using `mv chas ~/bin/`

    - Update your PATH: If `~/bin` is not already in your PATH, you need to add it. You can do this by modifying your shell's configuration file (`~/.bashrc`, `~/.bash_profile`, `~/.zshrc`, etc., depending on your shell). For example, if you're using bash, you can edit `~/.bashrc` using `echo 'export PATH="$HOME/bin:$PATH"' >> ~/.bashrc`
    
    - After adding this line, either restart your terminal or run `source ~/.bashrc`. This will update your PATH to include ~/bin.

    - Make chas executable (if it's not already). If you haven't made chas executable, you can do so with the chmod command `chmod +x ~/bin/chas`.
    
    - Test it! Open a new terminal window or tab (to ensure the updated PATH is loaded) and type `chas init` to initialise a .chas repo. Check if this hidden file is created.

## Usage
- `chas init`: initialise a .chas repo in your current working directory.
- `chas branch <branch name>`: create a branch with specified name.
- `chas add <pathToFile1>, <pathTofile2> ...`: Adds files listed as arguments to the staging area.
- `chas remove <pathToFile1>, <pathTofile2> ...`: Removes files listed as arguments from the staging area.
- `chas commit "Commit message"`: Commits files in staging area.
- `chas checkout <branch name> <commit hash>`: checkout a specific commit on a specific branch (both must be specified). Note any uncommited changes to your current working directory will be lost!
- `chas status`: prints out information of modified or untracked files in the current working directory by comparing to the commit that HEAD is pointing to.
- `chas log`: prints out the entire commit history for the current branch HEAD is on. 

## TODO
- Get pull and pushing to remote repo working.
- Get merging and displaying merge conflicts working.
- Add executable to bin path and test the program, **need to remove references to temp folder which I have been using to checkout commits for testing**.