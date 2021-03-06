/**************************mydd.c************************
 * mydd.c is used to read regular file and directory file
 * the first parameter has two mode : -r / -d
 * usage: exe_name -r filepath out_file_name
 * usage: exe_name -d dir_file_name , this mode will printf to terminal
 * email: zyc737347123@163.com
 **************************Zyc***************************/

#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<stdarg.h>

struct dir_item{
	unsigned int inode;
	unsigned short rec_len;
	unsigned char name_len;
	unsigned char file_type;
	char name[256];
};

void readfile(const char* filename,const char* outf);
unsigned int rdata(unsigned char* buf,int type);
int read1(const int in,const int out,off_t offset,off_t filesize);	// for file size <= 12KB
int read2(const int in,const int out,off_t offset,off_t filesize);	// for file size <= 268KB and >12KB
int readDir(const int in,off_t offset,off_t filesize);
void d_printf(struct dir_item item);

#define K 1024													  	//this K is 1Kbyte
#define INODE_SIZE_ADDR 0x458										//start address of inode_size
#define INODE_PER_GROUP_ADDR 0x428									//start address of inode_per_group
#define GROUP_DESC_ADDR 0x800										//start address of group descriptors

void err_sys(const char *fmt,...)
{
	va_list argv;
	va_start(argv,fmt);
	vprintf(fmt,argv);
	printf("\n");
	exit(0);
}


int main(int argc,char *argv[])
{
	if(argc<3){
		printf("useag: filename outfilename\n");
		exit(0);
	}
	
	char *type=argv[1];
	char *filename=argv[2];
	char *outfile=NULL;
	if(!strcmp(type,"-r"))	// -r mine file is regular file
		outfile=argv[3];
	if(!strcmp(type,"-d"))	// -d mine file is dir
		outfile="/dev/null";

	readfile(filename,outfile);

	exit(0);
}

/* because ext2 is little-endian,so we use this fuction
 * to read data from ext2
 */
unsigned int rdata(unsigned char *buf,int type)
{
	unsigned int result=-1;
	if(type==2){
		result=(buf[1]<<8)+buf[0];
	}
	if(type==4){
		result=(buf[3]<<24)+(buf[2]<<16)+(buf[1]<<8)+buf[0];
	}
	return result;
}

unsigned int inode_size=0;
unsigned int inode_per_group=0;
unsigned int file_group=0;
unsigned int iblock=0;

void readfile(const char* filename,const char* outf)
{
	struct stat statbuf;
	off_t filesize=0;
	off_t offset=0;
	int ino=0,in=-1,out=-1;
	unsigned int inode_table_start=0,inode_add=0; 
	unsigned char super_and_group[3*K];
	memset(super_and_group,0,sizeof(super_and_group));

	if((stat(filename,&statbuf))!=0){
			err_sys("stat %s error",filename);
	}
	filesize=statbuf.st_size;
	ino=statbuf.st_ino;
	
	if((in=open("/dev/loop0",O_RDONLY))<0)
		err_sys("open %s error",filename);
	if((out=open(outf,O_APPEND | O_RDWR | O_TRUNC))<0)
		err_sys("open %s error",outf);

	//get indoe_size and inode_per_group from SuperBlock
	if((read(in,super_and_group,3*K))<3*K)
		err_sys("read %s error",filename);
	inode_size = rdata(&super_and_group[INODE_SIZE_ADDR],2);
	inode_per_group = rdata(&super_and_group[INODE_PER_GROUP_ADDR],4);

	//get file_group and inode_table_start form group desc
	file_group = ino / inode_per_group;
	int add = GROUP_DESC_ADDR + file_group*0x20 + 8;
	inode_table_start = rdata(&super_and_group[add],4);

	//compute the inode_address
	inode_add = inode_table_start*K + ((ino-1) % inode_per_group)*inode_size;
	iblock = inode_add + 0x28;
	offset = iblock;
	
	//printf("%u,%u,%u,%u,%x\n",inode_size,inode_per_group,ino,file_group,inode_add);
	
	//check file type
	if(S_ISREG(statbuf.st_mode)){
		// this file is regular file
		unsigned int res=0;
		if((res=read1(in,out,offset,filesize))==0)
			err_sys("read1 error");

		//printf("%u\n",res);

		if(filesize > 12*K){
			res=read2(in,out,offset+(4*12),filesize);
			//printf("%u\n",res);
		}
	}else if(S_ISDIR(statbuf.st_mode)){
		readDir(in,offset,filesize);
	}

	close(in);
	close(out);
}

int read1(const int in,const int out,off_t offset,off_t filesize)
{
	int i=0,j=0;
	unsigned int i_b[12];
	unsigned char buf[1024];
	memset(buf,0,1024);
	if(lseek(in,offset,SEEK_SET)!=offset)
		err_sys("lseek error");
	if(read(in,buf,4*12)!=4*12)
		err_sys("read error");
	
	for(i=0;i<12;i++){
		i_b[i]=rdata(&buf[j],4);
		j += 4;
	}
	
	if(filesize >= 12*K)
		j=12;
	else
		j=filesize / K;

	i=0;
	while(i<j){
		lseek(in,i_b[i]*K,SEEK_SET);
		read(in,buf,K);
		write(out,buf,K);
		i++;
	}

	if(i_b[i] && j<12){
		lseek(in,i_b[i]*K,SEEK_SET);
		read(in,buf,filesize-j*K);
		write(out,buf,filesize-j*K);
	}

	return lseek(out,0,SEEK_CUR);
}

int read2(const int in,const int out,off_t offset,off_t filesize)
{
	int i=0,j=0;
	unsigned int i_b_12=0;
	unsigned int i_b_two[256];
	unsigned char buf[1024];
	memset(buf,0,1024);
	if(lseek(in,offset,SEEK_SET)!=offset)
		err_sys("lseek2 error");
	if(read(in,buf,4)!=4)
		err_sys("read2 error");
	
	i_b_12 = rdata(&buf[0],4);
	lseek(in,i_b_12*K,SEEK_SET);
	read(in,buf,K);
	for(i=0;i<256;i++){
		i_b_two[i]=rdata(&buf[j],4);
		j += 4;
	}
	
	if(filesize-12*K > 256*K){
		return 0;
	}else if(filesize - 12*K == 256*K){
		j=256;
	}else{
		j = (filesize - 12*K) / K;
	}

	i=0;
	while(i<j){
		lseek(in,i_b_two[i]*K,SEEK_SET);
		read(in,buf,K);
		write(out,buf,K);
		i++;
	}

	if(i_b_two[i] && j<256){
		lseek(in,i_b_two[i]*K,SEEK_SET);
		read(in,buf,filesize-(12+j)*K);
		write(in,buf,filesize-(12+j)*K);
	}

	return lseek(out,0,SEEK_CUR);
}

int readDir(const int in,off_t offset,off_t filesize)
{
	int i=0,j=0,seek=0;
	unsigned int i_b[12];
	unsigned char *buf = (unsigned char*)malloc(filesize);
	memset(buf,0,filesize);

	if(lseek(in,offset,SEEK_SET)!=offset)
		err_sys("lseek error");
	if(read(in,buf,4*12)!=4*12)
		err_sys("read error");

	for(i=0;i<12;i++){
		i_b[i]=rdata(&buf[j],4);
		j += 4;
	}

	j = filesize / K;

	memset(buf,0,filesize);
	for(i=0;i<j;i++){
		lseek(in,i_b[i]*K,SEEK_SET);
		read(in,&buf[seek],1024);
		seek += 1024;
	}

	seek=0;
	while(seek<filesize){
		struct dir_item item;
		if((item.inode=rdata(&buf[seek],4))==0)
			break;
		seek += 4;

		item.rec_len = rdata(&buf[seek],2);
		seek += 2;

		item.name_len = buf[seek];
		seek++;

		item.file_type = buf[seek];
		seek++;
		
		memcpy(item.name,&buf[seek],item.name_len);
		item.name[item.name_len]='\0';

		seek += item.rec_len - 8;

		d_printf(item);
	}

	free(buf);
	return 1;
}

void d_printf(struct dir_item item)
{
	char *type;
	switch(item.file_type){
		case 0:
			type = "unknow";
			break;
		case 1:
			type = "regular";
			break;
		case 2:
			type = "dir";
			break;
		case 3:
			type = "char";
			break;
		case 4:
			type = "block";
			break;
		case 5:
			type = "FIFO";
			break;
		case 6:
			type = "SOCK";
			break;
		case 7:
			type = "symlink";
	}
	printf("%u\t\t%s\t\t%s\n",item.inode,type,item.name);
}
