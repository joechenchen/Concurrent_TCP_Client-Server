/*
 ██████╗    ██╗         ██╗    ███████╗    ███╗   ██╗    ████████╗            ██████╗
██╔════╝    ██║         ██║    ██╔════╝    ████╗  ██║    ╚══██╔══╝           ██╔════╝
██║         ██║         ██║    █████╗      ██╔██╗ ██║       ██║              ██║     
██║         ██║         ██║    ██╔══╝      ██║╚██╗██║       ██║              ██║     
╚██████╗    ███████╗    ██║    ███████╗    ██║ ╚████║       ██║       ██╗    ╚██████╗
 ╚═════╝    ╚══════╝    ╚═╝    ╚══════╝    ╚═╝  ╚═══╝       ╚═╝       ╚═╝     ╚═════╝
*/

#include "unp.h"

int main( int argc, char **argv) {

	int					sockfd;
	struct sockaddr_in servaddr;

	if( argc != 4) {
	
		err_quit("usage: ./client <IP Address> <Port Number> <File>");
	}
	
	sockfd = Socket( AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA*) &servaddr, sizeof(servaddr));

	FILE* fp; 

	if( access( argv[3], F_OK) != -1) {
		Writen(sockfd, argv[3], strlen(argv[3]));
		fp = fopen( argv[3], "r");
	}
	else {
		err_quit("file name does not exist");
	}

	unsigned short name_size = 128;
	char server_name[name_size];
	Read(sockfd, server_name, name_size);
	printf("server name: %s\n", server_name);

	str_cli(fp, sockfd);

//	printf("client closing file\n");
//	fflush(stdout);
	fclose( fp);
	Close(sockfd);

	exit(0);
}
