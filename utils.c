#include "utils.h"

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>

int parse_input(char input[], char* command[], char cut_point)
{
	int num_command = 0;
	char* pch, *src;
	src = input;
	pch=strchr(src,' ');

	while (pch!=NULL) {
		size_t len = pch-src;
		if(len>0) {
			command[num_command] = (char*)malloc(sizeof(char)*len);
			memcpy(command[num_command], src, len+1);
			command[num_command++][len]='\0';
		}
		src = pch+1;
		pch=strchr(src, cut_point);
	}
	if(src!=NULL) {
		command[num_command++] = src;
	}
	return num_command;
}


unsigned str2num(const char* buf) {
	int i = 0;
	unsigned re = 0;
	for(; i<10; ++i) {
		unsigned digit = buf[i]-'0';
		if(0 <= digit && digit < 10) {
			re = re*10 + digit;
		}
		else break;
	}
	return re;
}

void num2str(unsigned num, char ret[]) {
	sprintf(ret, "%u", num);
	/*
	   char temp_num[64];
	   int i = 0;
	   do{
	   temp_num[i++] = num%10+'0';
	   }while((num/=10)>0);
	   int j;
	   for(j = 0; j<i; ++j){
	   ret[i-j-1]=temp_num[j];
	   }
	   ret[i]='\0';
	 */
}


void print_to_log(char head[], const char* content)
{
	char command[100];
	strcpy(command,head);
	strcat(command, content);
	strcat(command, " >> /opt/VDP/log ");
	system(command);
}

void print_num_to_log(char head[], unsigned num)
{
	char pid_num[64];
	num2str(num, pid_num);
	print_to_log(head, pid_num);
}

void my_copy(const char* src, const char *bakfile) {
	printf("src = %s -> dst = %s\n", src, bakfile);
	char buff[1024]; 
	int fd1,fd2,i; 
	fd1 = open(src, O_RDONLY, 0755);  // (src, 0_RDONLY, 0755);
	fd2 = open(bakfile, O_RDWR|O_CREAT|O_TRUNC, 0755); 
	if((fd1 < 0)||(fd2 < 0))
		printf("Open Error!Check if the file is exist and you have the permission!\n"); 
	else{
		while((i = read(fd1,buff,sizeof(buff))) > 0) { 
			write(fd2,buff,i);
		}
	}
	close(fd1);
	close(fd2);
}
/*
   int load_dir(const char* s, const char* d) {
   DIR* dp = opendir(s);
   if(dp!=NULL) {
   struct dirent* ent;
   while((ent = readdir(dp))!=NULL) {
   if(ent->d_type == 8)
   {
   char src_path[64], dst_path[64];
   strcpy(src_path, s);
   strcat(src_path, "/");
   strcat(src_path, ent->d_name);
   strcpy(dst_path, d);
   strcat(dst_path, "/");
   strcat(dst_path, ent->d_name);
   my_copy(src_path, dst_path);
   }
//printf("the ent->d_reclen is%d the ent->d_type is%d the ent->d_name is%s\n", ent->d_reclen, ent->d_type, ent->d_name);  
}
closedir(dp);
}
return 0;
}
 */
int my_ls_all(const char* dir) {
	DIR* dp = opendir(dir);
	if(dp!=NULL) {
		struct dirent* ptr;
		while((ptr = readdir(dp))!=NULL) {
			printf("%s   ",ptr->d_name);
		}
		printf("\n");
		closedir(dp);
	}
	return 0;
}

void my_touch(const char* src) {
	int fd2; 
	fd2 = open(src, O_RDWR|O_CREAT|O_TRUNC, 0644); 
	if(fd2 < 0)
		printf("create error!\n"); 
	else {
		char content[] = "helloworld!!!\n";
		write(fd2, content, strlen(content));

	}
	close(fd2);
}

void my_cat(const char* src) {
	char buff[1024]; 
	int fd1, i; 
	fd1 = open(src, O_RDONLY, 0755); 
	if((fd1 < 0))
		printf("Open Error!Check if the file is exist and you have the permission!\n"); 
	else{
		while((i = read(fd1,buff,sizeof(buff))) > 0) {
			write(1,buff,i);
		}
	}
	close(fd1);
}


void my_rm_r(const char* dir) {
	//fprintf(stderr, "in my_rm_r dir=%s\n", dir); //debug
	DIR* dp = opendir(dir);
	if(dp!=NULL) {
		struct dirent* ent;
		while((ent = readdir(dp))!=NULL) {
			if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0 ) continue;
			char temp[1024];
			strcpy(temp, dir);
			strcat(temp, "/");
			strcat(temp, ent->d_name);
			if(ent->d_type == DT_DIR) {
				my_rm_r(temp);
				rmdir(temp);
			}
			else {
				//fprintf(stderr, "in my_rm_r d_name=%s\n", temp); //debug
				//fprintf(stderr, "in my_rm_r d_type=%d\n", ent->d_type); //debug
				remove(temp);
			}
		}
		closedir(dp);
	}
}


void my_cp_r(const char* dir, const char* backup_dir) {
	//fprintf(stderr, "in my_rm_r dir=%s\n", dir); //debug
	DIR* dp = opendir(dir);
	if(dp!=NULL) {
		struct dirent* ent;
		while((ent = readdir(dp))!=NULL) {
			if(strcmp(ent->d_name,".")==0 || strcmp(ent->d_name,"..")==0 ) continue;
			char temp[1024];
			strcpy(temp, dir);
			strcat(temp, "/");
			strcat(temp, ent->d_name);
			char backup[1024];
			strcpy(backup, backup_dir);
			strcat(backup, "/");
			strcat(backup, ent->d_name);
			if(ent->d_type == DT_DIR) {
				if( mkdir(backup, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) <0) {
					fprintf(stderr, "create dir=%s error\n", backup);
				}
				my_cp_r(temp, backup);
			}
			else {
				//fprintf(stderr, "in my_rm_r d_name=%s\n", temp); //debug
				//fprintf(stderr, "in my_rm_r d_type=%d\n", ent->d_type); //debug
				my_copy(temp, backup);
			}
		}
		closedir(dp);
	}
}


int print_array(unsigned char* buf, unsigned len) {
	int i = 0;
	for(i = 0; i < len; ++i){
		fprintf(stderr, "%02x", buf[i]);
	}
	fprintf(stderr, "\n");
	return 0;
}



