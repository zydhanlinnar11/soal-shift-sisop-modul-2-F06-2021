#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>

#define BANYAK_FOLDER 3

void downloadFiles(pid_t child_id, int *status, char *filename[]) {
    char *download_links[] = {"https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download",
        "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download",
        "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download"};

    for(int i=0; i<BANYAK_FOLDER; i++) {
        if((child_id = fork()) == 0) {
            char *argv[] = {"wget", "--no-check-certificate",
                download_links[i],
                "-O", filename[i], "-q", NULL};
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

void removeExtractedFolders(pid_t child_id, int * status, char * foldername[]) {
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
    } else perror ("Couldn't open the directory");
}

void makeDirectories(pid_t child_id, int * status, char *foldername[], char *stevany_foldername[]) {
    if((child_id = fork()) == 0) {
        char *argv[] = {"mkdir", "-p", stevany_foldername[0], stevany_foldername[1], stevany_foldername[2], NULL};
        execv("/bin/mkdir", argv);
    }
    while(wait(status) > 0);
}

void zipStevanyFolders(pid_t child_id, int * status, char *stevany_foldername[], char zip_name[]) {
    if((child_id = fork()) == 0) {
        char *argv[] = {"zip", "-rmvq", zip_name, stevany_foldername[0], stevany_foldername[1], stevany_foldername[2], NULL};
        execv("/bin/zip", argv);
    }
    while(wait(status) > 0);
}

void stevany(pid_t child_id, char *stevany_foldername[]) {
    int status;

    char *filename[] = {"FILM.zip", "MUSIK.zip", "FOTO.zip"};
    char *foldername[] = {"FILM", "MUSIK", "FOTO"};

    makeDirectories(child_id, &status, foldername, stevany_foldername);
    downloadFiles(child_id, &status, filename);
    unzipFiles(child_id, &status, filename);    

    for(int i=0; i<BANYAK_FOLDER; i++)
        browseFolderThenMoveFiles(&status, foldername[i], stevany_foldername[i]);

    removeExtractedFolders(child_id, &status, foldername);
}

int isNowStevanyBirthday(int day, int month) {
    const int stev_b_day = 9;
    const int stev_b_month = 3;

    return stev_b_day == day && stev_b_month == month;
}

void runDaemon(int * status) {
    pid_t child_id;
    const unsigned SLEEP_INTERVAL_SECONDS = 1;
    char *stevany_foldername[] = {"Fylm", "Musyik", "Pyoto"};
    const int stev_b_hour = 22;
    const int stev_b_min = stev_b_hour;
    while (1) {
        time_t now = time(NULL);
        struct tm * nowLocal = localtime(&now);

        if(isNowStevanyBirthday(nowLocal->tm_mday, nowLocal->tm_mon) &&
            stev_b_hour == nowLocal->tm_hour && stev_b_min == nowLocal->tm_min &&
            nowLocal->tm_sec == 0)
            zipStevanyFolders(child_id, status, stevany_foldername, "Lopyu_Stevany.zip");
        else if(isNowStevanyBirthday(nowLocal->tm_mday, nowLocal->tm_mon) &&
            stev_b_hour - 6 == nowLocal->tm_hour && stev_b_min == nowLocal->tm_min &&
            nowLocal->tm_sec == 0)
            stevany(child_id,stevany_foldername);
        while(wait(status) > 0);

        sleep(SLEEP_INTERVAL_SECONDS);
    }
}

int main() {
    const char workingDir[] = "/home/zydhanlinnar11/soal-shift-sisop-modul-2-F06-2021";
    pid_t child_id, sid;
    int status;
    
    if((child_id = fork()) > 0)
        exit(EXIT_SUCCESS);

    umask(0);

    sid = setsid();
    if (sid < 0 || chdir(workingDir))
        exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    runDaemon(&status);
    return 0;
}