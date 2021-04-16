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
FILE *folderlog;

//Soal 3D: Membuat program bash killer untuk menghentikan proses
void makekiller(int argc,char **argv,int pid){
    FILE *pkiller = fopen("killer.sh", "w");

    if(strcmp(argv[1], "-x") == 0 && argc == 2){ 
        // Soal 3E: Menghentikan proses namun membiarkan proses selesai
        fprintf(pkiller, "#!/bin/bash\nkill %d\nrm \"$0\"", pid);
    }else{ 
        //Soal 3E: Menghentikan proses paksa
        fprintf(pkiller, "#!/bin/bash\nkillall -9 soal3\nrm \"$0\"");
    }

    fclose(pkiller);

    if(fork() == 0){
        char *chomdarg[] = {"chmod", "+x", "killer.sh", NULL};
        execv("/bin/chmod", chomdarg);
    }
}

//Soal 3C : Enkripsi Caesar Cipher
void caesarcipher(char* rawstr,int key){
    char ch;
    int i;
    for (i=0;rawstr[i]!='\0';i++){
        ch = rawstr[i];
        if(ch>='a'&&ch<='z'){
            ch=ch+key;
            if (ch > 'z') ch = ch-'z'+'a'-1;
            rawstr[i] = ch;
        }
        else if(ch>='A'&&ch<='Z'){
            ch=ch+key;
            if (ch > 'Z') ch = ch-'Z'+'A'-1;
            rawstr[i] = ch;
        }
    }
}

//Soal 3B: Mendownload 10 gambar setiap 5 detik dengan ukuran time%1000+50
void downloadgambar(char *namafolder){
    time_t rawtime;
    int i,ukuran;
    char namagambar[204],buffer[100],url[100];

    for(i=0;i<10;i++){
        time(&rawtime);
        strftime(buffer, 100, "%Y-%m-%d_%X", localtime(&rawtime));
        
        sprintf(namagambar,"%s/%s.jpg",namafolder,buffer);
        ukuran = (int)time(NULL);
        ukuran = (ukuran%1000)+50;
        sprintf(url,"https://picsum.photos/%d",ukuran);

        if(fork()==0){
            char *dlarg[] = {"wget", "-qO", namagambar, url, NULL};
            execv("/usr/bin/wget", dlarg);
        }
        fprintf(folderlog,"Gambar %s berhasil diunduh\n",namagambar);
        fflush(folderlog);
        sleep(5);
    }
}

//Soal 3C: Membuat zip dari folder yang sudah diisi 10 gambar dan menghapus direktori
void buatzip(char *namafolder){
    char namazip[112];
    sprintf(namazip,"%s.zip",namafolder);
    char *zipargv[] = {"zip", "-rmq", namazip, namafolder, NULL};
    execv("/usr/bin/zip", zipargv);
}

//Soal 3C: Membuat status.txt yang dienkripsi di dalam folder setelah berhasil download semua gambar
void buatstatustext(char *namafolder){    
    char statusdl[112];
    char pesandl[20] = "Download Success";
    sprintf(statusdl,"%s/status.txt",namafolder);

    caesarcipher(pesandl,5);
    FILE *isdownload = fopen(statusdl,"w");
    fprintf(isdownload,"%s",pesandl);
    fclose(isdownload);
}

//Soal 3A : Membuat folder berdasarkan timestamp [YYYY-MM-dd_hh:mm:ss] per 40 detik
void buatfolder(char *namafolder){
    if(fork()==0){ 
        char *mkdirarg[] = {"mkdir", "-p",namafolder, NULL};
        execv("/bin/mkdir", mkdirarg);
    }
}

void paketcomplete(char *namafolder){
    int s1;
    if(fork()==0){ 
        fprintf(folderlog,"Folder %s berhasil dibuat\n",namafolder);
        fflush(folderlog);

        //Soal 3B: Mendownload 10 gambar setiap 5 detik dengan ukuran time%1000+50
        downloadgambar(namafolder);

        //Soal 3C: Membuat status.txt yang dienkripsi di dalam folder setelah berhasil download semua gambar
        buatstatustext(namafolder);
        fprintf(folderlog,"Status pesan : status.txt berhasil dibuat\n");
        fflush(folderlog);

        //Soal 3C: Membuat zip dari folder yang sudah diisi 10 gambar
        buatzip(namafolder);
    }
}

void programutama(){
    time_t rawtime;
    char namafoldertemp[100];

    while (1){
        int s1,s2;

        //Membuat nama folder berdasarkan timestamp [YYYY-MM-dd_hh:mm:ss]
        time(&rawtime);
        strftime(namafoldertemp, 100, "%Y-%m-%d_%X", localtime(&rawtime));
        
        //Membuat Folder
        buatfolder(namafoldertemp);

        //Menunggu folder dibuat
        while((wait(&s1))>0);

        //Membuat zip yang berisi folder yang telah diisi 10 gambar
        paketcomplete(namafoldertemp);

        //Menunggu folder sudah di zip
        while((wait(&s1))>0);

        fprintf(folderlog,"Status Zip : File zip %s.zip sudah berhasil dibuat\n",namafoldertemp);
        fflush(folderlog);

        sleep(40); //Soal 3A: Direktori/40detik
    }
}

int main(int argc, char **argv){
    pid_t pid, sid; // Variabel untuk menyimpan PID
    char namafoldertemp[100];
    time_t rawtime;

    pid = fork(); // Menyimpan PID dari Child Process

    /* Keluar saat fork gagal (nilai variabel pid < 0) */
    if (pid < 0) exit(EXIT_FAILURE);

    /* Keluar saat fork berhasil (nilai variabel pid adalah PID dari child process) */
    if (pid > 0) exit(EXIT_SUCCESS);

    umask(0);

    sid = setsid();
    if (sid < 0) exit(EXIT_FAILURE);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    folderlog = fopen("folderlog.txt", "w+");

    //Soal 3D : Membuat killer bash program
    makekiller(argc,argv,(int)getpid());

    fprintf(folderlog,"Program Killer dibuat\n");
    fflush(folderlog);

    //Program Utama
    programutama();

    fclose(folderlog);
}