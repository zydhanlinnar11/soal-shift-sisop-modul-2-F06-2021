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
