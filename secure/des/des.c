#include<apue.h>
#include<stdio.h>
#include<memory.h>
#include<time.h>
#include<stdlib.h>
#include<fcntl.h>
#include "des.h"

int ByteToBit(ElemType ch,ElemType bit[8]);  
int BitToByte(ElemType bit[8],ElemType *ch);  
int Char8ToBit64(ElemType ch[8],ElemType bit[64]);  
int Bit64ToChar8(ElemType bit[64],ElemType ch[8]);  
int DES_MakeSubKeys(ElemType key[64],ElemType subKeys[16][48]);  
int DES_PC1_Transform(ElemType key[64], ElemType tempbts[56]);  
int DES_PC2_Transform(ElemType key[56], ElemType tempbts[48]);  
int DES_ROL(ElemType data[56], int time);  
int DES_IP_Transform(ElemType data[64]);  
int DES_IP_1_Transform(ElemType data[64]);  
int DES_E_Transform(ElemType data[48]);  
int DES_P_Transform(ElemType data[32]);  
int DES_SBOX(ElemType data[48]);  
int DES_XOR(ElemType R[48], ElemType L[48],int count);  
int DES_Swap(ElemType left[32],ElemType right[32]);  
int DES_EncryptBlock(ElemType plainBlock[8], ElemType subKeys[16][48], ElemType cipherBlock[8]);  
int DES_DecryptBlock(ElemType cipherBlock[8], ElemType subKeys[16][48], ElemType plainBlock[8]);  
int DES_Encrypt(char *plainFile, char *keyStr,char *cipherFile);  
int DES_Decrypt(char *cipherFile, char *keyStr,char *plainFile); 
int decode(int cipherFile,char *keyStr,int plainFile,off_t size);
int encode(int plainFile,char* keyStr,int cipherFile); 
void create_key(unsigned char *k,const int keylen);

void create_key(unsigned char *k,const int keylen)
{
	int judge[256],i=0;
	memset(judge,0,sizeof(int)*256);

	for(i=0;i<keylen;){
		int t=i;
		t=rand()%256;
		if(judge[t])
			continue;
		judge[t]=1;
		k[i]=t;
		i++;
	}
}


int main(int argc,char* argv[])
{
	char nn;
	if(argc<4)
		err_sys("usage: ./rc4 -e/-d descfile keyfile outfile");

	int fd1=-1,fd2=-1,fd3=-1; // fd1 is plainFile,fd2 is keyStr,fd3 is cipherFile.
	struct stat statbuf;

	if((fd1=open(argv[2],O_RDONLY))<0)
		err_sys("open %s fail",argv[1]);
	if(lstat(argv[2],&statbuf)<0)
		err_sys("stat %s fail",argv[1]);

	mode_t mode=statbuf.st_mode&00777;

	if(argc>4){
		if((fd3=open(argv[4],O_RDWR | O_EXCL | O_CREAT | O_TRUNC,mode))<0){
		printf("%s is exits,do you want to rewrite(y/n)\n",argv[4]);
		scanf("%c",&nn);
		if(nn =='y' || nn == 'Y')
			fd3=open(argv[4],O_RDWR | O_TRUNC);
		else{ 
			return 0;
		}
		}
	}else{
		if((fd3=open("out.txt",O_RDWR | O_CREAT,mode))<0)
			err_sys("crete out.txt fail");
	}
	

	int r=0;

	if(strcmp(argv[1],"-e")==0){
		if((fd2=open(argv[3],O_RDWR | O_EXCL | O_CREAT | O_TRUNC,mode))<0){
			printf("%s is exits,do you want to rewrite(y/n)\n",argv[3]);
			scanf("%c",&nn);
			scanf("%c",&nn);
			if(nn =='y' || nn == 'Y')
				fd2=open(argv[3],O_RDWR | O_TRUNC);
			else return 0;
		}
	}else{
		if((fd2=open(argv[3],O_RDONLY))<0)
			err_sys("open %s fail",argv[3]);
	}

	srand(time(NULL));

	int n=8;
	unsigned char *k=(unsigned char*)malloc(sizeof(char)*n);
	create_key(k,n);
	
	off_t lek,size=statbuf.st_size;

	if(strcmp(argv[1],"-e")==0){
			int w=0;
			if((w=write(fd2,k,8))!=8)
				err_sys("write fail,write %d byte",w);
			encode(fd1,(char *)k,fd3);
			
	}else{
		if((r=read(fd2,k,8))!=8)
			err_sys("read fail,read,read %d byte",r);

		decode(fd1,(char *)k,fd3,size);
			
	}
	close(fd2);
	/*
	while(lek<size){
		memset(m,0,sizeof(char)*1024);
		r=read(fd1,m,1024);
		encode(m,k,r,n);
		write(fd3,m,r);
		lek=lseek(fd1,0,SEEK_CUR);
	}*/
	close(fd1);
	close(fd3);
	exit(0);
}

int ByteToBit(ElemType ch,ElemType bit[8])
{
	int i=0;
	for(i=0;i<8;i++)
		bit[i]=(ch>>i)&1;
	return 0;
}

int BitToByte(ElemType bit[8],ElemType *ch)
{
	int i=0;
	for(i=0;i<8;i++)
		*ch |= bit[i]<<i;

	return 0;
}

int Char8ToBit64(ElemType ch[8],ElemType bit[64])
{
	int i=0;
	for(i=0;i<8;i++)
		ByteToBit(ch[i],bit+(i<<3));

	return 0;
}

int Bit64ToChar8(ElemType bit[64],ElemType ch[8])
{
	int i=0;
	memset(ch,0,8);
	for(i=0;i<8;i++)
		BitToByte(bit+(i<<3),ch+i);

	return 0;
}

// 生成子密钥 subkeys
int DES_MakeSubKeys(ElemType key[64],ElemType subKeys[16][48])
{
	ElemType temp[56];
	int i=0;
	DES_PC1_Transform(key,temp); // PC1置换
	for(i=0;i<16;i++){ // 16 轮迭代 产生16个子密钥
		DES_ROL(temp,MOVE_TIMES[i]); // 循环左移
		DES_PC2_Transform(temp,subKeys[i]); // PC2置换，产生子密钥
	}

	return 0;
}
		
// 密钥置换 PC1
int DES_PC1_Transform(ElemType key[64],ElemType tempbts[56])
{
	int i=0;
	for(i=0;i<56;i++)
		tempbts[i]=key[PC_1[i]];

	return 0;
}

// PC2
int DES_PC2_Transform(ElemType key[56],ElemType tempbts[48])
{
	int i=0;
	for(i=0;i<48;i++)
		tempbts[i]=key[PC_2[i]];

	return 0;
}

// 循环左移
int DES_ROL(ElemType data[56],int time)
{
	ElemType temp[56];

	//保存将要循环移动到右边的位
	memcpy(temp,data,time);
	memcpy(temp+time,data+28,time); // 分成两部分，temp只保存 2 * time 个位的数据

	//前28位移动，一个闭环的左移
	memcpy(data,data+time,28-time); // 前 time 位的数据被覆盖
	memcpy(data+28-time,temp,time); // 前 time 位的数据变为最后 time 位

	//后28位的移动
	memcpy(data+28,data+28+time,28-time);
	memcpy(data+56-time,temp+time,time);

	return 0;
}

// IP置换
int DES_IP_Transform(ElemType data[64])
{
	int i=0;
	ElemType temp[64];
	for(i=0;i<64;i++)
		temp[i] = data[IP_Table[i]];

	memcpy(data,temp,64);

	return 0;
}

// IP 逆转换
int DES_IP_1_Transform(ElemType data[64])
{
	int i=0;
	ElemType temp[64];

	for(i=0;i<64;i++)
		temp[i] = data[IP_1_Table[i]];

	memcpy(data,temp,64);
	return 0;
}

// 扩展置换
int DES_E_Transform(ElemType data[48])
{
	int i=0;
	ElemType temp[48];
	for(i=0;i<48;i++)
		temp[i] = data[E_Table[i]];

	memcpy(data,temp,48);
	return 0;
}

// P扩展
int DES_P_Transform(ElemType data[32])
{
	int i=0;
	ElemType temp[32];
	for(i=0;i<32;i++)
		temp[i]=data[P_Table[i]];

	memcpy(data,temp,32);
	return 0;
}

// 异或
int DES_XOR(ElemType R[48],ElemType L[48],int count)
{
	int i=0;
	for(i=0;i<count;i++)
		R[i] ^= L[i];

	return 0;
}

// S盒置换
int DES_SBOX(ElemType data[48])
{
	int i=0;
	int line,row,output;
	int cur1,cur2;

	for(i=0;i<8;i++){
		cur1 = i*6;
		cur2 = i*4;

		line = (data[cur1] << 1) + data[cur1+5];
		row = (data[cur1+1] << 3) + (data[cur1+2] << 2) 
			+ (data[cur1+3] << 1) + data[cur1+4];
		output = S[i][line][row];

		data[cur2] = (output & 0x08) >> 3;
		data[cur2+1] = (output & 0x04) >> 2;
		data[cur2+2] = (output & 0x02) >> 1;
		data[cur2+3] = (output & 0x01);
	}

	return 0;
}

// 交换
int DES_Swap(ElemType left[32],ElemType right[32])
{
	ElemType temp[32];
	memcpy(temp,left,32);
	memcpy(left,right,32);
	memcpy(right,temp,32);
	return 0;
}

// 加密单个分组
int DES_EncryptBlock(ElemType plainBlock[8],ElemType subKeys[16][48],ElemType cipherBlock[8])
{
	ElemType plainBits[64];
	ElemType copyRight[48];

	int i=0;

	Char8ToBit64(plainBlock,plainBits);

	DES_IP_Transform(plainBits);

	for(i=0;i<16;i++){
		memcpy(copyRight,plainBits+32,32);
	
		// 将右半部分进行扩展置换,从32位到48位
		DES_E_Transform(copyRight);
		
		// 与子密钥进行异或
		DES_XOR(copyRight,subKeys[i],48);

		// 异或结果进入S盒，输出32位结果
		DES_SBOX(copyRight);

		DES_P_Transform(copyRight);

		// 将明文左半部分和经过处理的右半部分 进行异或
		DES_XOR(plainBits,copyRight,32);

		if(i != 15)
			DES_Swap(plainBits,plainBits+32);
	}

	DES_IP_1_Transform(plainBits);
	Bit64ToChar8(plainBits,cipherBlock);

	return 0;
}

//解密单个分组  
int DES_DecryptBlock(ElemType cipherBlock[8], ElemType subKeys[16][48],ElemType plainBlock[8]){  
    ElemType cipherBits[64];  
    ElemType copyRight[48];  
    int i;  
  
    Char8ToBit64(cipherBlock,cipherBits);         
    //初始置换（IP置换）  
    DES_IP_Transform(cipherBits);  
      
    //16轮迭代  
    for(i = 15; i >= 0; i--){        
        memcpy(copyRight,cipherBits+32,32);  
        //将右半部分进行扩展置换，从32位扩展到48位  
        DES_E_Transform(copyRight);  
        //将右半部分与子密钥进行异或操作  
        DES_XOR(copyRight,subKeys[i],48);       
        //异或结果进入S盒，输出32位结果  
        DES_SBOX(copyRight);  
        //P置换  
        DES_P_Transform(copyRight);       
        //将明文左半部分与右半部分进行异或  
        DES_XOR(cipherBits,copyRight,32);  
        if(i != 0){  
            //最终完成左右部的交换  
            DES_Swap(cipherBits,cipherBits+32);  
        }  
    }  
    //逆初始置换（IP^1置换）  
    DES_IP_1_Transform(cipherBits);  
    Bit64ToChar8(cipherBits,plainBlock);  
    return 0;  
}  

int encode(int plainFile,char* keyStr,int cipherFile)
{
	ElemType plainBlock[8],cipherBlock[8],keyBlock[8];
	ElemType bKey[64];
	ElemType subKeys[16][48];

	memcpy(keyBlock,keyStr,8);
	Char8ToBit64(keyBlock,bKey);
	DES_MakeSubKeys(bKey,subKeys);

	int r=1,w=0;
	time_t s1,e1,summ=0;

	while(1){
		if((r=read(plainFile,plainBlock,sizeof(ElemType)*8)) == 8){
			s1=clock();
			DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
			e1=clock();
			summ += e1-s1;
			if((w=write(cipherFile,cipherBlock,sizeof(ElemType)*8)) != 8)
				err_sys("write error,write %d byte",w);
		}else{
			break;
		}
	}

	if(r){
		memset(plainBlock + r,0,7-r);
		plainBlock[7]=8-r;
		s1=clock();
		DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
		e1=clock();
		summ += e1 - s1;
		write(cipherFile,cipherBlock,sizeof(ElemType)*8);
	}

	printf("time is %ld ms",summ/1000);
	return 0;
}

int decode(int cipherFile,char *keyStr,int plainFile,off_t size)
{
	time_t s1,e1,summ=0;
	int times =0;
	ElemType plainBlock[8],cipherBlock[8],keyBlock[8];
	ElemType bKey[64];
	ElemType subKeys[16][48];

	memcpy(keyBlock,keyStr,8);
	Char8ToBit64(keyBlock,bKey);
	DES_MakeSubKeys(bKey,subKeys);

	while(1){
		read(cipherFile,cipherBlock,sizeof(ElemType)*8);
		s1=clock();
		DES_DecryptBlock(cipherBlock,subKeys,plainBlock);
		e1=clock();
		times += 8;
		if(times < size){
			write(plainFile,plainBlock,sizeof(ElemType)*8);
		}else{
			break;
		}
	}

	int count;
	count=0;
	if(plainBlock[7] < 8){
		for(count = 8-plainBlock[7];count < 7;count++)
			if(plainBlock[count] != 0)
				break;
	}

	if(count == 7)
		write(plainFile,plainBlock,sizeof(ElemType)*(8-plainBlock[7]));
	else
		write(plainFile,plainBlock,sizeof(ElemType)*8);

	return 0;
}


