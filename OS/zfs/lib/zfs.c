#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
//#include <zfs.h>

#include </home/gen/apue_note/OS/zfs/include/zfs.h>

#define FS_PATH /home/gen/zfs
#define K 1024
#define M 1024*1024
#define GROUP_NUM 100

extern char defprompt[100];

struct super_block s_model;
struct group_desc g_model;

uint8_t *filesys = NULL;

void get_super_block()
{
	memcpy(&s_model, filesys, sizeof(struct super_block));
}

void zfs_init_zero(uint8_t *byte, int g_nr)
{
	struct block_bitmap *bmap;
	struct inode_bitmap *imap;
	struct inode_table *itable;
	struct zfs_dir_entry *zdir;
	int i;
	uint8_t *bytep = byte; //block 0

	s_model.vaild = 1;
	s_model.inodes_count = 1024 * GROUP_NUM;
	s_model.block_count = 1024 * GROUP_NUM;
	s_model.block_size = 1*K;
	s_model.blocks_per_group = 1*K;
	s_model.inodes_per_group = 1*K;
	s_model.inode_size = sizeof(struct zfs_inode);
	s_model.inode_table_size = s_model.inode_size * s_model.inodes_per_group;

	g_model.group_nr = g_nr;
	g_model.bg_block_bitmap = (g_nr * s_model.blocks_per_group) + 1;
	g_model.bg_inode_bitmap = g_model.bg_block_bitmap + 1;
	g_model.bg_inode_table = g_model.bg_inode_bitmap + 1;
	g_model.free_block_count = 1024 - 68;
	g_model.free_inodes_count = 1024 - 1;

	memcpy((void*)bytep, (void*)&s_model, sizeof(s_model));
	memcpy((void*)(bytep + sizeof(s_model)), (void*)&g_model, sizeof(g_model));

	bytep += 1*K; // block 1
	bmap = (struct block_bitmap*)bytep;
	memset(bmap, 0, sizeof(struct block_bitmap));
	for(i = 0;i < 68;i++) {
		bmap->map[i] = 1;
	}

	bytep += 1*K; // block 2
	imap = (struct inode_bitmap*)bytep;
	memset(imap, 0, sizeof(struct inode_bitmap));
	imap->map[0] = 1; // for root dir
	
	bytep += 1*K; // block 3
	itable = (struct inode_table*)bytep;
	memset(itable, 0, sizeof(struct inode_table));
	itable->table[0].i_size = 1*K;
	itable->table[0].i_blocks = 1;
	itable->table[0].i_block[0] = 67;

	bytep += 64*K; // block 67
	zdir = (struct zfs_dir_entry*)bytep;
	zdir->inodes = 0;
	zdir->type = 0; // type 0 mean dir
	zdir->name_len = 1;
	zdir->name[0] = '.';

	return;
}

void zfs_init_other(uint8_t *byte, int g_nr)
{
	struct block_bitmap *bmap;
	struct inode_bitmap *imap;
	struct inode_table *itable;
	int i;
	uint8_t *bytep = byte;

	g_model.group_nr = g_nr;
	g_model.bg_block_bitmap = (g_nr * s_model.blocks_per_group) + 1;
	g_model.bg_inode_bitmap = g_model.bg_block_bitmap + 1;
	g_model.bg_inode_table = g_model.bg_inode_bitmap + 1;
	g_model.free_block_count = 1024 - 67;
	g_model.free_inodes_count = 1024;

	memcpy((void*)bytep, (void*)&s_model, sizeof(s_model));
	memcpy((void*)(bytep + sizeof(s_model)), (void*)&g_model, sizeof(g_model));

	bytep += 1*K; // block 1
	bmap = (struct block_bitmap*)bytep;
	memset(bmap, 0, sizeof(struct block_bitmap));
	for(i = 0;i < 67;i++) {
		bmap->map[i] = 1;
	}

	bytep += 1*K; // block 2
	imap = (struct inode_bitmap*)bytep;
	memset(imap, 0, sizeof(struct inode_bitmap));
	
	bytep += 1*K; // block 3
	itable = (struct inode_table*)bytep;
	memset(itable, 0, sizeof(struct inode_table));

	return;

}
void zfs_init(void *fsmmap)
{
	/*test*/
	sprintf(defprompt, "[zyc@zfs:/] $ ");
	/*test*/

	int i;
	struct super_block *sp;
	uint8_t *bytep;

	filesys = (uint8_t*)fsmmap;

	sp = (struct super_block*)fsmmap;
	bytep = (uint8_t*)fsmmap;

	
	if(sp->vaild ) {
		get_super_block();
		printf("zfs had init\n");
		return;
	}

	memset(fsmmap, 0, 100*M);

	for(i = 0;i < GROUP_NUM;i++, bytep += M){
		if(i == 0){
			zfs_init_zero(bytep, i);
			continue;
		}
		zfs_init_other(bytep, i);
	}

}

int zfs_read_info(uint32_t inodes, struct zfs_inode *info)
{
	int g_nr, inode_offset;
	uint8_t *bytep;
	struct group_desc *gd;
	struct zfs_inode zfsi;
	struct inode_table *it;

	g_nr = inodes / s_model.inodes_per_group;
	inode_offset = inodes % s_model.inodes_per_group;

	bytep = filesys + (g_nr * s_model.blocks_per_group * 
			s_model.block_size);

	bytep += sizeof(s_model);	

	gd = (struct group_desc*)bytep;

	bytep = filesys + (gd->bg_inode_table * s_model.block_size);

	it = (struct inode_table*)bytep;

	zfsi = it->table[inode_offset];
	*info = zfsi;

	return 0;
}

int zfs_read(uint32_t inodes, char *buf, const uint32_t size)
{
	int g_nr, inode_offset;
	uint32_t i;
	uint8_t *bytep;
	struct group_desc *gd;
	struct zfs_inode zfsi;
	struct inode_table *it;

	g_nr = inodes / s_model.inodes_per_group;
	inode_offset = inodes % s_model.inodes_per_group;

	bytep = filesys + (g_nr * s_model.blocks_per_group * 
			s_model.block_size);

	bytep += sizeof(s_model);	

	gd = (struct group_desc*)bytep;

	bytep = filesys + (gd->bg_inode_table * s_model.block_size);

	it = (struct inode_table*)bytep;

	zfsi = it->table[inode_offset];

	int j = 0, k = 0;

	for(i = 0; i < size && i < zfsi.i_size; i++) {
		if(i % s_model.block_size == 0) {
			bytep = filesys + (zfsi.i_block[j++] * s_model.block_size);
			k = 0;
		}
		buf[i] = bytep[k++];
	}

	return i+1;

}

int zfs_read_dir(uint32_t inodes, struct zfs_dir_entry *zde)
{
	int g_nr, inode_offset;
	uint32_t i = 0;
	uint8_t *bytep;
	struct group_desc *gd;
	struct zfs_inode zfsi;
	struct inode_table *it;
	struct zfs_dir_entry *tmp_zde;

	g_nr = inodes / s_model.inodes_per_group;

	inode_offset = inodes % s_model.inodes_per_group;

	bytep = filesys + (g_nr * s_model.blocks_per_group * 
			s_model.block_size);

	bytep += sizeof(s_model);	

	gd = (struct group_desc*)bytep;

	bytep = filesys + (gd->bg_inode_table * s_model.block_size);

	it = (struct inode_table*)bytep;

	zfsi = it->table[inode_offset];
	
	bytep = filesys + (zfsi.i_block[0] * s_model.block_size);

	tmp_zde = (struct zfs_dir_entry*)bytep;

	for(i = 0; i < (s_model.block_size / sizeof(struct zfs_dir_entry)) ; i++) {
		if(tmp_zde[i].name_len == 0) {
			i--;
			break;
		}
		zde[i] = tmp_zde[i];	
	}

	return i+1;
}

uint32_t zfs_create(uint8_t type)
{
	uint8_t *bytep = (uint8_t*)filesys;
	uint32_t inode = -1;
	struct group_desc *gd;
	struct inode_table *it;
	int i;
	int inode_offset = 0;
	int block_offset = 0;

	for(i = 1; i < GROUP_NUM; i++) {
		gd = (struct group_desc*)(bytep + sizeof(struct super_block));

		if(gd->free_inodes_count && gd->free_block_count) {
			struct inode_bitmap *im;
			struct block_bitmap *bm;
			im = (struct inode_bitmap*)(filesys + 
					s_model.block_size * gd->bg_inode_bitmap);

			for(inode_offset = 0; inode_offset < 1024 ; inode_offset++) {
				if(im->map[inode_offset] == 0) {
					im->map[inode_offset] = 1;
					gd->free_inodes_count--;
					break;
				}
			}

			
			if(type == 0) {	
				bm = (struct block_bitmap*)(filesys +
							s_model.block_size * gd->bg_block_bitmap);
				for(; block_offset < 1024; block_offset++) {
					if(bm->map[block_offset] == 0) {
						bm->map[block_offset] = 1;
						gd->free_block_count--;
						break;
					}
				}
			}
			

			break;
		}

		bytep += s_model.block_size * s_model.blocks_per_group;
	}



	if(i == GROUP_NUM)
		return inode;

	inode = gd->group_nr * s_model.inodes_per_group + inode_offset;
	block_offset += gd->group_nr * s_model.blocks_per_group;

	it = (struct inode_table*)(filesys + 
			gd->bg_inode_table * s_model.block_size);


	if(type == 0) {	
		it->table[inode_offset].i_size = 1*K;
		it->table[inode_offset].i_blocks = 1;
		it->table[inode_offset].i_block[0] = block_offset;
	} else {
		it->table[inode_offset].i_size = 0;
		it->table[inode_offset].i_blocks = 0;
		it->table[inode_offset].i_block[0] = 0;
	}

	return inode;
}

int zfs_write_dir(uint32_t dir_inode, uint32_t inode, char *name, uint8_t type)
{

	int g_nr, inode_offset;
	uint32_t i = 0;
	uint8_t *bytep;
	struct group_desc *gd;
	struct zfs_inode zfsi;
	struct inode_table *it;
	struct zfs_dir_entry *tmp_zde;

	if(strlen(name) > 64)
		return -1;

	g_nr = dir_inode / s_model.inodes_per_group;

	inode_offset = dir_inode % s_model.inodes_per_group;

	bytep = filesys + (g_nr * s_model.blocks_per_group * 
			s_model.block_size);

	bytep += sizeof(s_model);	

	gd = (struct group_desc*)bytep;

	bytep = filesys + (gd->bg_inode_table * s_model.block_size);

	it = (struct inode_table*)bytep;

	zfsi = it->table[inode_offset];
	
	bytep = filesys + (zfsi.i_block[0] * s_model.block_size);

	tmp_zde = (struct zfs_dir_entry*)bytep;

	for(i = 0; i < (s_model.block_size / sizeof(struct zfs_dir_entry)) ; i++) {
		if(tmp_zde[i].name_len == 0) {
			tmp_zde[i].inodes = inode;
			tmp_zde[i].type = type;
			tmp_zde[i].name_len = strlen(name);
			strcpy(tmp_zde[i].name, name);
			break;
		}
		if(tmp_zde[i].inodes == inode) {
			tmp_zde[i].name_len = strlen(name);
			strcpy(tmp_zde[i].name, name);
			break;
		}
	}

	return sizeof(struct zfs_dir_entry);

}

int set_b_bitmap(uint32_t block_num)
{
	int gnr = block_num / s_model.blocks_per_group;
	int block_offset = block_num % s_model.blocks_per_group;

	struct group_desc *gd;
	struct block_bitmap *bm;
	uint8_t *bytep = (uint8_t*)filesys;

	bytep = bytep + gnr * s_model.block_size * s_model.blocks_per_group;
	gd = (struct group_desc*)(bytep + sizeof(s_model));

	bytep = (uint8_t*)filesys;
	bm = (struct block_bitmap*)(bytep + gd->bg_block_bitmap * 
			s_model.block_size);

	bm->map[block_offset] = 0;

	return 0;
}

int set_i_bitmap(uint32_t inode_num)
{
	int gnr = inode_num / s_model.inodes_per_group;
	int inode_offset = inode_num % s_model.inodes_per_group;
	
	struct group_desc *gd;
	struct inode_bitmap *im;
	uint8_t *bytep = (uint8_t*)filesys;

	bytep = bytep + gnr * s_model.block_size * s_model.blocks_per_group;
	gd = (struct group_desc*)(bytep + sizeof(s_model));

	bytep = (uint8_t*)filesys;
	im = (struct inode_bitmap*)(bytep + gd->bg_inode_bitmap *
			s_model.block_size);

	im->map[inode_offset] = 0;

	return 0;
}

int zfs_write(uint32_t inodes, char *buf, const uint32_t size)
{
	int g_nr, inode_offset;
	int block_offset;
	uint32_t i = 0;
	uint8_t *bytep;
	struct group_desc *gd;
	struct zfs_inode *zfsi;
	struct inode_table *it;
	uint8_t *ptr;

	g_nr = inodes / s_model.inodes_per_group;
	inode_offset = inodes % s_model.inodes_per_group;

	bytep = filesys + (g_nr * s_model.blocks_per_group * 
			s_model.block_size);

	bytep += sizeof(s_model);	

	gd = (struct group_desc*)bytep;

	bytep = filesys + (gd->bg_inode_table * s_model.block_size);

	it = (struct inode_table*)bytep;

	zfsi = &(it->table[inode_offset]);

	int j = 0, k = 0;

	if(size <= zfsi->i_size) {
		for(i = 0; i < size ; i++) {
			if(i % s_model.block_size == 0) {
				bytep = filesys + (zfsi->i_block[j++] * s_model.block_size);
				k = 0;
			}
			bytep[k++] = buf[i];
		}
	} else {
		for(; (uint32_t)j < zfsi->i_blocks ;) {
			ptr = filesys + (zfsi->i_block[j++] * s_model.block_size);
			if(size - i > s_model.block_size) {
				memcpy(ptr, &buf[i], s_model.block_size);
				i += s_model.block_size;
			} else {
				memcpy(ptr, &buf[i], size - i);
				i = size;

				zfsi->i_size = size;
				zfsi->i_blocks = size / s_model.block_size + 1;
				return size;
			}
		}
	}

	if((uint32_t)j < zfsi->i_blocks) {
		while((uint32_t)j != zfsi->i_blocks) {
			set_b_bitmap(zfsi->i_block[j++]);
		}

		zfsi->i_size = size;
		zfsi->i_blocks = size / s_model.block_size + 1;
		return size;
	}

	bytep = (uint8_t*)filesys;
	for(k = 1; k < GROUP_NUM; k++) {
		gd = (struct group_desc*)(bytep + sizeof(struct super_block));
		if(gd->free_block_count > 0) {
			struct block_bitmap *bm;
			int block_num;

			bm = (struct block_bitmap*)(filesys +
					s_model.block_size * gd->bg_block_bitmap);
			for(block_offset = 0; block_offset < 1024; block_offset++) {
				if(bm->map[block_offset] == 0) {
					bm->map[block_offset] = 1;
					gd->free_block_count--;
					block_num = gd->group_nr * s_model.blocks_per_group + block_offset;
					ptr = filesys + block_num * s_model.block_size;

					if(size - i > s_model.block_size) {
						memcpy(ptr, &buf[i], s_model.block_size);
						i += s_model.block_size;
						zfsi->i_block[j++] = block_num;
					} else {
						//printf("%u %d %u\n", zfsi.i_size, i, block_num);
						memcpy(ptr, &buf[i], size - i);
						i = size;
						zfsi->i_block[j++] = block_num;

						zfsi->i_size = size;
						//printf("%u\n", zfsi.i_size);
						zfsi->i_blocks = size / s_model.block_size + 1;
						return size;
					}
				}
			}
		}
	}

	zfsi->i_size = size;
	zfsi->i_blocks = size / s_model.block_size + 1;
	return size;

}

int zfs_delete(uint32_t inode)
{
	return 0;
}
