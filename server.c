/*
███████╗    ███████╗    ██████╗     ██╗   ██╗    ███████╗    ██████╗             ██████╗
██╔════╝    ██╔════╝    ██╔══██╗    ██║   ██║    ██╔════╝    ██╔══██╗           ██╔════╝
███████╗    █████╗      ██████╔╝    ██║   ██║    █████╗      ██████╔╝           ██║     
╚════██║    ██╔══╝      ██╔══██╗    ╚██╗ ██╔╝    ██╔══╝      ██╔══██╗           ██║     
███████║    ███████╗    ██║  ██║     ╚████╔╝     ███████╗    ██║  ██║    ██╗    ╚██████╗
╚══════╝    ╚══════╝    ╚═╝  ╚═╝      ╚═══╝      ╚══════╝    ╚═╝  ╚═╝    ╚═╝     ╚═════╝
*/

#include "unp.h"

int main(int argc, char **argv) {

	int listenfd, connfd;
	pid_t childpid;
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr;
	int opt ;
	
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family		= AF_INET;
	servaddr.sin_addr.s_addr= htonl(INADDR_ANY);

	
	if( argc != 2) {
		err_quit("usage: ./server <Port Number> \n");
	}

	servaddr.sin_port		= htons(atoi(argv[1]));
	
	struct ifaddrs* ifap, *ifa;
	struct sockaddr_in *sa;
	char* addr;

	getifaddrs(&ifap);
	for( ifa = ifap; ifa; ifa = ifa->ifa_next) {

		if( ifa->ifa_addr->sa_family == AF_INET) {

			sa = (struct sockaddr_in *) ifa->ifa_addr;
			addr = inet_ntoa(sa->sin_addr);
			printf("Connect to Interface: %s\tAddress: %s\n", ifa->ifa_name, addr);
		}
	}

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	
	Listen(listenfd, LISTENQ);
	
	for( ; ;) {

	
		clilen = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *) &cliaddr, &clilen);
	
		if( (childpid = Fork()) == 0) {
		
			Close(listenfd);
			
			unsigned short name_size = 64;
			char temp[name_size];
			bzero(temp, name_size);

			Read(connfd, temp, name_size);
			FILE* fp;
			unsigned short server_name_size = 128;
			char new_name[server_name_size];
			
			if( access( temp, F_OK) != -1) {

					// file exists
					bzero(new_name, server_name_size);
				do {

					time_t now = time(NULL);
					strcpy(new_name, temp);

					strcat(new_name,ctime(&now));
					char* d = new_name;
					char* s = new_name;
					do while(isspace(*s)) s++; while(*d++ = *s++);
						
				 } while( access( new_name, F_OK ) != -1 ); 

				Writen(connfd, new_name, strlen(new_name));
//				printf("filename: X%sX \n", new_name);
//				fflush(stdout);
				fp = Fopen( new_name, "ab+");
			}
			else {

//				printf("filename: %s \n", temp);
				strcpy(new_name, temp);
				fp = Fopen( new_name, "ab+");
			}			

			str_serv(fp, connfd);
			
			Fclose(fp);

/*
			fp = Fopen( new_name, "ab+");
			ssize_t read;                                                                                             
			char line[MAXLINE];                                                                                       
			size_t len;   


			while( fgets(line, sizeof(line),fp)) {                                                          
		//		printf("%s", line);
				reverse_string(line);                                                                                 
				printf("%s", line);
				Fputs(line, fp);                                                                                      
			}        
//			printf("server closing file\n");
			Fclose(fp);

*/
			exit(0);
		}
	}
	Close(connfd);
}
