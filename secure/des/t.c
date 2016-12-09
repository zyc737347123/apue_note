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
int DES_SBOX2(ElemType data[48]);  
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
	int i=0;
	int t=1;
	char key[8],palin[8];
	for(i=0;i<8;i++){
		key[i]=(t<<4)|(t+1);
		palin[i]=key[i];
		t=t+2;
		//printf("%d ",(int)key[i]);
	}

	ElemType plainBlock[8],cipherBlock[8],keyBlock[8];
	ElemType bKey[64];
	ElemType subKeys[16][48];

	memcpy(keyBlock,key,8);
	Char8ToBit64(keyBlock,bKey);
	DES_MakeSubKeys(bKey,subKeys);

	printf("key\n");
	for(i=0;i<48;i++)
		printf("%d ",subKeys[0][i]);
	printf("\n");

	ElemType ttt[8];
	DES_EncryptBlock(palin,subKeys,ttt);


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
	printf("PC1\n");
	for(i=0;i<56;i++){
		tempbts[i]=key[PC_1[i]];
		printf("%d ",(int)tempbts[i]);
	}
	printf("\n");
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
	printf("S\n");
	for(i=0;i<8;i++){
		cur1 = i*6;
		cur2 = i*4;

		line = (data[cur1] << 1) + data[cur1+5];
		row = (data[cur1+1] << 3) + (data[cur1+2] << 2) 
			+ (data[cur1+3] << 1) + data[cur1+4];
		output = S[i][line][row];
		int j=0;
		for(j=0;j<6;j++){
			printf("%d ",data[j+cur1]);
		}
		printf("\n");
		printf("line: %d row: %d \n",(data[cur1] << 1)+(data[cur1+5]),(data[cur1+1] << 3)+(data[cur1+2] << 2)+(data[cur1+3] << 1)+data[cur1+4]);
		printf("%d\n",output);
		data[cur2] = (output & 0x08) >> 3;
		data[cur2+1] = (output & 0x04) >> 2;
		data[cur2+2] = (output & 0x02) >> 1;
		data[cur2+3] = (output & 0x01);
	}

	return 0;
}

int DES_SBOX2(ElemType data[48])
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
		if(i==0){
			int j=0;
			printf("E[R0]\n");
			for(j=0;j<48;j++)
				printf("%d ",(int)copyRight[j]);
			printf("\n");
		}
		
		// 与子密钥进行异或
		DES_XOR(copyRight,subKeys[i],48);
		if(i==0){
			int j=0;
			printf("A\n");
			for(j=0;j<48;j++)
				printf("%d ",(int)copyRight[j]);
			printf("\n");
		}

		// 异或结果进入S盒，输出32位结果
		ElemType te[48];
		memcpy(te,copyRight,48);
		DES_SBOX2(copyRight);
		if(i==0){
			DES_SBOX(te);
			int j=0;
			printf("B\n");
			for(j=0;j<48;j++)
				printf("%d ",(int)copyRight[j]);
			printf("\n");
		}

		DES_P_Transform(copyRight);
		if(i==0){
			int j=0;
			printf("P(B)\n");
			for(j=0;j<32;j++)
				printf("%d ",(int)copyRight[j]);
			printf("\n");
		}

		// 将明文左半部分和经过处理的右半部分 进行异或
		DES_XOR(plainBits,copyRight,32);
		if(i==0){
			int j=0;
			printf("R1=P(B)^ L0\n");
			for(j=0;j<32;j++)
				printf("%d ",(int)plainBits[j]);
			printf("\n");
		}

		if(i != 15)
			DES_Swap(plainBits,plainBits+32);
	}

	DES_IP_1_Transform(plainBits);
	int j=0;
	printf("R1=P(B)^ L0\n");
	for(j=0;j<64;j++)
		printf("%d ",(int)plainBits[j]);
	printf("\n");
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


