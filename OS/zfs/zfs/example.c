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

struct stack {
	char name[16][64];
	int top;
	int base;
};

struct stack name_s;

void name_spush(char *name)
{
	if(name_s.top >= 15)
		return;
	name_s.top++;
	strcpy(name_s.name[name_s.top], name);
	return;
}

void name_spop()
{
	if(name_s.top == 0)
		return;
	name_s.top--;
}

int ls(int argc, char *argv[])
{
	int i, j;
	struct zfs_inode info;
	int infoflag = 0;

	if((argv[1] != NULL) && (strcmp("-l", argv[1]) == 0)) {
		infoflag = 1;
	}

	i = zfs_read_dir(current_dir_inode, cur_dir);

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
		if(infoflag) {
			zfs_read_info(cur_dir[j].inodes, &info);
			printf("\t%u(byte)\t%u(block)", info.i_size, info.i_blocks);
		}
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

	if(strcmp(".", argv[1]) == 0)
		return 0;

	for(i = 0; i < cur_dir_num; i++) {
		if(strcmp(cur_dir[i].name, argv[1]) == 0) {
			current_dir_inode = cur_dir[i].inodes;
			j = zfs_read_dir(cur_dir[i].inodes, cur_dir);

			if(strcmp("..", argv[1]) == 0) {
				name_spop();
				strcpy(current_dir, name_s.name[name_s.top]);
			} else {
				name_spush(argv[1]);
				strcpy(current_dir, argv[1]);
			}

			sprintf(defprompt, "[zyc@zfs:%s] $ ", current_dir);
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

int touch(int argc, char *argv[])
{
	uint32_t inode = -1;
	
	inode = zfs_create(1);
	zfs_write_dir(current_dir_inode, inode, argv[1], 1);

	return 0;
}

int echo(int argc, char *argv[])
{
	if(argc < 2)
		return -1;
	
	int i;

	for(i = 0; i < cur_dir_num ; i++) {
		if(strcmp(cur_dir[i].name, argv[1]) == 0) {
			zfs_write(cur_dir[i].inodes, argv[2], strlen(argv[2]));
			break;
		}
	}

	return 0;
}

int cat(int argc, char *argv[])
{
	if(argc < 2)
		return -1;

	int i;
	uint32_t size = 1024;
	char buf[1024];

	memset(buf, 0 ,1024);

	for(i = 0 ; i < cur_dir_num ; i++) {
		if(strcmp(cur_dir[i].name, argv[1]) == 0) {
			size = zfs_read(cur_dir[i].inodes, buf, size);
			printf("%s\n", buf);
			break;
		}
	}

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

	name_s.top = -1;
	name_s.base = 0;
	name_spush("/");

	command_register("ls", ls, NULL);
	command_register("mkdir", mkdir, NULL);
	command_register("cd", cd, NULL);
	command_register("touch", touch, NULL);
	command_register("echo", echo, NULL);
	command_register("cat", cat, NULL);

	shell_init();
	shell_run();

	return 0;
}
