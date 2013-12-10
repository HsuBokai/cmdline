#ifndef _Cmdline_H_
#define _Cmdline_H_

#define MAX_LEN 64

typedef struct _Cmdline Cmdline;
Cmdline* new_Cmdline(char input[], char cut_point);
void delete_Cmdline(Cmdline* obj);

//void push_cmd(Cmdline* obj, const char* y);
void set_cmd(Cmdline* obj, int i, char src[]);
char* get_cmd(Cmdline* obj, int index);
int get_size(Cmdline* obj);
void print_cmd(Cmdline* obj);
void exec_cmd(Cmdline* obj, int index);

int write_file(Cmdline* obj, char info_file_path[]);
int read_file(Cmdline* obj, char info_file_path[]);
#endif
