#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <stdbool.h>
#include <wait.h>
#include <time.h>

FILE *folderlog = NULL;

void caesarchiper(char* rawstr,int key){
    char ch;
    int i;
    for (i = 0; rawstr[i] != '\0'; ++i){
        ch = rawstr[i];

        if (ch >= 'a' && ch <= 'z'){
            ch = ch + key;
            if (ch > 'z') ch = ch - 'z' + 'a' - 1;
            rawstr[i] = ch;
        }
        else if (ch >= 'A' && ch <= 'Z'){
            ch = ch + key;
            if (ch > 'Z') ch = ch - 'Z' + 'A' - 1;
            rawstr[i] = ch;
        }
    }
}

int main(){
    pid_t pid, sid; // Variabel untuk menyimpan PID

    char buffer[50],namagambartemp[50],namafoldertemp[50],url[50];

    pid = fork(); // Menyimpan PID dari Child Process

    time_t rawtime;
    struct tm *curtime;

    /* Keluar saat fork gagal
     * (nilai variabel pid < 0) 
     */
    if (pid < 0){
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0){
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0){
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    if(pid==0){
        //Mempersiapkan variabel
        folderlog = fopen("FolderLog.txt", "w+");
        pid_t p1,p2,p3;
        int s1,i; 
        long ukuran; //ukuran gambar

        while (1){
            //Pengosongan varibael
            strcpy(namagambartemp,"");
            strcpy(namafoldertemp,"");
            strcpy(buffer,"");

            //Membuat nama folder berdasarkan timestamp [YYYY-MM-dd_hh:mm:ss]
            time(&rawtime);
            strftime(buffer, 50, "%Y-%m-%d_%X", localtime(&rawtime));
            strcpy(namafoldertemp,buffer);

            p1=fork();
            
            if(p1<0) exit(0);
            //Pembuatan Folder dari timestamp
            if(p1==0){
                char *argv[] = {"mkdir", "-p",namafoldertemp, NULL};
                execv("/bin/mkdir", argv);
            }
            //Mendownload 10 gambar dan dimasukkan ke folder yang dibuat
            else{
                while ((wait(&s1)) > 0); //menunggu folder dibuat
                chdir(namafoldertemp); //pindah direktori ke folder yang telah dibuat
                fprintf(folderlog,"Status Folder : Folder %s berhasil dibuat\n",namafoldertemp);
                fflush(folderlog);

                for(i=0;i<10;i++){

                    p2 = fork();

                    //Membuat nama file gambar dan urlnya
                    strcpy(buffer,"");
                    time(&rawtime);
                    strftime(buffer, 50, "%Y-%m-%d_%X", localtime(&rawtime));
                    strcpy(namagambartemp,buffer);
                    strcat(namagambartemp,".jpg");
                    ukuran = (long)time(NULL);
                    ukuran = (ukuran % 1000) + 50;
                    sprintf(url,"https://picsum.photos/%ld",ukuran);

                    //mendownload gambar
                    if(p2==0){
                        char *downloadarg[] = {"wget","-qO", namagambartemp,url, NULL};
                        execv("/usr/bin/wget", downloadarg);
                    }
                    fprintf(folderlog,"status gambar : gambar %s, url %s berhasil diunduh\n",namagambartemp,url);
                    fflush(folderlog);
                    sleep(5);
                }

                //mmebuat file status download yang ternkripsi dengan caesar chiper dengan shift 5
                char pesandownload[20] = "Download Success";
                FILE* statusdownload = fopen("status.txt", "w+");
                caesarchiper(pesandownload,5);
                fprintf(statusdownload,"%s",pesandownload);
                fclose(statusdownload);
                fprintf(folderlog,"Status pesan : status.txt berhasil dibuat\n");
                fflush(folderlog);

                chdir("..");

                p3 = fork();

                //membuat zip dari folder yang sudah diisi 10 gambar
                char namazip[100];
                strcpy(namazip,namafoldertemp);
                strcat(namazip,".zip");
                if(p3==0){
                    char *zipargv[] = {"zip", "-rm", namazip, namafoldertemp, NULL};
                    execv("/usr/bin/zip", zipargv);
                }
                fprintf(folderlog,"Status Zip : File zip %s sudah berhasil dibuat\n",namazip);
                fflush(folderlog);
            }
            sleep(40);
        }
        fclose(folderlog);
    }

}