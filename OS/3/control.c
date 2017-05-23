#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define getpch(type) (type*)malloc(sizeof(type));
struct pcb{
	int id;
	char name[10];
	int time_start;
	int time_need;
	int time_left;
	int time_used;
	char state;
};

typedef struct pcb PCB;

void _sleep(int n)
{
	clock_t goal;
	goal = (clock_t)n * CLOCKS_PER_SEC + clock();
	while(goal > clock());
}

char _keygo()
{
	char c;
	printf("type something\n");
	c=getchar();
	return c;
}

int time_unit = 2;
#define maxnum 10
int num = 4;
static int order[maxnum];
static int ready[maxnum];

/*
PCB pcbdata[maxnum] = {
	{1000,"A",0,4,4,0,'R'},
	{1001,"B",1,3,3,0,'R'},
	{1002,"C",2,5,5,0,'R'},
	{1003,"D",3,2,2,0,'R'},
	{1004,"E",4,4,4,0,'R'},
};
*/

/*
PCB pcbdata[maxnum] = {
	{1000,"A",1,9,9,0,'R'},
	{1001,"B",1,16,16,0,'R'},
	{1002,"C",1,3,3,0,'R'},
	{1003,"D",1,11,11,0,'R'},
};
*/
/*
PCB pcbdata[maxnum] = {
	{1000,"A",10,8,8,0,'R'},
	{1001,"B",12,12,12,0,'R'},
	{1002,"C",14,4,4,0,'R'},
	{1003,"D",16,6,6,0,'R'},
};
*/


PCB pcbdata[maxnum] = {
	{1000,"A",0,7,7,0,'R'},
	{1001,"B",5,4,4,0,'R'},
	{1002,"C",7,13,13,0,'R'},
	{1003,"D",12,9,9,0,'R'},
};

void input()
{
	int i;
	printf("进程总数:");
	scanf("%d",&num);
	for(i=0;i<num;i++){
		pcbdata[i].id = 1000 + i;
		scanf("%s%d%d",pcbdata[i].name,&pcbdata[i].time_start,
				&pcbdata[i].time_need);
		pcbdata[i].time_left = pcbdata[i].time_need;
		pcbdata[i].time_used = 0;
		pcbdata[i].state = 'R';
	}
}

void FCFS()
{
	int i,j,tmp;
	double k;
	for(i=0;i<num;i++){
		order[i] = pcbdata[i].time_start;
		ready[i] = i;
	}
	for(i=0;i<num;i++)
		for(j=i+1;j<num;j++){
			if(order[i] > order[j]){
				tmp = order[i];
				order[i] = order[j];
				order[j] = tmp;
				tmp = ready[i];
				ready[i] = ready[j];
				ready[j] = tmp;
			}
		}
	printf("---先来先到：非抢占，无时间片---\n");
	tmp = pcbdata[ready[0]].time_start;
	for(i=0;i<num;i++){
		printf("第%d个进程--%s, ",i+1,pcbdata[ready[i]].name);
		printf("到达时间 -- %d, 服务时间 -- %d\n",
				pcbdata[ready[i]].time_start,
				pcbdata[ready[i]].time_need);
		printf("本进程正在运行..............");
		_sleep(1);
		printf("运行完毕\n");
		tmp += pcbdata[ready[i]].time_need;
		j = tmp - pcbdata[ready[i]].time_start;
		k = (float)j / pcbdata[ready[i]].time_start;
		printf("完成时间 -- %d, 周转时间 -- %d, "
				"带权周转时间 -- %.1f\n\n",tmp,j,k);
		
	}
	printf("----------所有进程调度完毕--------\n");
}

void SJF()
{
	int i,j,tmp=1000,min=1000,idx;
	double k;
	int have_ready[maxnum];
	for(i=0;i<maxnum;i++)
		have_ready[i]=0;
	for(i=0;i<num;i++){
		tmp = tmp < pcbdata[i].time_start ? tmp : pcbdata[i].time_start;
	}

	for(i=0;i<num;i++){
		min = 1000;

		for(j=0;j<num;j++){
			if(!have_ready[j] && pcbdata[j].time_start <= tmp){
				if(min>pcbdata[j].time_need){
					min = pcbdata[j].time_need;
					idx = j;
				}
			}
		}
		
		ready[i]=idx;
		have_ready[idx]=1;
		tmp += pcbdata[idx].time_need;

	}

	tmp = pcbdata[ready[0]].time_start;
	for(i=0;i<num;i++){
		printf("第%d个进程--%s, ",i+1,pcbdata[ready[i]].name);
		printf("到达时间 -- %d, 服务时间 -- %d\n",
				pcbdata[ready[i]].time_start,
				pcbdata[ready[i]].time_need);
		printf("本进程正在运行..............");
		_sleep(1);
		printf("运行完毕\n");
		tmp += pcbdata[ready[i]].time_need;
		j = tmp - pcbdata[ready[i]].time_start;
		k = (float)j / pcbdata[ready[i]].time_start;
		printf("完成时间 -- %d, 周转时间 -- %d, "
				"带权周转时间 -- %.1f\n\n",tmp,j,k);
		
	}
	printf("----------所有进程调度完毕--------\n");

}

void HRF()
{
	int i,j,tmp=1000,idx;
	double k,h;
	int have_ready[maxnum];
	for(i=0;i<maxnum;i++)
		have_ready[i]=0;
	for(i=0;i<num;i++){
		tmp = tmp < pcbdata[i].time_start ? tmp : pcbdata[i].time_start;
	}

	for(i=0;i<num;i++){
		h = 0;

		for(j=0;j<num;j++){
			if(!have_ready[j] && pcbdata[j].time_start <= tmp){
				int hrf = (tmp-pcbdata[j].time_start+pcbdata[j].time_need) / pcbdata[j].time_need;
				if(h<hrf){
					h = hrf;
					idx = j;
				}
			}
		}
		
		ready[i]=idx;
		have_ready[idx]=1;
		tmp += pcbdata[idx].time_need;

	}

	tmp = pcbdata[ready[0]].time_start;
	for(i=0;i<num;i++){
		printf("第%d个进程--%s, ",i+1,pcbdata[ready[i]].name);
		printf("到达时间 -- %d, 服务时间 -- %d\n",
				pcbdata[ready[i]].time_start,
				pcbdata[ready[i]].time_need);
		printf("本进程正在运行..............");
		_sleep(1);
		printf("运行完毕\n");
		tmp += pcbdata[ready[i]].time_need;
		j = tmp - pcbdata[ready[i]].time_start;
		k = (float)j / pcbdata[ready[i]].time_start;
		printf("完成时间 -- %d, 周转时间 -- %d, "
				"带权周转时间 -- %.1f\n\n",tmp,j,k);
		
	}
	printf("----------所有进程调度完毕--------\n");
}

void Timeslice()
{
	int i,j,tmp;
	int finish=0;
	double k;
	for(i=0;i<num;i++){
		order[i] = pcbdata[i].time_start;
		ready[i] = i;
	}
	for(i=0;i<num;i++)
		for(j=i+1;j<num;j++){
			if(order[i] > order[j]){
				tmp = order[i];
				order[i] = order[j];
				order[j] = tmp;
				tmp = ready[i];
				ready[i] = ready[j];
				ready[j] = tmp;
			}
		}
	tmp = pcbdata[ready[0]].time_start;
	for(j=0;finish<num;j++){
		i = j%num;
		printf("第%d个进程--%s, ",i+1,pcbdata[ready[i]].name);
		printf("到达时间 -- %d, 服务时间 -- %d\n",
				pcbdata[ready[i]].time_start,
				pcbdata[ready[i]].time_need);
		printf("本进程正在运行..............");
		_sleep(1);
		printf("时间片运行完毕\n");
		
		if(pcbdata[ready[i]].time_left >= time_unit){
			pcbdata[ready[i]].time_left -= time_unit;
			tmp += time_unit;
		}else{
			tmp += pcbdata[ready[i]].time_left;
			pcbdata[ready[i]].time_left = 0;
		}

		printf("剩余服务时间 -- %d\n",pcbdata[ready[i]].time_left);

		int x=0;
		if(pcbdata[ready[i]].time_left == 0){
			x = tmp - pcbdata[ready[i]].time_start;
			k = (float)x / pcbdata[ready[i]].time_start;
			printf("完成时间 -- %d, 周转时间 -- %d, "
					"带权周转时间 -- %.1f\n\n",tmp,x,k);
			finish++;
		}
	}
	printf("----------所有进程调度完毕--------\n");
	
}

void MRLA()
{
	int i,j,tmp,current=-1;
	double k;
	int finish=0;
	int ready2[maxnum],num2=0;
	int ready3[maxnum],num3=0;
	int queue_time[maxnum];

	for(i=0;i<num;i++){
		order[i] = pcbdata[i].time_start;
		ready[i] = i;
		queue_time[i] = time_unit;
	}
	for(i=0;i<num;i++)
		for(j=i+1;j<num;j++){
			if(order[i] > order[j]){
				tmp = order[i];
				order[i] = order[j];
				order[j] = tmp;
				tmp = ready[i];
				ready[i] = ready[j];
				ready[j] = tmp;
			}
		}
	tmp = pcbdata[ready[0]].time_start;

	int r1=0,r2=0,r3=0,x=0;
	for(;finish<num;tmp++){
		r2 %= num; r3 %= num;
		num2 %= num; num3 %= num;
		if(pcbdata[ready[r1]].time_start<=tmp && r1<num){
			pcbdata[ready[r1]].time_left--;
			queue_time[ready[r1]]--;

			printf("进程--%s,剩余时间片%d\n ",pcbdata[ready[r1]].name,queue_time[ready[r1]]);
			
			if(current!=-1){
				int idx;
				for(idx=r2;idx!=num2;idx %= num){
					if(ready2[idx] == current){
						ready2[num2] = current;
						r2++;
						num2++;
						break;
					}
					idx++;
				}

				for(idx=r3;idx!=num3;idx %= num){
					if(ready3[idx] == current){
						ready3[num3] = current;
						r3++;
						num3++;
						break;
					}
					idx++;
				}
			}

			current = ready[r1];

			if(pcbdata[ready[r1]].time_left==0){
				x = tmp - pcbdata[ready[r1]].time_start;
				k = (float)(x+1) / pcbdata[ready[r1]].time_start;
				printf("进程--%s, ",pcbdata[ready[r1]].name);
				printf("完成时间 -- %d, 周转时间 -- %d, "
				"带权周转时间 -- %.1f\n\n",tmp+1,x+1,k);
				r1++;
				finish++;
				current = -1;
			}
			if(queue_time[ready[r1]] == 0){
				printf("进程--%s, ",pcbdata[ready[r1]].name);
				printf("到达时间 -- %d, 服务时间 -- %d\n",
				pcbdata[ready[r1]].time_start,
				pcbdata[ready[r1]].time_need);
				printf("本进程正在运行..............");
				printf("时间片运行完毕(1)\n");
				ready2[num2]=ready[r1];
				queue_time[ready2[num2]] = 2*time_unit;
				r1++;
				num2++;
				current = -1;
			}
			continue;
		}else if(r2!=num2){
			pcbdata[ready2[r2]].time_left--;
			queue_time[ready2[r2]]--;

			printf("进程--%s,剩余时间片%d\n ",pcbdata[ready2[r2]].name,queue_time[ready2[r2]]);			

			current = ready2[r2];

			if(pcbdata[ready2[r2]].time_left == 0){
				x = tmp - pcbdata[ready2[r2]].time_start;
				k = (float)(x+1) / pcbdata[ready2[r2]].time_start;
				printf("进程--%s, ",pcbdata[ready2[r2]].name);
				printf("完成时间 -- %d, 周转时间 -- %d, "
				"带权周转时间 -- %.1f\n\n",tmp+1,x+1,k);
				r2++;
				finish++;	
				current = -1;
			}
			if(queue_time[ready2[r2]] == 0){
				printf("进程--%s, ",pcbdata[ready2[r2]].name);
				printf("到达时间 -- %d, 服务时间 -- %d\n",
				pcbdata[ready2[r2]].time_start,
				pcbdata[ready2[r2]].time_need);
				printf("本进程正在运行..............");
				printf("时间片运行完毕(2)\n");
				ready3[num3]=ready2[r2];
				queue_time[ready3[num3]] = 4*time_unit;
				r2++;
				num3++;
				current = -1;
			}
			continue;
		}else if(r3!=num3){
			pcbdata[ready3[r3]].time_left--;
			queue_time[ready3[r3]]--;
			printf("进程--%s,剩余时间片%d\n ",pcbdata[ready3[r3]].name,queue_time[ready3[r3]]);
			current = ready3[r3];

			if(pcbdata[ready3[r3]].time_left == 0){
				x = tmp - pcbdata[ready3[r3]].time_start;
				k = (float)(x+1) / pcbdata[ready3[r3]].time_start;
				printf("进程--%s, ",pcbdata[ready3[r3]].name);
				printf("完成时间 -- %d, 周转时间 -- %d, "
				"带权周转时间 -- %.1f\n\n",tmp+1,x+1,k);
				r1++;
				finish++;
				current = -1;
			}
			if(queue_time[ready3[r3]] ==0 ){
				printf("进程--%s, ",pcbdata[ready3[r3]].name);
				printf("到达时间 -- %d, 服务时间 -- %d\n",
				pcbdata[ready3[r3]].time_start,
				pcbdata[ready3[r3]].time_need);
				printf("本进程正在运行..............");
				printf("时间片运行完毕(3)\n");
				ready3[num3]=ready3[r3];
				queue_time[ready3[num3]] = 4*time_unit;
				r3++;
				num3++;
				current = -1;
			}
			continue;
		}		
	}

}

int main()
{
	int i=0,sch=99;
	while(sch!=0){
		scanf("%d",&sch);
		switch(sch){
			case 1:
				FCFS();
				break;
			case 2:
				SJF();
				break;
			case 3:
				HRF();
				break;
			case 4:
				Timeslice();
				break;
			case 5:
				MRLA();
				break;
		}
	}
	_keygo();
	return 0;
}

