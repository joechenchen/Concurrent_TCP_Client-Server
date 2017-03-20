#include	"unp.h"

void reverse_string(char *str);
void str_cli(FILE *fp, int sockfd ) {

	char	sendline[MAXLINE], recvline[MAXLINE];
	unsigned int checksum;
	

		int i = 0;
	while ( fgets(sendline, MAXLINE, fp) > 0 ) {

//		printf("%s", sendline);
	
		//create a checksum 
		checksum = csum( sendline, MAXLINE);

	//set socket options
  int flag = 1;
  int result = setsockopt(sockfd,            /* socket affected */
                          IPPROTO_TCP,     /* set option at TCP level */
                          TCP_NODELAY,     /* name of option */
                          (char *) &flag,  /* the cast is historical 
                                                  cruft */
                          sizeof(int));    /* length of option value */

		//send message
		Writen(sockfd, sendline, strlen(sendline));

		//clear buffer 
		bzero(sendline, MAXLINE);

		//wait for checksum from server
//		printf("waiting on checksum\n");
		Read(sockfd,recvline, MAXLINE); 
		printf( "packet %d : %s\n", ++i,  checksum ^ recvline[0] == 0 ? "good checksum" : "bad checksum");
		

	/*
		if(Readline(sockfd, recvline, MAXLINE) == 0) {
			err_quit("str_cli: server terminated prematurely");
		}
	*/
	}
}

void str_serv(FILE *fp, int sockfd) {

	char	recvline[MAXLINE], sendline[MAXLINE];
	ssize_t temp;

	int i = 0;
	do {
//		printf("\tpacket: %d\t", ++i);
		
		//read the socket into the buffer
		temp = read(sockfd, recvline, MAXLINE);
		//printf(" %s ", recvline);
		
		//create checksum of the buffer and send it to client
		sendline[0] = csum(recvline, strlen(recvline));
		//printf("sending checksum\n");
		Writen(sockfd, sendline, strlen(sendline));

		
		//reverse the string and put it into the file
		reverse_string(recvline);
//		printf(" %s ", recvline);

		Fputs(recvline, fp);
		fflush(fp);
		if( temp  < 0) {
			err_sys("str_echo: read error");
		}
		bzero(recvline, MAXLINE);
	} while ( temp >= 0 && errno != EINTR );


}
void reverse_string(char *str) {
                                                                                                              
    /* skip null */                                                                                           
    if (str == 0)                                                                                             
    {                                                                                                         
        return;                                                                                               
    }                                                                                                         
                                                                                                              
    /* skip empty string */                                                                                   
    if (*str == 0)                                                                                            
    {                                                                                                         
        return;                                                                                               
    }                                                                                                         
                                                                                                              
    /* get range */                                                                                           
    char *start = str;                                                                                        
    char *end = start + strlen(str) - 2; /* buff is length 100 */                                             
    char temp;                                                                                                
                                                                                                              
    /* reverse */                                                                                             
    while (end > start)                                                                                       
    {                                                                                                         
        /* swap */                                                                                            
        temp = *start;                                                                                        
        *start = *end;                                                                                        
        *end = temp;                                                                                          
                                                                                                              
        /* move */                                                                                            
        ++start;                                                                                              
        --end;                                                                                                
    }                                                                                                         
}         
