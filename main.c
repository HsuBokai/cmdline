#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#include "utils.h"
#include "cmdline.h"

#define USAGE_UPDATE 		"* >> update [target name]         *\n"
#define USAGE_DELETE 		"* >> delete [target name]         *\n"
#define USAGE_ISSUE_TOKEN 	"* >> issue_token [target name]    *\n"

void print_usage() {
	printf("************** usage **************\n");
	printf("* >> help                         *\n");
	//printf(USAGE_UPDATE);
	//printf(USAGE_DELETE);
	//printf(USAGE_ISSUE_TOKEN);
	printf("* >> ls [absolute dir path]       *\n");
	printf("* >> cat [absolute file path]     *\n");
	printf("* >> touch [absolute file path]   *\n");
	printf("* >> exit                         *\n");
	printf("***********************************\n");
	printf("\n");
}

unsigned char* _ptr = NULL;

int main(int argc, char *argv[])
{
	printf("pid= %d\n",getpid());
	printf("helloworld is running !!! \n");
	print_usage();

	//int ret = 0;
	printf(">> ");
	char input[128];
	while(gets(input)){

		Cmdline* cmdline = new_Cmdline(input, ' ');
		//print_cmd(cmdline);

		if(strcmp(get_cmd(cmdline,0),"help")==0) {
			print_usage();
		}

		else if(strcmp(get_cmd(cmdline,0),"cp")==0) {
			my_copy(get_cmd(cmdline,1), get_cmd(cmdline,2));
		}

		else if(strcmp(get_cmd(cmdline,0),"ls")==0) {
			if(get_size(cmdline)>=2) {
				my_ls_all(get_cmd(cmdline,1));
			}
			else  printf("less args\n");
		}

		else if(strcmp(get_cmd(cmdline,0),"touch")==0) {
			if(get_size(cmdline)>=2) {
				printf("%s\n",get_cmd(cmdline,1));
				my_touch(get_cmd(cmdline,1));
			}
			else  printf("less args\n");
		}
		else if(strcmp(get_cmd(cmdline,0),"cat")==0) {
			if(get_size(cmdline)>=2) {
				my_cat(get_cmd(cmdline,1));
			}
			else  printf("less args\n");
		}

		else if(strcmp(get_cmd(cmdline,0),"write_file")==0) {
			write_file(cmdline, "./test_file");
		}
		else if(strcmp(get_cmd(cmdline,0),"read_file")==0) {
			Cmdline* another_cmdline = new_Cmdline("",' ');
			read_file(another_cmdline, "./test_file");
			fprintf(stderr, "before set_cmd() cmds[0]=%s\n", get_cmd(another_cmdline,0));
			set_cmd(another_cmdline, 0, "bokai hahahaha!! ya!!");
			fprintf(stderr, "after set_cmd() cmds[0]=%s\n", get_cmd(another_cmdline,0));
			delete_Cmdline(another_cmdline);
		}

		else if(strcmp(get_cmd(cmdline,0),"exec")==0) {
			if(get_size(cmdline)<2) printf("less args\n");

			printf("before vfork pid = %d\n",getpid());
			pid_t child_pid = vfork();
			if(child_pid==0) {
				printf("child pid = %d\n", getpid());
				exec_cmd(cmdline, 1);
				exit(0);
			}
			else {
				waitpid(child_pid, NULL, 0);
				printf("after fork pid = %d\n",getpid());
			}
		}
		else if(strcmp(get_cmd(cmdline,0),"exit")==0) {
			delete_Cmdline(cmdline);
			break;
		}

__func_end: 
		delete_Cmdline(cmdline);
		printf(">> ");
	}

	return 0;
}

