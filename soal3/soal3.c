#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <wait.h>


//Soal 3D: Membuat program bash killer untuk menghentikan proses
void makekiller(int argc,char **argv,int pid){
    FILE *pkiller = fopen("killer.sh", "w");

    if(strcmp(argv[1], "-x") == 0){ // Soal 3E: Menghentikan proses namun membiarkan proses selesai
        fprintf(pkiller, "#!/bin/bash\nkill %d\nrm \"$0\"", pid);
    }else if(strcmp(argv[1], "-z") == 0){ //Soal 3E: Menghentikan proses paksa
        fprintf(pkiller, "#!/bin/bash\nkillall -9 soal3\nrm \"$0\"");
    }else{
        fprintf(pkiller, "#!/bin/bash\nkillall -9 soal3\nrm \"$0\"");
    }
    fclose(pkiller);

    if(fork() == 0){
        char *chomdarg[] = {"chmod", "+x", "killer.sh", NULL};
        execv("/bin/chmod", chomdarg);
    }
}

void caesarchiper(char* rawstr,int key){
    char ch;
    int i;
    for (i=0;rawstr[i]!='\0';i++){
        ch = rawstr[i];
        if(ch>='a'&&ch<='z'){
            ch=ch+key;
            if (ch > 'z') ch = ch - 'z' + 'a' - 1;
            rawstr[i] = ch;
        }
        else if(ch>='A'&&ch<='Z'){
            ch=ch+key;
            if (ch > 'Z') ch = ch - 'Z' + 'A' - 1;
            rawstr[i] = ch;
        }
    }
}

int main(int argc, char **argv){
    FILE *parentid,*folderlog;
    pid_t pid, sid; // Variabel untuk menyimpan PID
    char buffer[100], namagambartemp[204], namafoldertemp[100], url[100];
    time_t rawtime;


    pid = fork(); // Menyimpan PID dari Child Process

    /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }

    /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();
    if (sid < 0)
    {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int i,ukuran,program_id;
    pid_t p1,p2;
    folderlog = fopen("folderlog.txt", "w+");
    char pesandl[20] = "Download Success";

    int ca;
    for(ca=0;ca<argc;ca++){
        fprintf(folderlog,"argv[%d] : %s\n",ca,argv[ca]);
    }

    if(pid == 0){
        makekiller(argc,argv,(int)getpid());
    }

    fprintf(folderlog,"Program Killer dibuat\n");

    while (1){
        int s1,s2;
        //Pengosongan varibael
        strcpy(namagambartemp,"");
        strcpy(namafoldertemp,"");
        strcpy(buffer,"");

        //Membuat nama folder berdasarkan timestamp [YYYY-MM-dd_hh:mm:ss]
        time(&rawtime);
        strftime(buffer, 100, "%Y-%m-%d_%X", localtime(&rawtime));
        strcpy(namafoldertemp,buffer);

        p1 = fork();
        
        if(p1==0){ //Soal3A: Membuat direktori setiap 40 detik
            char *mkdirarg[] = {"mkdir", "-p",namafoldertemp, NULL};
            execv("/bin/mkdir", mkdirarg);
        }
        else{ //Soal 3B: Mendownload 10 gambar setiap 5 detik dengan ukuran time%1000+50
            while((wait(&s1))>0);
            fprintf(folderlog,"Folder %s berhasil dibuat\n",namafoldertemp);
            fflush(folderlog);

            for(i=0;i<10;i++){
                time(&rawtime);
                strftime(buffer, 50, "%Y-%m-%d_%X", localtime(&rawtime));
                
                sprintf(namagambartemp,"%s/%s.jpg",namafoldertemp,buffer);
                ukuran = (int)time(NULL);
                ukuran = (ukuran%1000)+50;
                sprintf(url,"https://picsum.photos/%d",ukuran);

                p2 = fork();
                if(p2==0){
                    char *dlarg[] = {"wget", "-qO", namagambartemp, url, NULL};
                    execv("/usr/bin/wget", dlarg);
                }
                fprintf(folderlog,"Gambar %s berhasil diunduh\n",namagambartemp);
                fflush(folderlog);
                sleep(5);
            }

            //SOal 3C: Membuat status.txt yang dienkripsi di dalam folder setelah berhasil download semua gambar

            char statusdl[112];
            sprintf(statusdl,"%s/status.txt",namafoldertemp);

            caesarchiper(pesandl,5);
            FILE *isdownload = fopen(statusdl,"w");
            fprintf(isdownload,"%s",pesandl);
            fclose(isdownload);
            fprintf(folderlog,"Status pesan : status.txt berhasil dibuat\n");
            fflush(folderlog);

            //Soal 3C: Membuat zip dari folder yang sudah diisi 10 gambar
            char namazip[112];
            sprintf(namazip,"%s.zip",namafoldertemp);
            if(fork()==0){
                char *zipargv[] = {"zip", "-rmq", namazip, namafoldertemp, NULL};
                execv("/usr/bin/zip", zipargv);
            }
            while((wait(&s2))>0);
            fprintf(folderlog,"Status Zip : File zip %s sudah berhasil dibuat\n",namazip);
            fflush(folderlog);
        }
        sleep(40); //Soal 3A: Direktori/40detik
    }
    fclose(folderlog);
}