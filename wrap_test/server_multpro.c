#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <ctype.h>
#include <unistd.h>

#include "wrap.h"

#define SERV_PORT 6666


void do_sigchild(int num)
{
    while (waitpid(0, NULL, WNOHANG) > 0)
        ;
}

int main(void)
{
   	int lfd,connfd;
	pid_t pid;
	int n,i;
	socklen_t cliaddr_len;
	char buf[BUFSIZ],str[BUFSIZ];
	struct sockaddr_in s_addr,c_addr;


	lfd = Socket(AF_INET,SOCK_STREAM,0);
	s_addr.sin_family=AF_INET;
	s_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	s_addr.sin_port = htons(SERV_PORT);


 struct sigaction newact;
 newact.sa_handler = do_sigchild;
    sigemptyset(&newact.sa_mask);
    newact.sa_flags = 0;
    sigaction(SIGCHLD, &newact, NULL);
  int opt = 1;
    setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));



	
	Bind(lfd,(struct sockaddr *)&s_addr,sizeof(s_addr));
	
	Listen(lfd,128);
	
	printf("wait connect...\n");
	
	while(1)
	{	  cliaddr_len = sizeof(c_addr);
		connfd = Accept(lfd, (struct sockaddr *)&c_addr, &cliaddr_len);
		pid = fork();
		if (pid == 0) {
		    Close(lfd);
		    while (1) {
		        n = Read(connfd, buf, sizeof(buf));
		        if (n == 0) {
		            printf("the other side has been closed.\n");
		            break;
		        }
		        printf("received from %s at PORT %d\n",
		                inet_ntop(AF_INET, &c_addr.sin_addr, str, sizeof(str)),
		                ntohs(c_addr.sin_port));

		        for (i = 0; i < n; i++)
		            buf[i] = toupper(buf[i]);

		        Write(STDOUT_FILENO, buf, n);
		        Write(connfd, buf, n);
		    }
		    Close(connfd);
           		 return 0;}
		else if(pid >0)
		{
			Close(connfd);
		}
		else 
		{
			perr_exit("fock");
		}
	}
    return 0;
}



