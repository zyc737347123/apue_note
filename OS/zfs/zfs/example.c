#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
//#include<zfs.h>

#include </home/gen/apue_note/OS/zfs/include/zfs.h>

#define FS_PATH "/home/gen/zfs" 
#define K 1024
#define M 1024*1024

int main(void)
{
	int fd;
	void *fsmmap;
	struct super_block *s;
	struct group_desc *g;
	struct block_bitmap *bm;

	fd = open(FS_PATH, O_RDWR);
	fsmmap = mmap(NULL, 100*M, PROT_READ | PROT_WRITE, MAP_SHARED,
			fd, 0);
	if(fsmmap == MAP_FAILED) {
		printf("mmap file error\n");
		exit(1);
	}


	zfs_init(fsmmap);
	
	fsmmap += M;

	s = (struct super_block*)fsmmap;
	//s->vaild = 0;
	g = (struct group_desc*)(fsmmap + sizeof(*s));
	bm = (struct block_bitmap*)(fsmmap + (s->block_size * g->bg_block_bitmap));
	printf("\n\n%d,%d\n", s->vaild, g->group_nr);
	
	
	return 0;
}
