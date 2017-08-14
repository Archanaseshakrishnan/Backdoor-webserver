#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 2    // how many pending connections queue will hold
#define MAXDATASIZE 100000
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

/*void tostring(char str[], long num)

{

    long rem, len = 0;

    long i,n;

    n = num;

    while (n != 0)

    {

        len++;

        n /= 10;

    }

    for (i = 0; i < len; i++)

    {

        rem = num % 10;

        num = num / 10;

        str[len - (i + 1)] = rem + '0';

    }

    str[len] = '\0';

}
*/
int main(int argc, char* argv[])
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1,status,found=0;
    char s[INET6_ADDRSTRLEN];
    char statement[100][100];
    int rv,i=0,j=0,k=0,l=0,m=0,n=0;
    int port,numbytes;
    port=atoi(argv[1]);
    char message[MAXDATASIZE];
    char path[MAXDATASIZE],words[MAXDATASIZE],command[MAXDATASIZE],command1[MAXDATASIZE];
    FILE *fp;
    int countspaces=0;
    char *reply2;
    char path2[MAXDATASIZE]="";
    char result[100];
    long len;
    int prev;
    char reply[MAXDATASIZE] = 
"HTTP/1.1 200 OK\n"
"Content-Type: text/html\n"
"Accept-Ranges: bytes\n"
"Connection: close\n"
"\n"
;
char reply1[MAXDATASIZE] = 
"HTTP/1.1 404 Not Found\n"
"Content-Type: text/html\n"
"Accept-Ranges: bytes\n"
"Connection: close\n"
"\n"
"404 not found"
;	
    

  // fp = fopen("~/test.txt", "w+");

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while(1) {  // main accept() loop

        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
            get_in_addr((struct sockaddr *)&their_addr),
            s, sizeof s);
        printf("server: got connection from %s\n", s);

              
           /* if (send(new_fd, "Hello, world!", 13, 0) == -1)
                perror("send");*/
           if ((numbytes = recv(new_fd, message, MAXDATASIZE-1, 0)) == -1) {
        perror("recv");
           exit(1);
	}
    message[numbytes] = '\0';
    
  /*  while(message[i]!='\0')
    {
       if(message[i]!='/')
	{	
		if(message[i]=='%')
		{
			printf("true");
			if(message[i+1]=='2' && message[i+2]=='0')
			 //{i+=3;statement[k][j]='\0';k++;j=0;countspaces++;}
			{statement[k][j]='\t';j++;i+=2;}
		}
		else if(message[i]!='+' && message[i]!='=' && message[i]!='?' && message[i]!='.' && message[i]!='#' && message[i]!='@' && message[i]!='$' && message[i]!='!' && message[i]!='^' && message[i]!='&' && message[i]!='*' && message[i]!='(' && message[i]!=')' && message[i]!='%')
		{statement[k][j]=message[i]; j++;} 
	}     
	else
	{
		if(message[i+1]=='/')
		i++;
		statement[k][j]='\0';k++;j=0;
	}
	i++;
    }*/
printf("server: received '%s'\n",message);
    /*while(message[i]!='\0')
    {
	if(message[i]==32)
	{
		statement[k][j]='\0';
		k++;j=0;i++;prev=32;
	}
	else if(message[i]=='/')
	{
		i++;
		if(prev!=32)
		{
			statement[k][j]='\0';
			k++;j=0;prev='/';
		}
		
	}
	else
	{
		if(message[i]=='%')
		{
		//	printf("true");
			if(message[i+1]=='2' && message[i+2]=='0')
			 //{i+=3;statement[k][j]='\0';k++;j=0;countspaces++;}
			{statement[k][j]='\t';j++;i+=3;prev='%';}
		}
		else if(message[i]!='+' && message[i]!='=' && message[i]!='?' && message[i]!='.' && message[i]!='#' && message[i]!='@' && message[i]!='$' && message[i]!='!' && message[i]!='^' && message[i]!='&' && message[i]!='*' && message[i]!='(' && message[i]!=')' && message[i]!='%')
		{statement[k][j]=message[i]; prev=message[i];j++;i++;}
	}
	i++;
    }*/
    while(message[i]!='\0')
    {
       if(message[i]==32)
	{i++;statement[k][j]='\0';k++;j=0;}
       else
	{statement[k][j]=message[i]; j++; i++;}
    }

    for(i=0;i<=k;i++)
	{printf("%s \n",statement[i]);
	 if(strcmp("GET",statement[i])==0)
	 {break;}
    }
	if(i>k)
	{
		if (send(new_fd, reply1, strlen(reply1), 0) == -1)
                perror("send");
    		close(sockfd);
    		close(new_fd);
                return 0;
	}
m=0;n=0;
	for(l=0;l<=k;l++)
	{
		while(statement[l][m]!='\0')//have to check inequality here
		{
			if(statement[l][m]=='/')
			{
				if(statement[l][m+1]=='e' && statement[l][m+2]=='x' && statement[l][m+3]=='e' && statement[l][m+4]=='c' && statement[l][m+5]=='/')
				{
					m=m+6;
					while(statement[l][m]!='\0')
					{
						command[n]=statement[l][m];m++;
						printf("%c",command[n]);
						n++;
					}
					command[n]='\0';
					printf("\n%s",command);found=1;break;
				}
			}
		m++;
		}
		m=0;
		if(found==1)
		break;
	}	
if(l>k)
{
	if (send(new_fd, reply1, strlen(reply1), 0) == -1)
                perror("send");
		close(new_fd);    		
		close(sockfd);
    		return 0;
}
    /*for(i=0;i<=k;i++)
	{printf("%s \n",statement[i]);
	 
	 if(strcmp("exec",statement[i])==0)
	 {break;}
    }*/
      
	/*if(i>k)
	{
		if (send(new_fd, reply1, strlen(reply1), 0) == -1)
                perror("send");
/*		reply1[MAXDATASIZE]="HTTP/1.1 404 Not Found\n"
"Content-Type: text/plain\n"
"Accept-Ranges: bytes\n"
"Connection: close\n"
"\n"
;
*//*
    		close(sockfd);
    		close(new_fd);
                return 0;
	}*/
    	printf("\n%s",command);
    printf("\nstatement%s",statement[i+1]);
    //printf("\ncommand-%s",command);
           //mycommand "$(< ~/test.txt)";
          // parent doesn't need this
    //fp = popen(system((const char*)strcat(statement[i+1]," > ~/test.txt")), "r");
    /*
    for(l=0;l<=countspaces;l++)
    {
	
        words1=strcat(words,statement[l+i+1]);
	words1=strcat(words,'\t');	
    }*/
l=0;m=0;
while(l<=n)
{
	if(command[l]=='%')
	{
		if(command[l+1]=='2' && command[l+2]=='0')
		{
			command1[m]='\t';m++;l=l+3;
		}
	}
	else
	{
		command1[m]=command[l];m++;l++;
	}
}
command1[m]='\0';
    fp = popen(command1, "r");
  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
   while (fgets(path, MAXDATASIZE, fp) != NULL)
    {
        strcat(path2,path);
	printf("%s", path);
           
    }
        status = pclose(fp);
	printf("%d",status);	
	printf("here%s", path2);
        
     /*  fgets(path,MAXDATASIZE,fp);
	strcat(path2,path);*/ 
        //tostring(result,sizeof(path2));
        //printf("result-%s",result);
        //printf("\nlen-%ld", len);
	/*strcpy(reply2,"Content-Length: ");
        //sprintf(result,"%ld",len);
        strcat(reply2,result);
        strcat(reply,reply2);*/
        // strcat(reply,"\n");
	//strcat(reply,"\n");
           
        strcat(reply,path2);
	//while(reply!=NULL)
    	if (send(new_fd, reply, strlen(reply), 0) == -1)
                perror("send");
	/*reply[MAXDATASIZE]="HTTP/1.1 200 OK\n"
"Content-Type: text/html\n"
"Accept-Ranges: bytes\n"
"Connection: close\n"
"\n"
;*/
	//n=0;
    	close(new_fd);
    
    	  //fputs(system(statement[i+1]),fp);
    }

    close(sockfd);
    return 0;
}
