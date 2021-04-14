#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define BANYAK_FOLDER 3

void downloadFiles(pid_t child_id, int *status, char *filename[]) {
    char *download_links[] = {"https://drive.google.com/u/0/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download",
        "https://drive.google.com/u/0/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download",
        "https://drive.google.com/u/0/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download"};

    for(int i=0; i<BANYAK_FOLDER; i++) {
        if((child_id = fork()) == 0) {
            char *argv[] = {"wget", "--no-check-certificate",
                download_links[i],
                "-O", filename[i], NULL};
            execv("/bin/wget", argv);
        }
        while(wait(status) > 0);
    }
}

void unzipFiles(pid_t child_id, int * status, char *filename[]) {
    for(int i=0; i<BANYAK_FOLDER; i++) {
        if((child_id = fork()) == 0) {
            char *argv[] = {"unzip", "-qq", filename[i], NULL};
            execv("/bin/unzip", argv);
        }
        while(wait(status) > 0);
    }
}

void removeFiles(pid_t child_id, int * status, char * foldername[]) {
    if((child_id = fork()) == 0) {
        char *argv[] = {"rm", "-rf", foldername[0], foldername[1], foldername[2], NULL};
        execv("/bin/rm", argv);
    }
    while(wait(status) > 0);
}

void moveFile(struct dirent * drent, int * status, char foldername[], char stevany_foldername[]) {
    pid_t child_move = fork();
    if(child_move == 0 && (strcmp(drent->d_name, ".") == 0 || strcmp(drent->d_name, "..") == 0))
        exit(EXIT_SUCCESS);
    if(child_move == 0) {
        char filePath[256];
        strcpy(filePath, foldername);
        strcat(filePath, "/");
        strcat(filePath, drent->d_name);
        char *argv[] = {"mv", filePath, stevany_foldername, NULL};
        execv("/bin/mv", argv);
    }
    while(wait(status) > 0);
}

void browseFolderThenMoveFiles(int * status, char foldername[], char stevany_foldername[]) {
    DIR * dir = opendir(foldername);
    struct dirent * drent;
    if (dir != NULL) {
        while ((drent = readdir(dir)))
            moveFile(drent, status, foldername, stevany_foldername);

        (void) closedir (dir);
        exit(EXIT_SUCCESS);
    } else perror ("Couldn't open the directory");
}

void makeDirectories(pid_t child_id, int * status, char *foldername[], char *stevany_foldername[]) {
    if((child_id = fork()) == 0) {
        char *argv[] = {"mkdir", "-p", stevany_foldername[0], stevany_foldername[1], stevany_foldername[2], NULL};
        execv("/bin/mkdir", argv);
    }
    while(wait(status) > 0);
}

void stevany() {
    pid_t child_id;
    int status;

    char *stevany_foldername[] = {"Fylm", "Musyik", "Pyoto"};
    char *filename[] = {"FILM.zip", "MUSIK.zip", "FOTO.zip"};
    char *foldername[] = {"FILM", "MUSIK", "FOTO"};

    makeDirectories(child_id, &status, foldername, stevany_foldername);
    // downloadFiles(child_id, &status, filename);
    unzipFiles(child_id, &status, filename);    

    for(int i=0; i<BANYAK_FOLDER; i++) {
        if((child_id = fork()) == 0)
            browseFolderThenMoveFiles(&status, foldername[i], stevany_foldername[i]);
        while(wait(&status) > 0);
    }

    removeFiles(child_id, &status, foldername);
}

int main() {
    stevany();
    return 0;
}