#ifndef UTILS_H
#define UTILS_H

#define is_debug 1

#define CHECK_RETURN(func, ret) \
	if ((ret) < 0) { \
		printf("Fail to execute %s, ret = %08Xh\n", func, ret); \
		goto __func_end; \
	} else { \
		if(is_debug) printf("Execute %s success\n", func); \
	} 

unsigned str2num(const char* buf);
void num2str(unsigned num, char ret[]);

void my_copy(const char* s, const char *d);
int my_ls_all(const char* dir);
void my_touch(const char* src);
void my_cat(const char* src);
void my_rm_r(const char* dir);
void my_cp_r(const char* dir, const char* backup_dir);

int print_array(unsigned char* buf, unsigned len);
#endif
