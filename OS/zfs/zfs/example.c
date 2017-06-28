#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<string.h>
//#include<zfs.h>
#include<shell.h>

#include </home/gen/apue_note/OS/zfs/include/zfs.h>

#define FS_PATH "/home/gen/zfs" 
#define K 1024
#define M 1024*1024

char current_dir[60] = "/";
uint32_t current_dir_inode = 0;
struct zfs_dir_entry cur_dir[100];
int cur_dir_num = 0;
extern char defprompt[100];

int ls(int argc, char *argv[])
{
	int i, j;
	//struct zfs_dir_entry dir[10];

	i = zfs_read_dir(current_dir_inode, cur_dir);

//	printf("%u\n",current_dir_inode);

	for(j = 0; j < i; j++) {
		if(cur_dir[j].type == 1) {
			printf("reg  ");
		} else {
			printf("dir  ");
		}

		printf("%u  ",cur_dir[j].inodes);

		int k = 0;
		for(k = 0;k < cur_dir[j].name_len;k++)
			printf("%c", cur_dir[j].name[k]);
		printf("\n");
	}
	
	cur_dir_num = i;

	return 0;
}

int mkdir(int argc, char *argv[])
{
	uint32_t inode;
	char c[2] = ".";
	char p[3] = "..";

	inode = zfs_create(0);

	zfs_write_dir(current_dir_inode, inode, argv[1], 0);
	zfs_write_dir(inode, inode, c, 0);
	zfs_write_dir(inode, current_dir_inode, p, 0);


	return 0;
}

int cd(int argc, char *argv[])
{
	int i, j = 0;
	
	if(argv[1] == NULL) {
		fprintf(stderr, "dir name is NULL!\n");
		return -1;
	}

	for(i = 0; i < cur_dir_num; i++) {
		if(strcmp(cur_dir[i].name, argv[1]) == 0) {
			current_dir_inode = cur_dir[i].inodes;
			j = zfs_read_dir(cur_dir[i].inodes, cur_dir);
			sprintf(defprompt, "[zyc@zfs:%s] $ ", argv[1]);
			strcpy(current_dir, argv[1]);
			break;
		}
	}

	if(i == cur_dir_num) {
		fprintf(stderr, "dir not find\n");
		return -1;
	}

	cur_dir_num = j;

	return 0;
}

int main(void)
{
	int fd;
	void *fsmmap;
	
	fd = open(FS_PATH, O_RDWR);
	fsmmap = mmap(NULL, 100*M, PROT_READ | PROT_WRITE, MAP_SHARED,
			fd, 0);
	if(fsmmap == MAP_FAILED) {
		printf("mmap file error\n");
		exit(1);
	}


	zfs_init(fsmmap);

	command_register("ls", ls, NULL);
	command_register("mkdir", mkdir, NULL);
	command_register("cd", cd, NULL);

	shell_init();
	shell_run();

	return 0;
}
