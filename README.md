# soal-shift-sisop-modul-2-F06-2021

## Soal Shift Modul 2

### **Soal No. 1**

Pada suatu masa, hiduplah seorang Steven yang hidupnya pas-pasan. Steven punya pacar, namun sudah putus sebelum pacaran. Ketika dia galau memikirkan mantan, ia selalu menonton `https://www.youtube.com/watch?v=568DH_9CMKI` untuk menghilangkan kesedihannya.

Di lain hal Steven anak yang tidak amat sangat super membenci matkul sisop, beberapa jam setelah diputus oleh pacarnya dia menemukan wanita lain bernama Stevany, namun Stevany berkebalikan dengan Steven karena menyukai sisop. Steven ingin terlihat jago matkul sisop demi menarik perhatian Stevany.

Pada hari ulang tahun Stevany, Steven ingin memberikan Stevany zip berisikan hal-hal yang disukai Stevany. Steven ingin isi zipnya menjadi rapi dengan membuat folder masing-masing sesuai extensi. *(a)* Dikarenakan Stevany sangat menyukai huruf **Y**, Steven ingin nama folder-foldernya adalah **Musyik** untuk mp3, **Fylm** untuk mp4, dan **Pyoto** untuk jpg *(b)* untuk musik Steven mendownloadnya dari link di bawah, film dari link di bawah lagi, dan foto dari link dibawah juga :). *(c)* Steven tidak ingin isi folder yang dibuatnya berisikan zip, sehingga perlu **meng-extract-nya** setelah didownload serta *(d)* **memindahkannya** ke dalam folder yang telah dibuat (hanya file yang dimasukkan).

*(e)* Untuk memudahkan Steven, ia ingin semua hal di atas berjalan **otomatis** 6 jam sebelum waktu ulang tahun Stevany). *(f)* Setelah itu pada **waktu** ulang tahunnya Stevany, semua folder akan di zip dengan nama **Lopyu_Stevany.zip** dan semua folder akan di **delete**(sehingga hanya menyisakan .zip).

Kemudian Steven meminta bantuanmu yang memang sudah jago sisop untuk membantunya mendapatkan hati Stevany. Bantu Woy!!

**Note:**

- Ulang Tahun Stevany : 09 April Pukul 22.22 WIB
- Semua poin dijalankan oleh 1 script di latar belakang, termasuk mendownload file zip-nya. Jadi cukup jalankan script 1x serta ubah time dan date untuk check hasilnya.

```text
Preview :
https://drive.google.com/drive/folders/1NzRiPPoVlR_H8P51cxN4jaceeFQGk4un
*tontonnya 720p biar jelas.. ಠ‿ಠ
```

- **Tidak boleh** menggunakan fungsi system(), mkdir(), dan rename() (Yang di bahasa C) .... FORBIDDENNN!!
- **Tidak boleh** pake **cron** !!!
- Menggunakan fork dan exec.
- Link

```text
Foto :
https://drive.google.com/file/d/1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD/view
Musik :
https://drive.google.com/file/d/1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J/view
Film : 
https://drive.google.com/file/d/1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp/view
```

**Tips :**

- Gunakan fungsi wait() dan sleep() untuk memperlancarrr..
- untuk mendownload zip yang dibutuhkan bisa menggunakan command :

```text
wget --no-check-certificate "https://drive.google.com/uc?id=ID-FILE&export=download" -O Nama_untuk_filenya.ext
```

- contoh

```text
Wget --no-check-certificate "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download" -O Musik_for_Stevany.zip
```

### **Jawaban No. 1 (Fungsi Stevany)**

```c
void stevany(pid_t child_id, char *stevany_foldername[]) {
    int status;

    char *filename[] = {"FILM.zip", "MUSIK.zip", "FOTO.zip"};
    char *foldername[] = {"FILM", "MUSIK", "FOTO"};

    makeDirectories(child_id, &status, foldername, stevany_foldername);
    downloadAndUnzipFiles(child_id, &status, filename);

    for(int i=0; i<BANYAK_FOLDER; i++)
        browseFolderThenMoveFiles(&status, foldername[i], stevany_foldername[i]);

    removeExtractedFolders(child_id, &status, foldername);
}
```

### **Penjelasan No. 1 (Fungsi Stevany)**

Fungsi ini bertugas untuk memanggil fungsi lain yang melakukan pembuatan folder sesuai perintah soal, download serta unzip, dan memindahkan file yang didownload ke folder yang diinginkan.

### **Jawaban No. 1 (Fungsi membuat directory)**

```c
void makeDirectories(pid_t child_id, int * status, char *foldername[], char *stevany_foldername[]) {
    if((child_id = fork()) == 0) {
        char *argv[] = {"mkdir", "-p", stevany_foldername[0], stevany_foldername[1], stevany_foldername[2], NULL};
        execv("/bin/mkdir", argv);
    }
    while(wait(status) > 0);
}
```

### **Penjelasan No. 1 (Fungsi membuat directory)**

Fungsi ini akan melakukan fork sehingga childnya akan mengeksekusi mkdir untuk setiap folder yang diinginkan.

### **Jawaban No. 1 (Fungsi Download dan Unzip)**

Secara umum :

```c
void downloadAndUnzipFiles(pid_t child_id, int * status, char *filename[]) {
    char *download_links[] = {"https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download",
        "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download",
        "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download"};
    
    for(int i=0; i<BANYAK_FOLDER; i++) {
        if((child_id = fork()) == 0)
            downloadFile(download_links[i], filename[i]);
        while(wait(status) > 0);

        if((child_id = fork()) == 0)
            unzipFile(filename[i]);
        while(wait(status) > 0);
    }
}
```

Fungsi tersebut melakukan loop sebanyak 3x untuk mendownload file-file dari link yang ada pada array `download_links`, di setiap loop, akan dilakukan fork sebanyak dua kali dimana setiap childnya melakukan pemanggilan fungsi `downloadFile()` dan `unzipFile()`.

Fungsi `downloadFile()` :

```c
void downloadFile(char download_link[], char filename[]) {
    char *argv[] = {"wget", "--no-check-certificate",
                download_link,
                "-O", filename, "-q", NULL};
    execv("/bin/wget", argv);
}
```

Fungsi ini akan langsung mengeksekusi wget yang akan mendownload file yang diperlukan.

Fungsi `unzipFile()` :

```c
void unzipFile(char filename[]) {
    char *argv[] = {"unzip", "-qq", filename, NULL};
    execv("/bin/unzip", argv);
}
```

Fungsi ini akan langsung mengeksekusi unzip untuk mengextract file-file yang diperlukan.

### **Jawaban No. 1 (Fungsi memindahkan file-file)**

Fungsi yang mengiterasi setiap file pada directory :

```c
void browseFolderThenMoveFiles(int * status, char foldername[], char stevany_foldername[]) {
    DIR * dir = opendir(foldername);
    struct dirent * drent;
    if (dir != NULL) {
        while ((drent = readdir(dir)))
            moveFile(drent, status, foldername, stevany_foldername);

        (void) closedir (dir);
    } else perror ("Couldn't open the directory");
}
```

Fungsi ini akan melakukan pembukaan pada directory lalu mengiterasi setiap filenya dan untuk setiap file akan dipanggil fungsi `moveFile()`. Terdapat catatan apabila foldernya bernama "." ataupun "..", maka tidak perlu dilakukan pemindahan file (Tentunya karena itu merujuk pada direktori saat ini dan parentnya).

Fungsi `moveFile()` :

```c
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
```

Fungsi ini akan melakukan fork, dimana childnya akan membentuk string `filepath` yang menunjukkan path dari file yang akan dipindah, lalu menggunakannya sebagai argumen mv yang akan diexec.

### **Jawaban No. 1 (Fungsi membuang folder kosong)**

```c
void removeExtractedFolders(pid_t child_id, int * status, char * foldername[]) {
    if((child_id = fork()) == 0) {
        char *argv[] = {"rm", "-rf", foldername[0], foldername[1], foldername[2], NULL};
        execv("/bin/rm", argv);
    }
    while(wait(status) > 0);
}
```

### **Penjelasan No. 1 (Fungsi membuang folder kosong)**

Fungsi ini akan melakukan fork sehingga childnya akan mengeksekusi remove untuk setiap folder hasil extract yang isinya telah dipindahkan sehingga kosong.

### **Jawaban No. 1 (Fungsi daemon utama)**

```c
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
```

### **Penjelasan No. 1 (Fungsi daemon utama)**

Fungsi ini akan melakukan looping dengan interval waktu selama 1 detik, di setiap loopnya akan dicek apakah sekarang ulang tahun Stevany atau 6 jam sebelumnya dengan memanggil fungsi `isNowStevanyBirthday()`. Jika masih 6 jam sebelumnya, maka fungsi akan memanggil `stevany()`. Namun jika sudah ulang tahun, maka akan dipanggil fungsi `zipStevanyFolders()`

### **Jawaban No. 1 (Fungsi untuk memeriksa hari ulang tahun Stevany)**

```c
int isNowStevanyBirthday(int day, int month) {
    const int stev_b_day = 9;
    const int stev_b_month = 3;

    return stev_b_day == day && stev_b_month == month;
}
```

### **Penjelasan No. 1 (Fungsi untuk memeriksa hari ulang tahun Stevany)**

Fungsi ini hanya memeriksa apabila hari dan tanggalnya sesuai dengan soal, return 1 (`true`).

### **Jawaban No. 1 (Fungsi untuk zip Lopyu_Stevany.zip)**

```c
void zipStevanyFolders(pid_t child_id, int * status, char *stevany_foldername[], char zip_name[]) {
    if((child_id = fork()) == 0) {
        char *argv[] = {"zip", "-rmvq", zip_name, stevany_foldername[0], stevany_foldername[1], stevany_foldername[2], NULL};
        execv("/bin/zip", argv);
    }
    while(wait(status) > 0);
}
```

### **Penjelasan No. 1 (Fungsi untuk zip Lopyu_Stevany.zip)**

Fungsi ini akan melakukan forking lalu childnya akan mengeksekusi zip untuk mengarsip sesuai permintaan soal.

### **Jawaban No. 1 (Fungsi `main()`)**

```c
int main() {
    const char workingDir[] = "/home/zydhanlinnar11/soal-shift-sisop-modul-2-F06-2021/soal1";
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
```

### **Penjelasan No. 1 (Fungsi `main()`)**

Fungsi ini akan melakukan fork dan membunuh parentnya, setelah itu melakukan `umask()` dan meminta session id, setelah itu mengganti current directorynya ke direktori repo lokal, setelah itu menutup semua akses ke terminal sehingga fungsi `runDaemon()` yang melakukan semua logic dari daemon ini dapat dijalankan.

### **Kode sumber No. 1**

```c
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

void downloadFile(char download_link[], char filename[]) {
    char *argv[] = {"wget", "--no-check-certificate",
                download_link,
                "-O", filename, "-q", NULL};
    execv("/bin/wget", argv);
}

void unzipFile(char filename[]) {
    char *argv[] = {"unzip", "-qq", filename, NULL};
    execv("/bin/unzip", argv);
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

void downloadAndUnzipFiles(pid_t child_id, int * status, char *filename[]) {
    char *download_links[] = {"https://drive.google.com/uc?id=1ktjGgDkL0nNpY-vT7rT7O6ZI47Ke9xcp&export=download",
        "https://drive.google.com/uc?id=1ZG8nRBRPquhYXq_sISdsVcXx5VdEgi-J&export=download",
        "https://drive.google.com/uc?id=1FsrAzb9B5ixooGUs0dGiBr-rC7TS9wTD&export=download"};
    
    for(int i=0; i<BANYAK_FOLDER; i++) {
        if((child_id = fork()) == 0)
            downloadFile(download_links[i], filename[i]);
        while(wait(status) > 0);

        if((child_id = fork()) == 0)
            unzipFile(filename[i]);
        while(wait(status) > 0);
    }
}

void stevany(pid_t child_id, char *stevany_foldername[]) {
    int status;

    char *filename[] = {"FILM.zip", "MUSIK.zip", "FOTO.zip"};
    char *foldername[] = {"FILM", "MUSIK", "FOTO"};

    makeDirectories(child_id, &status, foldername, stevany_foldername);
    downloadAndUnzipFiles(child_id, &status, filename);

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
    const char workingDir[] = "/home/zydhanlinnar11/soal-shift-sisop-modul-2-F06-2021/soal1";
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
```

### **Output No. 1**

Berupa video yang mengarah ke YouTube, silahkan diklik.

[![demo-modul-2-sistem-operasi](https://img.youtube.com/vi/w9gAYkTxCq4/0.jpg)](https://youtu.be/w9gAYkTxCq4 "Demo Modul 2 Sistem Operasi")

### **Kendala Pengerjaan No. 1**

- Saat saya melakukan percobaan dengan mengganti tanggal, saya lupa menggantinya kembali ke tanggal saat ini sehingga beberapa commit pada repository ini tertulis dicommit pada 9 April 2021.

### **Soal No. 2**

Loba bekerja di sebuah petshop terkenal, suatu saat dia mendapatkan zip yang berisi banyak sekali foto peliharaan dan Ia diperintahkan untuk mengkategorikan foto-foto peliharaan tersebut. Loba merasa kesusahan melakukan pekerjaanya secara manual, apalagi ada kemungkinan ia akan diperintahkan untuk melakukan hal yang sama. Kamu adalah teman baik Loba dan Ia meminta bantuanmu untuk membantu pekerjaannya.

a. Pertama-tama program perlu mengextract zip yang diberikan ke dalam folder “/home/[user]/modul2/petshop”. Karena bos Loba teledor, dalam zip tersebut bisa berisi folder-folder yang tidak penting, maka program harus bisa membedakan file dan folder sehingga dapat memproses file yang seharusnya dikerjakan dan menghapus folder-folder yang tidak dibutuhkan.

b. Foto peliharaan perlu dikategorikan sesuai jenis peliharaan, maka kamu harus membuat folder untuk setiap jenis peliharaan yang ada dalam zip. Karena kamu tidak mungkin memeriksa satu-persatu, maka program harus membuatkan folder-folder yang dibutuhkan sesuai dengan isi zip.
Contoh: Jenis peliharaan kucing akan disimpan dalam “/petshop/cat”, jenis peliharaan kura-kura akan disimpan dalam “/petshop/turtle”.

c. Setelah folder kategori berhasil dibuat, programmu akan memindahkan foto ke folder dengan kategori yang sesuai dan di rename dengan nama peliharaan.
Contoh: “/petshop/cat/joni.jpg”. 

d. Karena dalam satu foto bisa terdapat lebih dari satu peliharaan maka foto harus di pindah ke masing-masing kategori yang sesuai. Contoh: foto dengan nama “dog;baro;1_cat;joni;2.jpg” dipindah ke folder “/petshop/cat/joni.jpg” dan “/petshop/dog/baro.jpg”.

e. Di setiap folder buatlah sebuah file "keterangan.txt" yang berisi nama dan umur semua peliharaan dalam folder tersebut. Format harus sesuai contoh.

```
nama : joni
umur  : 3 tahun

nama : miko
umur  : 2 tahun
```


Loba sangat mengapresiasi bantuanmu, minggu depan ia akan mentraktir makan malam!

**Note:**
- Setiap data peliharaan disimpan sebagai nama foto dengan format [jenis peliharaan]:[nama peliharaan]:[umur peliharaan dalam tahun]. Jika terdapat lebih dari satu peliharaan, data dipisahkan menggunakan underscore(_).
- Tidak boleh menggunakan fungsi system(), mkdir(), dan rename().
- Menggunakan fork dan exec.

### **Jawaban No. 2 Fungsi `my_system()`**

```c
void my_system(char command[], char *arg[]){
	int status;
	pid_t pid;
	pid = fork();
	if(pid == 0){
		execv(command, arg);
	}
	else{
		((wait(&status))>0);
	}
}
```

### **Penjelasan No. 2 Fungsi `my_system()`**

Fungsi ini bertujuan untuk meniru fungsi system() dengan melakukan fork agar childnya melakukan execv. Fungsi ini menerima command yang berisi seperti /bin/mkdir dan argumen yang diinginkan.

### **Jawaban No. 2 (mkdir dan unzip)**

```c
char locto[] = "/home/zoniacer/modul2/petshop";
int main () {
  pid_t child_id;
  int status;
  char *argv1[4] = {"mkdir","-p", locto, NULL};
  char *argv2[5] = {"unzip", "pets.zip", "-d", locto, NULL};
  
  my_system("/bin/mkdir",argv1);
  my_system("/bin/unzip",argv2);
  listFilesRecursively(locto);
  
  
  return 0;

}
```

### **Penjelasan No. 2 (mkdir dan unzip)**

Membuat argumen1 untuk membuat direktori dengan -p agar saat membuat direktori jika sudah ada tidak akan error dan locto, sebagai lokasi direktori, kemudian dilakukan unzip -d agar dapat memasuki ke direktori yang diinginkan dan locto sebagai lokasi direktori, kemudian digunakan fungsi my_system().

### **Jawaban No. 2 (menghapus file tidak diperlukan)**

```c
void listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    const char *deli1 = ";";
    const char *deli2 = "_";
    int i,j;
    FILE *fptr;
    if (!dir)
        return;
    
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            if(dp->d_type == DT_DIR){
          	char combine[100] = "/home/zoniacer/modul2/petshop/";
          	strcat(combine, dp->d_name);
          	char *argv3[] = {"rm", "-rf", combine, NULL};
          	my_system("/bin/rm", argv3);
          	}
            ...
```

### **Penjelasan No. 2 (menghapus file tidak diperlukan)**

Untuk menghapus dibuat fungsi listFilesRecursively() dari modul 2 tetapi dengan modifikasi. Saat dilihat satu - satu file dicek apabila file tersebut file direktori atau `dp->d_type == DT_DIR` jika iya maka dengan path direktori yang disimpan dalam `combine` digunakan untuk menghapus file tersebut dengan rm untuk menghapus file, `-r` dihapus secara rekursi dan f secara paksa tanpa konfirmasi.

### **Jawaban No. 2 (Membuat file direktori jenis hewan)**

Kodingan lanjutan dari sebelumnya
```c
...
	else{
          	char token[100]= "";
          	strcpy(token, dp->d_name);
          	char newfile[100] = "/home/zoniacer/modul2/petshop/";
          	strtok(token, deli1);
          	strcat(newfile, token);
          	char *argv4[] = {"mkdir","-p", newfile, NULL};
          	my_system("/bin/mkdir", argv4);
		...
```

### **Penjelasan No. 2 (Membuat file direktori jenis hewan)**

Saat dilakukan pengecekkan setiap file jika file tersebut bukan direktori maka di proses dengan variable `token` untuk mengambil kata sebelum bertemu `;` yaitu jenis hewan dengan menggunakan strtok dan variable `newfile` sebagai path untuk direktori baru. Kemudian dengan `argv4[]` dan `my_system` dibuat direktori. Ini hanya untuk file kata depan, tidak dilakukan untuk dibuat direktori dengan file yang memiliki dua jenis hewan, setelah `_` masih belom dibuat direktori.

### **Jawaban No. 2 (Memasuki gambar pada direktori jenis hewan(gambar tanpa _))**

Kodingan lanjutan dari sebelumnya
```c
...
		char mvfile[100] = "/home/zoniacer/modul2/petshop/";
        	strcat(mvfile, dp->d_name);
        	char filename[50] = "";
        	strcpy(filename, dp->d_name);
        	char s1[1]=";";
          	char s2[1]="_";
          	char s3[2]=".j";
          	if(!(strstr(dp->d_name, deli2))){
        	  	char *argv5[] = {"mv", "-f", mvfile, newfile, NULL};
        	  	char name[] = "";
        	  	my_system("/bin/mv", argv5);
			...
```

### **Penjelasan No. 2 (Memasuki gambar pada direktori jenis hewan(gambar tanpa _))**

Variable `mvfile` digunakan sebagai path sebelum dipindah dan `newfile` path jenis hewan. Kemudian disini dicek apabila file tersebut memiliki `_` jika tidak maka langsung dimasukin ke dalam direktori baru dengan `argv5[]` dan `my_system`.

### **Jawaban No. 2 (Membuat file keterangan.txt dan deskripsi(gambar tanpa _))**

Kodingan lanjutan dari sebelumnya
```c
...
			char name[] = "";
        	  	my_system("/bin/mv", argv5);
        	  	strcpy(keterangan,newfile);
        	  	strcat(keterangan,"/keterangan.txt");
        	  	fptr = fopen(keterangan,"a");
        	  	int count =2;
        	  	for(i=0;i<50;i++){
        	  		if(count==2){
        	  			if(filename[i]==s1[0]){
        	  				count--;
        	  				fprintf(fptr,"nama : ");
        	  			}
        	  			continue;
        	  		}
        	  		if(count==1){
        	  			if(filename[i]==s1[0]){
        	  				count--;
        	  				fprintf(fptr,"\numur : ");
        	  				continue;
        	  			}
        	  			fprintf(fptr,"%c", filename[i]);
        	  			strncat(name,&filename[i],1);
        	  			continue;
        	  		}
        	  		if(filename[i]==s3[0] && filename[i+1]==s3[1]){
        	  			fprintf(fptr, " tahun\n\n");
        	  			break;	
        	  		}
        	  		fprintf(fptr, "%c", filename[i]);
        	  	}
			...
```

### **Penjelasan No. 2 (Membuat file keterangan.txt dan deskripsi(gambar tanpa _))**

Dibuat file keterangan.txt dengan variable `keterangan` sebagai path untuk membuat file dengan fopen menggunakan `"a"` untuk append. Untuk menulis nama dilakukan looping dengan bantuan var `count` pertama dilakukan loopin secara berlanjut sampai bertemu `;` pertama kemudian count dikurangi dan dimasukin string `nama : `. Kemudian dilanjutkan dengan char per kata dimasukin ke dalam file keterangan sampai bertemu `;` berikutnya sekalian menyimpan nama hewan dalam variable `name` per char, count dikurangi. Setelah itu bertemu dengan `;` dimasukin string `\numur : ` dan memasuki umur hewan sampai bertemu `.j`, sesudah betemu `.j` dimasukin string ` tahun\n\n`.

### **Jawaban No. 2 (Mengubah nama gambar(gambar tanpa _))**

Kodingan lanjutan dari sebelumnya
```c
			...
			char rename[100] = "";
        	  	strcpy(rename,newfile);
        	  	strcat(rename,"/");
        	  	strcat(rename,name);
        	  	strcat(rename,".jpg");
        	  	strcat(newfile,"/");
        	  	strcat(newfile, dp->d_name);
        	  	char *argv10[] = {"mv",newfile,rename,NULL};
        	  	my_system("/bin/mv", argv10);
		}
		...
```

### **Penjelasan No. 2 (Mengubah nama gambar(gambar tanpa _))**

Dibuat path untuk nama barunya dengan variable `rename` dengan cpy variabel `newfile`, kemudian cat char `/`, cat variabel `name`, dan cat string `.jpg`, kemudian melakukan cat pada variable `newfile` agar dapat path nama gambar sebelum diubah. Dengan `argv10` dan `my_system` diubah nama file tersebut.

### **Jawaban No. 2 (Memasuki gambar pada direktori jenis hewan(gambar dengan _))**

Kodingan lanjutan dari sebelumnya
```c
		...
		else{
          		char cpyfile[100] = "";
          		char temp[10] ="";
          		int flag=1;
          		
          		int count2 =2;
          		for(i = 0; i<50;i++){
          		if(count2==2){
          			if(filename[i]==s1[0]){
          				count2--;
          				strcpy(cpyfile,locto);
          				strcat(cpyfile,"/");
          				strcat(cpyfile,temp);
          				char *argv6[] = {"mkdir","-p", cpyfile, NULL};
          				my_system("/bin/mkdir", argv6);
          				char *argv7[] = {"cp", mvfile, cpyfile, NULL};
          				my_system("/bin/cp", argv7);
          				char buatketerangan[50]="";
          				strcpy(buatketerangan,cpyfile);
          				strcat(buatketerangan,"/keterangan.txt");
          				fptr = fopen(buatketerangan,"a");
          				strcpy(temp,empty);
          				continue;
          			}
				strncat(temp,&filename[i],1);
          			continue;
          		}
				...
```

### **Penjelasan No. 2 (Membuat direktori, Memasuki gambar pada direktori jenis hewan dan Membuat file keterangan.txt(gambar dengan _))**

Karena file dengan `_` memiliki 2 jenis hewan dan perlu masuk 2 direktori yang dilakukan adalah copy gambar tersebut kedalam 2 direktori tersebut. Pertama yang dilakukan menyimpan string sampai bertemu `;`, dibuat direktori dari jenis hewan tersebut jika masih belom ada kemudian di copy gambar ke dalam direktori tersebut. Pada saat ini karena sudah ada path dalam direktori dibuat path untuk buat `keterangan.txt`. Dalam cuplikan kodingan ini variable `temp` menyimpan jenis hewan kemudian dalam `if` berikutnya akan digunakan lagi maka `temp` dilakukan `strcpy()` dengan `empty`, string yang tidak ada isinya, sama saja dengan menghapus isi `temp`. Variable `count2` sebagai penolong juga dikurangi saat bertemu `;`.

### **Jawaban No. 2 (Memasuki keterangan dan rename file(gambar dengan _))**

Kodingan lanjutan dari sebelumnya
```c
			...
			if(count2==1){
          			if(filename[i]==s1[0]){
          				count2--;
          				fprintf(fptr,"nama : %s\numur : ",temp);
          				char rename2[100] = "";
          				strcpy(rename2, cpyfile);
          				strcat(rename2,"/");
          				char now[100] = "";
          				strcpy(now,rename2);
          				strcat(rename2,temp);
          				strcat(rename2, ".jpg");
          				strcat(now,dp->d_name);
          				char *argv11[] = {"mv", now, rename2, NULL};
          				my_system("/bin/mv", argv11);
          				continue;
          			}
          			strncat(temp,&filename[i],1);
          			continue;
          		}
          		if(flag>0){
          			if(filename[i]==s2[0]){
          				flag--;
          				count2=2;
          				fprintf(fptr, " tahun\n\n");
          				strcpy(temp,empty);
          				continue;
          			}
   				fprintf(fptr,"%c",filename[i]);
   				continue;
          		}
          		if(filename[i]==s3[0] && filename[i+1] == s3[1]){
          			fprintf(fptr, " tahun\n\n");
          			break;
          		}
          		fprintf(fptr,"%c", filename[i]);
          		}
          		char *argv9[] = {"rm", mvfile, NULL};
          		my_system("/bin/rm", argv9);
          		
          	}
		}
	    strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            listFilesRecursively(path);
        }
    }

    closedir(dir);
}
		...
```

### **Penjelasan No. 2 (Memasuki keterangan dan rename file(gambar dengan _))**

Kemudian sama seperti sebelumnya dilooping dan cat char nama hewan ke dalam variable `temp` sampai bertemu `;`. Saat bertemu `;` variable `count2` dikurangi kemudian dimasukin string ke dalam `keterangan.txt`, kemudian membuat path untuk mengubah nama gambar tersebut dan path gambar saat ini, kemudian di rename dengan `argv11` dan `my_system`. Kemudian dicek apabila variable `flag` lebih dari 1, kemudian dilooping dan memasuki umur hewan sampai bertemu `_`. Saat bertemu `_` variable `flag` dikurangi, variable `count2` kembali manjadi 2, memasuki string sesuai permintaan soal dan variable `temp` dikosongi. Karena variable `count2` kembali menjadi 2 maka diulangi proses ini sampai `count2` 0, saat itu dimasukin umur hewan sampai bertemu `.j` kemudian karena gambar sudah masuk dua gambar maka gambar original dihapus.

### **Source Code Soal No.2**

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <dirent.h>

char locto[] = "/home/zoniacer/modul2/petshop";
char keterangan[100]="";
char empty[1]="";
void my_system(char command[], char *arg[]){
	int status;
	pid_t pid;
	pid = fork();
	if(pid == 0){
		execv(command, arg);
	}
	else{
		((wait(&status))>0);
	}
}

void listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    const char *deli1 = ";";
    const char *deli2 = "_";
    int i,j;
    FILE *fptr;
    if (!dir)
        return;
    
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            if(dp->d_type == DT_DIR){
          	char combine[100] = "/home/zoniacer/modul2/petshop/";
          	strcat(combine, dp->d_name);
          	char *argv3[] = {"rm", "-rf", combine, NULL};
          	my_system("/bin/rm", argv3);
          	}
          	else{
          	char token[100]= "";
          	strcpy(token, dp->d_name);
          	char newfile[100] = "/home/zoniacer/modul2/petshop/";
          	strtok(token, deli1);
          	strcat(newfile, token);
          	char *argv4[] = {"mkdir","-p", newfile, NULL};
          	my_system("/bin/mkdir", argv4);
          	char mvfile[100] = "/home/zoniacer/modul2/petshop/";
        	strcat(mvfile, dp->d_name);
        	char filename[50] = "";
        	strcpy(filename, dp->d_name);
        	char s1[1]=";";
          	char s2[1]="_";
          	char s3[2]=".j";
          	if(!(strstr(dp->d_name, deli2))){
        	  	char *argv5[] = {"mv", "-f", mvfile, newfile, NULL};
        	  	char name[] = "";
        	  	my_system("/bin/mv", argv5);
        	  	strcpy(keterangan,newfile);
        	  	strcat(keterangan,"/keterangan.txt");
        	  	fptr = fopen(keterangan,"a");
        	  	int count =2;
        	  	for(i=0;i<50;i++){
        	  		if(count==2){
        	  			if(filename[i]==s1[0]){
        	  				count--;
        	  				fprintf(fptr,"nama : ");
        	  			}
        	  			continue;
        	  		}
        	  		if(count==1){
        	  			if(filename[i]==s1[0]){
        	  				count--;
        	  				fprintf(fptr,"\numur : ");
        	  				continue;
        	  			}
        	  			fprintf(fptr,"%c", filename[i]);
        	  			strncat(name,&filename[i],1);
        	  			continue;
        	  		}
        	  		if(filename[i]==s3[0] && filename[i+1]==s3[1]){
        	  			fprintf(fptr, " tahun\n\n");
        	  			break;	
        	  		}
        	  		fprintf(fptr, "%c", filename[i]);
        	  	}
        	  	char rename[100] = "";
        	  	strcpy(rename,newfile);
        	  	strcat(rename,"/");
        	  	strcat(rename,name);
        	  	strcat(rename,".jpg");
        	  	strcat(newfile,"/");
        	  	strcat(newfile, dp->d_name);
        	  	char *argv10[] = {"mv",newfile,rename,NULL};
        	  	my_system("/bin/mv", argv10);
          	}
          	else{
          		char cpyfile[100] = "";
          		char temp[10] ="";
          		int flag=1;
          		
          		int count2 =2;
          		for(i = 0; i<50;i++){
          		if(count2==2){
          			if(filename[i]==s1[0]){
          				count2--;
          				strcpy(cpyfile,locto);
          				strcat(cpyfile,"/");
          				strcat(cpyfile,temp);
          				char *argv6[] = {"mkdir","-p", cpyfile, NULL};
          				my_system("/bin/mkdir", argv6);
          				char *argv7[] = {"cp", mvfile, cpyfile, NULL};
          				my_system("/bin/cp", argv7);
          				char buatketerangan[50]="";
          				strcpy(buatketerangan,cpyfile);
          				strcat(buatketerangan,"/keterangan.txt");
          				fptr = fopen(buatketerangan,"a");
          				strcpy(temp,empty);
          				continue;
          			}
          			strncat(temp,&filename[i],1);
          			continue;
          		}
          		if(count2==1){
          			if(filename[i]==s1[0]){
          				count2--;
          				fprintf(fptr,"nama : %s\numur : ",temp);
          				char rename2[100] = "";
          				strcpy(rename2, cpyfile);
          				strcat(rename2,"/");
          				char now[100] = "";
          				strcpy(now,rename2);
          				strcat(rename2,temp);
          				strcat(rename2, ".jpg");
          				strcat(now,dp->d_name);
          				char *argv11[] = {"mv", now, rename2, NULL};
          				my_system("/bin/mv", argv11);
          				continue;
          			}
          			strncat(temp,&filename[i],1);
          			continue;
          		}
          		if(flag>0){
          			if(filename[i]==s2[0]){
          				flag--;
          				count2=2;
          				fprintf(fptr, " tahun\n\n");
          				strcpy(temp,empty);
          				continue;
          			}
   				fprintf(fptr,"%c",filename[i]);
   				continue;
          		}
          		if(filename[i]==s3[0] && filename[i+1] == s3[1]){
          			fprintf(fptr, " tahun\n\n");
          			break;
          		}
          		fprintf(fptr,"%c", filename[i]);
          		}
          		char *argv9[] = {"rm", mvfile, NULL};
          		my_system("/bin/rm", argv9);
          		
          	}
          	}
            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            listFilesRecursively(path);
        }
    }

    closedir(dir);
}


int main () {
  pid_t child_id;
  int status;
  char *argv1[4] = {"mkdir","-p", locto, NULL};
  char *argv2[5] = {"unzip", "pets.zip", "-d", locto, NULL};
  
  my_system("/bin/mkdir",argv1);
  my_system("/bin/unzip",argv2);
  listFilesRecursively(locto);
  
  
  return 0;

}
```

### **Output No. 2**
Beberapa output
![image](https://user-images.githubusercontent.com/63639703/115649141-f3ec6880-a350-11eb-9ab4-025258f240f8.png)
![image](https://user-images.githubusercontent.com/63639703/115649167-02d31b00-a351-11eb-896f-1e20b848f401.png)

### **Soal No. 3**
Ranora adalah mahasiswa Teknik Informatika yang saat ini sedang menjalani magang di perusahan ternama yang bernama “FakeKos Corp.”, perusahaan yang bergerak dibidang keamanan data. Karena Ranora masih magang, maka beban tugasnya tidak sebesar beban tugas pekerja tetap perusahaan. Di hari pertama Ranora bekerja, pembimbing magang Ranora memberi tugas pertamanya untuk membuat sebuah program.

a. Ranora harus membuat sebuah program C yang dimana setiap 40 detik membuat sebuah direktori dengan nama sesuai timestamp [YYYY-mm-dd_HH:ii:ss]. 

b. Setiap direktori yang sudah dibuat diisi dengan 10 gambar yang didownload dari https://picsum.photos/, dimana setiap gambar akan didownload setiap 5 detik. Setiap gambar yang didownload akan diberi nama dengan format timestamp [YYYY-mm-dd_HH:ii:ss] dan gambar tersebut berbentuk persegi dengan ukuran (n%1000) + 50 pixel dimana n adalah detik Epoch Unix.

c. Setelah direktori telah terisi dengan 10 gambar, program tersebut akan membuat sebuah file “status.txt”, dimana didalamnya berisi pesan “Download Success” yang terenkripsi dengan teknik Caesar Cipher dan dengan shift 5. Caesar Cipher adalah Teknik enkripsi sederhana yang dimana dapat melakukan enkripsi string sesuai dengan shift/key yang kita tentukan. Misal huruf “A” akan dienkripsi dengan shift 4 maka akan menjadi “E”. Karena Ranora orangnya perfeksionis dan rapi, dia ingin setelah file tersebut dibuat, direktori akan di zip dan direktori akan didelete, sehingga menyisakan hanya file zip saja.

d. Untuk mempermudah pengendalian program, pembimbing magang Ranora ingin program tersebut akan men-generate sebuah program “Killer” yang executable, dimana program tersebut akan menterminasi semua proses program yang sedang berjalan dan akan menghapus dirinya sendiri setelah program dijalankan. Karena Ranora menyukai sesuatu hal yang baru, maka Ranora memiliki ide untuk program “Killer” yang dibuat nantinya harus merupakan program bash.

e. Pembimbing magang Ranora juga ingin nantinya program utama yang dibuat Ranora dapat dijalankan di dalam dua mode. Untuk mengaktifkan mode pertama, program harus dijalankan dengan argumen -z, dan Ketika dijalankan dalam mode pertama, program utama akan langsung menghentikan semua operasinya Ketika program Killer dijalankan. Sedangkan untuk mengaktifkan mode kedua, program harus dijalankan dengan argumen -x, dan Ketika dijalankan dalam mode kedua, program utama akan berhenti namun membiarkan proses di setiap direktori yang masih berjalan hingga selesai (Direktori yang sudah dibuat akan mendownload gambar sampai selesai dan membuat file txt, lalu zip dan delete direktori).

Ranora meminta bantuanmu untuk membantunya dalam membuat program tersebut. Karena kamu anak baik dan rajin menabung, bantulah Ranora dalam membuat program tersebut!

**Note:**
- Tidak boleh menggunakan system() dan mkdir()
- Program utama merupakan SEBUAH PROGRAM C
- Wajib memuat algoritma Caesar Cipher pada program utama yang dibuat

### **Jawaban Soal No. 3A**

Pada Soal ini diminta untuk membuat folder dengan  format penamaan menggunakan timestamap. Lalu dalam program ini saya memisahkan program utamanya kedalam fungsi agar lebih modular. Untuk Setiap perulangannya akan memiliki jeda waktu 40 detik jadi folder akan dibuat sesuai dengan jeda waktu itu.
```C
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
```

Untuk fungsi `buatfolder()` berisi fungsi yang bertugas membuat folder dengan `execv()`, isinya sebagai berikut 
```C
void buatfolder(char *namafolder){
    if(fork()==0){ 
        char *mkdirarg[] = {"mkdir", "-p",namafolder, NULL};
        execv("/bin/mkdir", mkdirarg);
    }
}
```

### **Jawaban Soal No. 3B**

Pada Soal ini diminta untuk mendownload 10 gambar di situs https://picsum.photos/ pada folder yang telah dibuat pada soal A. Gambar akan didownload dengan jeda tiap gambar adalah 5 detik. Untuk ukuran dari tiap gambar ditentukan dari waktu `Epoch Unix` mod 1000 + 50 piksel, setelah dihitung ukurannya dapat diletakkan di url dan akan menampilkan gambar sesuai dengan ukuran yang telah dihitung. 

```C
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
```
Untuk mendapatkan waktu Epoch unix bisa didapatkan dengan fungsi `(int)time(NULL)`. Dan untuk download setiap gambarnya akan dilakukan `fork()` lalu akan menunggu selama 5 detik.

### **Jawaban Soal No. 3C**

Pada Soal ini diminta untuk membuat sebuah text file yang bernama `status.txt` yang berisi pesan `Download Success` yang menandakan bahwa program sudah berhasil mendownload 10 gambar. Tapi sebelum pesan ditulis akan dilakukan enkripsi Caesar Cipher terlebih dahulu. Berikut adalah fungsi untuk membuat file txt.

```C
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
```

Lalu untuk fungsi yang digunakan untuk enkripsi pesan dalam file text nya seperti ini
```C
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
```

### **Jawaban Soal No. 3D**

Pada Soal ini diminta untuk membuat program bash killer untuk menghentikan proses yang sedang berjalan saat ini dan mengehapus dirinya sendiri setelah menghentikan proses. Fungsi untuk membuat program killer nya sebagai berikut
```C
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
```
Pada fungsi diatas menggunakan `chmod` pada file program `killer.sh` untuk mengubah permission file nya agar bisa langsung dijalankan.

### **Jawaban Soal No. 3E**

Pada soal ini kita diminta agar program ini bisa dijalankan dengan 2 mode, yaitu dengan argumen `-x` dan `-z`. Untuk mode pertama program tidak langsung mengehntikan semua proses, tapi menunggu hingga proses selesai. Dan mode yang kedua proses yang berjalan akan dihentikan paksa secara keseluruhan, jadi akan langsung menghentikan prosesnya tanpa menunggu selesai.

Untuk mode yang pertama dilakukan hanya dengan menghentikan proses `parent`-nya saja, agar proses anaknya dapat diselesaikan. Perintah untuk menghentikan proses parentnya adalah `bash kill <PID Parent>` dan perintah untuk menghapus diri sendiri adalah `rm "$0"`.

Untuk mode kedua dilakukan dengan menghentikan semua proses yang berjalan takpeduli proses nya sudah selesai taau belum. Perintah untuk mengehntikan semua prosesnya adalah `bash killall soal3`. 

Untuk fungsi yang bertugas menjalankan program dalam 2 mode terketak sama dengan fungsi di soal sebelumnya
```C
//Soal 3D: Membuat program bash killer untuk menghentikan proses
...
    if(strcmp(argv[1], "-x") == 0 && argc == 2){ 
        // Soal 3E: Menghentikan proses namun membiarkan proses selesai
        fprintf(pkiller, "#!/bin/bash\nkill %d\nrm \"$0\"", pid);
    }else{ 
        //Soal 3E: Menghentikan proses paksa
        fprintf(pkiller, "#!/bin/bash\nkillall -9 soal3\nrm \"$0\"");
    }
...
```

### **Source Code**

Kode keseluruhan dari program ini sebagai berikut :
```C
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
```
### **Tampilan saat program dijalankan**

Berikut merupakan tampilan saat program dijalankan

![Tampilan Runtime](https://user-images.githubusercontent.com/40484843/115485057-2bd6ab80-a27e-11eb-833b-07c3e075d651.PNG)

Bisa dilihat program berjalan sesuai yang diharapkan dan mendownload gambar setiap jeda 5 detik.
**Catatan** : untuk file `folderlog.txt` hanya digunakan untuk melihat jalannya program.

### **Kendala**
Dalam pengerjaan soal nomor 3 ini ada beberapa kendala yang dialami, yaitu:
- Saat pertama kali mencoba untuk download gambar menggunakan wget hasil gambarnya selalu ukuran 0kb, setelah mencari tahu dibutuhkan parameter tambahan pada perintahnya yaitu `-q`. Karena tanpa parameter ini perintah wget akan mengeluarkan log tapi tidak di izinkan karena proses berjalan secara daemon
- Saat melakukan zip pada folder hasilnya menjadi tidak bisa dibuka/corupt. Ini juga karena tidak menambahkan parameter `-q` seperti sebelumnya
- Saat membuat program killer untuk menghentikan proses dengan menunggu hingga proses download selesai. Karena proses yang berjalan sebelumnya cuma 1 jadi proses akan langsung berhenti. Jadi dibutuhkan `fork` tambahan saat download, agar proses untuk membuat folder dan download berjalan tersendiri (tidak jadi satu)
