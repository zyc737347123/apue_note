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
	struct zfs_dir_entry dir[10];
	uint8_t *ptr;

	fd = open(FS_PATH, O_RDWR);
	fsmmap = mmap(NULL, 100*M, PROT_READ | PROT_WRITE, MAP_SHARED,
			fd, 0);
	if(fsmmap == MAP_FAILED) {
		printf("mmap file error\n");
		exit(1);
	}


	zfs_init(fsmmap);
	
	ptr = (uint8_t*)fsmmap;
	ptr += M;

	s = (struct super_block*)fsmmap;
	s->vaild = 0;
	g = (struct group_desc*)(ptr + sizeof(*s));
	bm = (struct block_bitmap*)(ptr + (s->block_size * g->bg_block_bitmap));
	printf("\n\n%d,%d\n", s->block_size, g->group_nr);
	
	int i = 0, j = 0;

	i = zfs_read_dir((uint32_t)0, dir);

	printf("i: %d\n", i);

	for(j = 0; j < i; j++) {
		if(dir[j].type == 1) {
			printf("reg  ");
		} else {
			printf("dir  ");
		}

		int k = 0;
		for(k = 0;k < dir[j].name_len;k++)
			printf("%d %c", k, dir[j].name[k]);
		printf("\n");
	}
	
	return 0;
}
