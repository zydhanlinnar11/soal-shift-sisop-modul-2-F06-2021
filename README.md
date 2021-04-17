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

2. Loba bekerja di sebuah petshop terkenal, suatu saat dia mendapatkan zip yang berisi banyak sekali foto peliharaan dan Ia diperintahkan untuk mengkategorikan foto-foto peliharaan tersebut. Loba merasa kesusahan melakukan pekerjaanya secara manual, apalagi ada kemungkinan ia akan diperintahkan untuk melakukan hal yang sama. Kamu adalah teman baik Loba dan Ia meminta bantuanmu untuk membantu pekerjaannya.

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

Note:
Setiap data peliharaan disimpan sebagai nama foto dengan format [jenis peliharaan]:[nama peliharaan]:[umur peliharaan dalam tahun]. Jika terdapat lebih dari satu peliharaan, data dipisahkan menggunakan underscore(_).
Tidak boleh menggunakan fungsi system(), mkdir(), dan rename().
Menggunakan fork dan exec.
