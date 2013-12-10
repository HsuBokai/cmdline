#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//#include "glib.h"
#include "cmdline.h"

struct _Cmdline {
//	GArray* _cmds;
	char* _cmds[MAX_LEN];
	int len;
};

void init(Cmdline* re, char input[], char cut_point)
{
	//re->_cmds = g_array_new(FALSE, FALSE, sizeof(char*));
	//g_array_ref(re->_cmds);
	char* pch, *src;
	src = input;
	pch=strchr(src, cut_point);

	re->len = 0;
	while (pch!=NULL) {
		size_t len = pch-src;
		//fprintf(stderr, "in while len = %d\n", len);
		if(len>0) {
			char* newline = (char*)calloc(len+1, sizeof(char));
			memcpy(newline, src, len);
			newline[len]='\0';
			//g_array_append_val(re->_cmds, newline);
			re->_cmds[re->len++] = newline;
		}
		src = pch+1;
		pch=strchr(src, cut_point);
	}
	if(src!=NULL) {
		size_t len = strlen(src);
		char* newline = (char*)calloc(len+1, sizeof(char));
		memcpy(newline, src, len);
		newline[len]='\0';
//fprintf(stderr, "%p\n", newline);
		//g_array_append_val(re->_cmds, newline);
		re->_cmds[re->len++] = newline;
	}
}

Cmdline* new_Cmdline(char input[], char cut_point) {
	Cmdline* re = (Cmdline*) malloc(sizeof(Cmdline));
	init(re, input, cut_point);
	return re;
}



void reset(Cmdline* obj)
{
	//GArray* a = obj->_cmds;
	//fprintf(stderr, "len = %d\n", a->len);
	int i;
	for (i = 0; i < obj->len; ++i) {
		//char* newline = g_array_index(a, char*, i);
		if(obj->_cmds[i] != NULL) {
			free(obj->_cmds[i]);
			obj->_cmds[i] = NULL;
		}
	}
	//g_array_free(obj->_cmds, FALSE);
}


void delete_Cmdline(Cmdline* obj) {
	reset(obj);
	free(obj);
	obj = NULL;
}
/*
void push_cmd(Cmdline* obj, const char* y) {
	g_array_append_val(obj->_cmds, y);
}

*/
void set_cmd(Cmdline* obj, int i, char src[]) {
	if(obj->_cmds[i] != NULL) {
		free(obj->_cmds[i]);
		obj->_cmds[i] = NULL;
	}
	unsigned int len = strlen(src);
	char* newline = (char*)calloc(len+1, sizeof(char));
	memcpy(newline, src, len);
	newline[len]='\0';
	//g_array_append_val(re->_cmds, newline);
	obj->_cmds[i] = newline;
	return;
}

char* get_cmd(Cmdline* obj, int index) {
	//return g_array_index(obj->_cmds, char*, index);
	return obj->_cmds[index];
}
int get_size(Cmdline* obj) {
	return obj->len;
}
/*
void print_cmd(Cmdline* obj) {
	printf("Array holds: \n");
	GArray* a = obj->_cmds;
	int i;
	for (i = 0; i < a->len; i++) {
		printf("%s\n", g_array_index(a, char*, i));
	}
	printf("end of array\n");
}
*/
void exec_cmd(Cmdline* obj, int index) {
/*
	char* command[16];
	GArray* a = obj->_cmds;
	int i;
	for (i = 0; i < a->len && i<15; i++) {
		command[i] = g_array_index(a, char*, i);
	}
	command[i] = 0;
*/
	obj->_cmds[obj->len] = 0;
	execv(obj->_cmds[index], obj->_cmds+index);
	return;
}


#define ARGV "argv\n"
int write_file(Cmdline* obj, char info_file_path[]) {
#ifdef DEBUG
	fprintf(stderr, "in cmdline:write_file\n"); //debug
#endif
	//check executable command file exist
	int s = obj->len;
	if(s<1) return -1;

	FILE* fPtr = fopen(info_file_path, "w+");
	if (!fPtr) {
		printf("open write_file fail...\n");
		return -1;
	}

	fprintf(fPtr, ARGV);
	int i = 0;
	for(i=0; i<s; ++i) {
		char* argv = obj->_cmds[i];
		if(argv==NULL) {
			fprintf(stderr, "argv NULL\n"); //debug
			fclose(fPtr);
			return -1;
		}
		fprintf(fPtr, "%s ", argv);
	}
	fprintf(fPtr, "\n");
	fclose(fPtr);
	return 0;
}


int read_file(Cmdline* obj, char info_file_path[])
{
#ifdef DEBUG
	fprintf(stderr, "in cmdline:read_file\n"); //debug
#endif
	FILE* fPtr = fopen(info_file_path, "r");
	if (!fPtr) {
		printf("open read_file fail...\n");
		return -1;
	}
	reset(obj);

	char line[64];
	while(fgets(line, 64, fPtr)!=NULL){
		if(strcmp(line, ARGV)==0) {
			char argv[1024];
			if(fgets(argv, 1024, fPtr)!=NULL) 
			{
				fprintf(stderr, "%s\n", argv);
				init(obj, argv, ' ');
				break;
			}
			//int i = 0;
			//for(i=0; i<MAX_PASSWORD_SIZE; ++i) {
			//	fscanf(fPtr, "%i", (unsigned int*)(password+i));
			//}
		}
	}
	fclose(fPtr);
	return 0;
}
