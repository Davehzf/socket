#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define MYPORT  8888
#define QUEUE   20
#define BUFFER_SIZE 1024
struct socket_date{
	struct sockaddr_in client_info;
	int client_fd;
	int client_id;	
}socket_date;

void data_trans(struct socket_date *p)
{
	int i = 0;
	printf("\nclient ip is 		%s\n",inet_ntoa(p->client_info.sin_addr));
	printf("\nclient port is 	%d\n",p->client_info.sin_port);
	printf("\nclient client_fd is	%d\n",p->client_fd);
	printf("\nclient client_id is	%d\n",p->client_id);
/*	while(1)
	{
		usleep(3000000);
		send(conn,)
	}
*/
	close(p->client_fd);
	pthread_exit(NULL);

}



int main()
{
	///定义sockfd
	int server_sockfd = socket(AF_INET,SOCK_STREAM, 0);
	
	

	int conn_fd;
	int count;
	int length;
	
	
	
	char buffer[BUFFER_SIZE];
	///定义sockaddr_in
	struct sockaddr_in server_sockaddr;
	struct sockaddr_in client_addr;

	length = sizeof(client_addr);

	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(MYPORT);
//	server_sockaddr.sin_addr.s_addr = htonl(0xc0a80108l);192.168.1.8
	server_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);



	// 设置套接字选项避免地址使用错误 bind: Address already in use  
	int on=1;  
	if((setsockopt(server_sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)  
	{  
		perror("setsockopt failed");  
		exit(1);  
	} 

	///bind，成功返回0，出错返回-1
	if(bind(server_sockfd,(struct sockaddr *)&server_sockaddr,sizeof(server_sockaddr)) == -1)
	{
		perror("bind");
		exit(1);
	}

	///listen，成功返回0，出错返回-1
	if(listen(server_sockfd,QUEUE) == -1)
	{
		perror("listen");
		exit(1);
	}

	while(1)
	{
		pthread_t	thread_id;
		struct socket_date s_socket_date;
		
		
		
		conn_fd = accept(server_sockfd, (struct sockaddr*)&client_addr, &length);
		if(conn_fd < 0)
		{
			perror("connect");
			exit(1);
		}
		count++;
		
		memcpy(&s_socket_date.client_info,&client_addr,sizeof(client_addr));
		s_socket_date.client_fd = conn_fd;
		s_socket_date.client_id = count;
		
		
		
		pthread_create(&thread_id,NULL,data_trans,&s_socket_date);

/*		while(1)
		{
			memset(buffer,0,sizeof(buffer));
			int len = recv(conn, buffer, sizeof(buffer),0);
			if(strcmp(buffer,"exit\n")==0)
				break;
			fputs(buffer, stdout);
			send(conn, buffer, len, 0);
		}
		close(conn_fd);
		close(server_sockfd);
*/
	}
	return 0;
}
