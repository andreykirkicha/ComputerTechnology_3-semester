#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN		4096
#define HTTP_PORT	80
/* Получить и распечатать какую либо страничку из интернета man 3 gethostbyname */
int main (int argc, char ** argv)
{
	int sock, count;
	char * buf;
	struct hostent * host;
	struct sockaddr_in addr;
	
	if (argc < 2) 
	{
		fprintf (stderr, "Too few arguments\n");
		return 1;
	}

	buf = (char *) malloc (BUF_LEN);
	if (buf == NULL) 
	{
		fprintf (stderr, "malloc() error\n");
		return 1;
	}

	sock = socket (AF_INET, SOCK_STREAM, 0);
	if (sock == -1) 
	{
		fprintf (stderr, "socket() error\n");
		return 1;
	}

	addr.sin_family = AF_INET;

	host = gethostbyname(argv[1]);
	if (host == NULL)
	{
		fprintf (stderr, "Unknown server\n");
		return 1;
	}

	addr.sin_addr = *(struct in_addr*)host->h_addr_list[0];

	addr.sin_port = HTTP_PORT;

	printf("o");

	if (connect (sock, (struct sockaddr *) &addr, sizeof (addr)) == -1) 
	{
		fprintf (stderr, "connect() error\n");
		return 1;
	}

	printf("o");
	
	strcpy (buf, "GET /");
	strcat (buf, " HTTP/1.1\nHost: ");
	strcat (buf, argv[1]);
	strcat (buf, "\nUser-Agent: Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:50.0) Gecko/20100101 Firefox/50.0");
	strcat (buf, "\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
	strcat (buf, "\nAccept-Language: en-US,en;q=0.5");
	strcat (buf, "\nAccept-Encoding: gzip, deflate");
	strcat (buf, "\nConnection: keep-alive\n\n");
  	strcat (buf, "\n\n");
	
	write (sock, buf, strlen (buf));
	
	while ((count = read (sock, buf, BUF_LEN)) > 0)
		write (1, buf, count);

	close (sock);
	free (buf);
	return 0;
}
