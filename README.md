# Concurrent_TCP_Client-Server
This project is to write concurrent TCP client and server programs in C

###TCP Client

client first sends a message to the server, indicating the name of an ASCII file to be sent to the server.

client waits for an ACK message from the server to start sending the data

client also receives the filename (different from the one sent by the client) that the server will use
	 to store the received data within this first ACK message.

After that, the client reads one line at a time from the file and sends to the server.
	client needs to get an ACK message from the server before sending the next line.
	The client may receive an error message from the server during the transmission.
	If an error message is received
		the client should report the error and exit.

an example of a command line that could be used to run your TCP client on port 1234
and send a file specified by "myfile" to the TCP server at address 130.127.45.7:

	./client  130.127.45.7  1234  myfile

	The first command line parameter (argv[1]) must be the TCP server's IP address.
	The second parameter (argv[2]) is the port number that will be used by both the client and server programs.
	The third parameter (argv[3]) is the name of the ASCII file to be sent to the server.

###TCP Server

server creates a TCP socket and waits for the client to send data at a port
	 specified by the command line parameter.

server first receives the filename from the TCP client and create an ASCII file for writing

Since this is a concurrent server,
	 your server must create a filename that not only reflects the original filename in the client
	 but also avoids the name conflict with the other clients.

After the file is created, the server should send an ACK message along with the filename 
	to the client so that the client can start the data transmission
	
The server will then read ASCII data line by line sent by the client
	and reverse the order of characters in each line and write it to the file opened by the server.

The server must send an ACK message for each line it receives

Below is an example of a command line that could be used to run your TCP server on port 1234:

	./server 1234


