#include<apue.h>
#include<stdlib.h>

/* have head mine head not have value */

struct Node{
	int data;
	struct Node *next;
};

typedef struct Node LNode;
typedef struct Node *LinkedList;

void init1(LinkedList l)
{
	l=NULL;
}

// have head
void init2(LinkedList l)
{
	l=(LNode *)malloc(sizeof(LNode));
	if(l==NULL)
		err_sys("link init error");
	l->next = NULL;
}

// have head
int length(LinkedList l)
{
	LNode *p; // why not LinkedList p?
	p = l->next; // first node
	int j=0;
	while(p!=NULL){
		j++;
		p=p->next;
	}
	return j;
}

// have head
LinkedList getInode(LinkedList l,int i)
{
	LNode *p;
	p=l->next;
	int j=0;
	while((p!=NULL)&&(j<i)){
		j++;
		p = p->next;
	}
	return p;
}

// have head
LNode locateKey(LinkedList l,int key)
{
	LNode *p;
	p=l->next;
	while((p!=NULL)&&(p->data!=key)){
		p=p->next;
	}
	return *p;
}

void Inert1(LinkedList l,LinkedList p,int value)
{
	LNode *q;
	q=(LNode *)malloc(sizeof(LNode));
	q->data = value;

	if(p==l){ // 表头插入
		q->next=l;
		l=q;
	}else{ // 在中间插入
		LNode *pre = l;
		while((pre!=NULL)&&(pre->next!=p))
			pre=pre->next;

		// 插在p后面
		q->next = pre->next;
		pre->next = q;
	}
}

// have head
void Inert2(LinkedList l,LinkedList p,int value)
{
	LNode *q;
	q=(LNode*)malloc(sizeof(LNode));
	q->data = value;

	LNode *pre=l;
	while((pre!=NULL)&&(pre->next!=p))
		pre = pre->next;
	q->next = pre->next;
	pre->next = q;
}

void Del1(LinkedList l,LinkedList p,int value)
{
	LNode *pre = l;
	while((pre!=NULL)&&(pre->data!=value))
		pre = pre->next;
	p=pre->next;

	if(p!=NULL){
		if(p==l)
			l=p->next;
		else
			pre->next = p->next;
		free(p);
	}
}

// have head
void Del2(LinkedList l,LinkedList p,int value)
{
	LNode *pre = l;
	while((pre!=NULL)&&(pre->data!=value))
		pre = pre->next;
	p=pre->next;

	if(p!=NULL){
		pre->next = p->next;
		free(p);
	}
}

// have haed
void CreatHead(LinkedList l,int *value,int len)
{
	int i=0;
	l = (LNode*)malloc(sizeof(LNode));
	l->next = NULL;

	for(i=0;i<len;i++){
		LNode *p=(LNode*)malloc(sizeof(LNode));
		p->data = value[i];
		p->next=l->next;
		l->next = p;
	}
}

// have head
void CreateTail(LinkedList l,int *value,int len)
{
	int i=0;
	l = (LNode*)malloc(sizeof(LNode));
	l->next = NULL;
	LNode *tail=l;

	for(i=0;i<len;i++){
		LNode *p=(LNode*)malloc(sizeof(LNode));
		p->data = value[i];
		p->next = NULL;
		tail->next = p;
		tail = p;
	}
}

// have head
void Merger(LinkedList la,LinkedList lb,LinkedList lc)
{
	LNode *pa,*pb,*tc;
	lc = (LNode*)malloc(sizeof(LNode));
	lc->next = NULL;
	tc = lc;
	pa = la->next;
	pb = lb->next;
	while((pa!=NULL)&&(pb!=NULL)){
		if(pa->data <= pb->data){
			tc->next = pa;
			tc = pa;
			pa = pa->next;
		}else{
			tc->next = pb;
			tc = pb;
			pb = pb->next;
		}
	}
	if(pa==NULL)
		tc->next = pb;
	else
		tc->next = pa;
	free(la);
	free(lb);
}
