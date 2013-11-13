#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glib.h"
#include "cmdline.h"

struct _Cmdline {
	GArray* _cmds;
};

Cmdline* new_Cmdline(char input[], char cut_point) {
	Cmdline* re = (Cmdline*) malloc(sizeof(Cmdline));
	re->_cmds = g_array_new(FALSE, FALSE, sizeof(char*));

	char* pch, *src;
	src = input;
	pch=strchr(src, cut_point);

	while (pch!=NULL) {
		size_t len = pch-src;
		//fprintf(stderr, "in while len = %d\n", len);
		if(len>0) {
			char* newline = (char*)calloc(len+1, sizeof(char));
			memcpy(newline, src, len);
			newline[len]='\0';
			g_array_append_val(re->_cmds, newline);
		}
		src = pch+1;
		pch=strchr(src, cut_point);
	}
	if(src!=NULL) {
		size_t len = strlen(src);
		char* newline = (char*)calloc(len+1, sizeof(char));
		memcpy(newline, src, len);
		newline[len]='\0';
		g_array_append_val(re->_cmds, newline);
	}

	return re;
}

void delete_Cmdline(Cmdline* obj) {
	GArray* a = obj->_cmds;
	//fprintf(stderr, "len = %d\n", a->len);
	int i;
	for (i = 0; i < a->len; ++i) {
		free(g_array_index(a, char*, i));
	}
	g_array_free(obj->_cmds, FALSE);
	free(obj);
}

void push_cmd(Cmdline* obj, const char* y) {
	g_array_append_val(obj->_cmds, y);
}

char* get_cmd(Cmdline* obj, int index) {
	return g_array_index(obj->_cmds, char*, index);
}

int get_size(Cmdline* obj) {
	return obj->_cmds->len;
}

void print_cmd(Cmdline* obj) {
	printf("Array holds: \n");
	GArray* a = obj->_cmds;
	int i;
	for (i = 0; i < a->len; i++) {
		printf("%s\n", g_array_index(a, char*, i));
	}
	printf("end of array\n");
}

