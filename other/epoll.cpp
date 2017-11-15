#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/epoll.h>
using namespace std;
int main(void)
{
	int epfd,nfds,fd1,fd2,count;
	char buf[1024];
	struct epoll_event ev,events[5];//ev用于注册事件，数组用于返回要处理的事件
	
	fd1 = open("./error.c", O_RDWR);
	fd2 = open("./epoll.cpp", O_RDWR);

	epfd=epoll_create(1);

	ev.data.fd=fd1;
	ev.events=EPOLLIN | EPOLLHUP;
	epoll_ctl(epfd,EPOLL_CTL_ADD,fd1,&ev);
	
	ev.data.fd=fd2;
	ev.events=EPOLLIN | EPOLLHUP;
	epoll_ctl(epfd,EPOLL_CTL_ADD,fd2,&ev);

	for(;;)
	{
		nfds=epoll_wait(epfd,events,5,-1);
		cout << 1 << endl;
		for(int i=0;i<nfds;i++)
		{
			if(events[i].events & EPOLLIN)
				cout<<"hello world!"<<endl;
			while(1){
				count = read(events[i].data.fd, buf, 1024);
				if(count <= 0){
					break;
					close(events[i].data.fd);
				}
			}
			if(events[i].events & EPOLLHUP){
				cout << "hup" << endl;
			}
		}
	}
}
