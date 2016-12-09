#include<apue.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<stdio.h>

typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef struct node
{
	long w;
	short p,l,r;
}htnode,*htnp;

typedef struct huffman_code
{
	uchar len;
	uchar *codestr;
}hufcode;

#define OK       1  
#define ERROR   -1  
#define UNUSE   -1  
#define ARGS_ERR    -2//参数错误  
#define FILE_ERR    -3//文件错误  
#define HEAD_ERR    -4//头标错误  
#define MALLOC_ERR  -5//内存分配错误  
#define HUFTREE_ERR -6//霍夫曼树错误  
#define HUFCODE_ERR -7//霍夫曼编码错误  
#define CHAR_BITS   8//一个字符中的位数  
#define INT_BITS    32//一个整型中的位数  
#define CODE_SIZE   256//霍夫曼编码个数  
#define CACHE_SIZE  256//I/O缓存大小  
#define UINT_SIZE   sizeof(UINT)  
#define UCHAR_SIZE  sizeof(UCHAR)  
#define USHORT_SIZE sizeof(USHORT)  
#define ZIP_HEAD    0xFFFFFFFF//压缩文件头标  
#define MAX_NAME_LEN    512  

uchar char_to_bits(const uchar chars[8]);
int search_set(htnp ht,int n);
int create_huffmantree(long w[],int n,htnode ht[]);
int encode_huffmantree(htnp htp,int n,hufcode hc[]);
long calc_data_frequency(int fd,long frequency[]);
int compress(char *source,char *obj);
int c_init_file(char *source,int fd1,char obj,int fd2);
int write_compress_file(int fd1,int fd2,htnp ht,hufcode hc[],char *sourece,long size);






