#ifndef ZFS_H_
#define ZFS_H_

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;



struct super_block {
	uint8_t vaild;
	uint32_t inodes_count;
	uint32_t block_count;
	uint32_t block_size;
	uint32_t blocks_per_group;
	uint32_t inodes_per_group;
	uint16_t inode_size;
	uint16_t inode_table_size;
};

struct group_desc {
	uint8_t group_nr;
	uint32_t bg_block_bitmap;
	uint32_t bg_inode_bitmap;
	uint32_t bg_inode_table;
	uint16_t free_block_count;
	uint16_t free_inodes_count;
};

struct block_bitmap {
	uint8_t map[1024];
};

struct inode_bitmap {
	uint8_t map[1024];
};

struct zfs_inode {
	uint32_t i_size;
	uint32_t i_blocks;
	uint32_t i_block[14];
};

struct inode_table {
	struct zfs_inode table[1024];
};

struct zfs_dir_entry {
	uint32_t inodes;
	//uint16_t rec_len;
	uint8_t type;
	uint16_t name_len;
	char name[64];
};

void zfs_init(void *fsmmap);
#endif
