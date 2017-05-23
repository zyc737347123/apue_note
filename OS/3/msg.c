#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#define MAX_SEND_SIZE 80

struct mymsgbuf{
	long mtype;
	char mtext[MAX_SEND_SIZE];
};

void send_msg(int qid,struct mymsgbuf *qbuf,long type,char *text);
void read_msg(int qid,struct mymsgbuf *qbuf,long type);
void remove_queue(int qid);
void change_queue_mode(int qid,char *mode);
void usage(void);

int main(int argc,char *argv[])
{
	key_t key;
	int msgqueue_id;
	struct mymsgbuf qbuf;

	if(argc == 1)
		usage();
	key = ftok(".",'m');

	if((msgqueue_id = msgget(key,IPC_CREAT|0660)) == -1){
		perror("msgget");
		exit(1);
	}

	switch(tolower(argv[1][0])){
		case 's': 
			send_msg(msgqueue_id,&qbuf,atol(argv[2]),argv[3]);
			break;
		case 'r':
			read_msg(msgqueue_id,&qbuf,atol(argv[2]));
			break;
		case 'd':
			remove_queue(msgqueue_id);
			break;
		case 'm':
			change_queue_mode(msgqueue_id,argv[2]);
			break;
		default:
			usage();
	}

	return 0;
}

void send_msg(int qid,struct mymsgbuf *qbuf,long type,char *text)
{
	printf("Send a msg\n");
	qbuf->mtype = type;
	strcpy(qbuf->mtext,text);

	if((msgsnd(qid,(struct msgbuf*)qbuf,strlen(qbuf->mtext)+1,0)) == -1){
		perror("msg");
		exit(1);
	}
}

void read_msg(int qid,struct mymsgbuf *qbuf,long type)
{
	printf("Reading a msg\n");
	qbuf->mtype = type;
	msgrcv(qid,(struct msgbuf*)qbuf,MAX_SEND_SIZE,type,0);
	printf("Type: %ld Text: %s\n",qbuf->mtype,qbuf->mtext);
	return;
}

void remove_queue(int qid)
{
	msgctl(qid,IPC_RMID,0);
	return;
}

void change_queue_mode(int qid,char *mode)
{
	struct msqid_ds myqueue_ds;

	msgctl(qid,IPC_STAT,&myqueue_ds);

	scanf(mode,"%ho",&myqueue_ds.msg_perm.mode);

	msgctl(qid,IPC_SET,&myqueue_ds);
	return;
}

void usage()
{
}
