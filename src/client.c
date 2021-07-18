
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <time.h>
#include <errno.h>
static int socketConnectToClient;
static	unsigned short serverPortNumber;
static	int numberOfByteOfBuffer;
static struct sockaddr_in serverAddress;
static	struct hostent *serverIP;
static	char buffer[1024];

// Creation of a socket
static int createClientForConnexion()
{
	int returnValue = 0;

	printf( "Creating a client socket ...\n");

	socketConnectToClient = socket(AF_INET, SOCK_STREAM, 0);
	if (socketConnectToClient < 0)
	{
		returnValue = -200000; 
		printf("Error while creating a client socket"
	"[ERROR][%s][%d]\n", strerror(errno), returnValue); 
	}
	else
	{
		printf( "A client socket created\n");

	}
	return returnValue;
}

// Connect to a server
static int connectClientToServer()
{
	int returnValue = 0;
	printf( "Connecting to a server ...\n");

	bzero((char *) &serverAddress, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;

    bcopy((char *)serverIP->h_addr, (char *)&serverAddress.sin_addr.s_addr,
	serverIP->h_length);

    serverAddress.sin_port = htons(serverPortNumber);

    if (connect(socketConnectToClient,(struct sockaddr *) &serverAddress, 
	sizeof(serverAddress)) < 0)
    {
		returnValue = -200001; 
    	printf("Error while connecting to a server"
	"[ERROR][%s][%d]\n", strerror(errno), returnValue); 

    }
	else
	{
		printf( "A client connected to a server\n");
	}
	return returnValue;
}

// Initialize a  client
static void MyClientInit(char *_serverIP_, unsigned short _serverPort_)
{
	printf("Client initializing ...\n");

	serverPortNumber = _serverPort_;
	createClientForConnexion();
	serverIP = gethostbyname(_serverIP_);
	if (serverIP == NULL)
	{
		printf("Error host not found[%s]\n", strerror(h_errno));
	}
	else
	{
		printf("Client initialized\n");
	}

	connectClientToServer();
}


// Send a message to a server
static int sendRequestToServer(char *message)
{
	int returnValue = 0;
	printf( "A Client sending a request ...\n");


	numberOfByteOfBuffer = write(socketConnectToClient,message,100);
	if (numberOfByteOfBuffer < 0 )
	{
		returnValue = -200002; 		
		printf("Error of sending a request to a server"
            	"[ERROR][%s][%d]\n", strerror(errno), returnValue); 
	}
	else
	{
		printf( "Request sent to a server\n");

	}
	return returnValue;

}

// Receive response from the server
static int receiveResponseFromServer()
{
	int returnValue = 0;
	printf( "A Client receiving a response ...\n");

    bzero(buffer,1024);
     numberOfByteOfBuffer = read(socketConnectToClient,buffer,1023);
    if (numberOfByteOfBuffer < 0)
     {
		returnValue = -200003; 	
     	printf("Fail to receive response from a server"
	        "[ERROR][%s][%d]\n", strerror(errno), returnValue); 

     }
	 {
		 printf( "Response from a server\n");
		 printf("%s\n", buffer);
	 }
	return returnValue;

}

// close connextion with server
static int closeConnexion()
{
	int returnValue = 0;
	printf( "A Client closing a connexion ...\n");
	if(close(socketConnectToClient) < 0)
	{
		returnValue = -200004; 
     	printf("Fail to close a connexion with a server"
	        "[ERROR][%s][%d]\n", strerror(errno), returnValue);  

	}
	else
	{
     	printf("Connexion with a server closed\n");

	}
	return returnValue;
}


//Get current time, not used here
static void getCurrentTime(char *retTime)
{
	struct tm *tm;
	time_t t;
	//char str_time[100];
	t = time(NULL);
	tm = localtime(&t);

	strftime(retTime, sizeof(retTime), "%d _%m _%Y _%H _%M _%S.txt", tm);

}



void makeCommand()
{

  	char str[128];
    printf("Please insert your name:\n");  	
    scanf("%s", str);
    printf("Customer name is [%s]\n", str);
	sendRequestToServer(str);
	sleep(2);
    printf("Please insert the number of product type\n");
    int totalProductType = 0;
    scanf("%d", &totalProductType);
    printf("Total number of product type is [%d]\n", totalProductType);
    sprintf(str, "%d", totalProductType);
	//Send the total number of product type to the server
	sendRequestToServer(str);
	// use malloc,no need to duplicate of free because this is a unique thread
    int bSize = 100;
	//For each type of product insert name and quantity
    while(totalProductType--)
    {
        char *productName = (char*) malloc(bSize*sizeof(char));
        printf("Insert Product name:\n");
        scanf("%s", productName);
        printf("Product name is [%s]\n", productName);
		while(1)
		{
			if(sendRequestToServer(productName) >= 0) break;
		}

        sleep(2);

        char *productQuantity = (char*) malloc(bSize*sizeof(char));
        printf("Insert Product[%s] quantity:\n", productName);
        scanf("%s", productQuantity);

		while(1)
		{
			if(sendRequestToServer(productQuantity) >= 0) break;
		}

        printf("Product[%s] quantity is [%s]\n", productName, productQuantity);

        free(productName);
        free(productQuantity);
    }

}

//client connxion info, server to connect to IP and port number 
typedef struct CONN_INFO
{
    unsigned short serverPort;
    char serverIP[128];
}CONN_INFO;
//Initialize conn_info
static CONN_INFO gConnectionInfo = {0};
static void PrintCommandLineOptions()
{
    printf("#########################################################################\n");
    printf("./Client [options]\n");
    printf("[options]\n");
    printf("[M]\t-i [Server IP]\t\t\tServer to connect to digit IP address.\n");
    printf("[M]\t-p [Server Port Number]\t\tServer to connect to Port Number.\n");
    printf("[O]\t-h \t\t\t\tShow usage\n");
}

static int CommandLineParser(int argc, char *argv[], CONN_INFO *conn_info)
{

    int option = 0;
	
    while((option = getopt(argc, argv, "i:p:h")) > 0)
	{
        switch(option) 
		{
          case 'i':
            strncpy(conn_info->serverIP, optarg, sizeof(conn_info->serverIP));
            //conn_info->udid[MAX_PUBLIC_UDID_LENGTH] = '\0';
            printf("Server IP %s\n", conn_info->serverIP);
            break;
          case 'p':
			conn_info->serverPort = (unsigned short) strtoul(optarg, NULL, 0);
				printf("\nf\n");
            break;
          case 'h':
            return -1;
          default:
            printf("Unknown option %c\n", option);
            return -1;
        }

	 if(strlen(conn_info->serverIP) == 0)
	 {
        printf("Must specify Server IP\n");
        return -1;
    }	 
	
	if(conn_info->serverPort == -1)
	 {
        printf("Must specify Server Port number\n");
        return -1;
    }

    }
	

    if(strlen(conn_info->serverIP) == 0)
	{
        printf("Must specify Server IP\n");
        return -1;
    }    
    return 0;
}

int main(int argc, char *argv[])
{  
	CONN_INFO *conn_info = &gConnectionInfo;
	//No server port number
	conn_info->serverPort = -1;

	    /* Check command line options */
    int out = CommandLineParser(argc, argv, conn_info);

    if(out < 0)
	{

        printf("CommandLineParser ret[%d]\n", out);
        PrintCommandLineOptions();
        return -1;
    }

	//Create a client and tell him/her the server address
	MyClientInit(conn_info->serverIP, conn_info->serverPort);

	makeCommand();
	closeConnexion();
	
	return 0;
}




	