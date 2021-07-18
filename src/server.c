#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>      ///< errno
#include <sys/socket.h> ///< socket
#include <netinet/in.h> ///< sockaddr_in
#include <arpa/inet.h>  ///< getsockname
#include <unistd.h>     ///< close
#include<string.h>
#include<netinet/in.h> //#define INET_ADDRSTRLEN 16 #define INET6_ADDRSTRLEN 46
#include <pthread.h>

static char *history = "./resource/history.txt";
static char *database = "./resource/price.txt";
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int socketListenToConnexion;
static	int socketProcessAcceptedConnexion;
static	int isConnexionActive;
static	unsigned short MAX_CONNEXION;
static	unsigned short serverPortNumber;
static	unsigned short  clientPortNumber;
	
static	ssize_t numberOfByteReadFromBuffer;// [-1, +inf]
static	char clientIPv4[INET_ADDRSTRLEN +1];
static	socklen_t clientAddressLength;
static	socklen_t serverAddressLength;
static	char buffer[1024];
static char serverIPv4[128];
static	struct sockaddr_in serverAddrress;
static	struct sockaddr_in clientAddress;



static int getLocalAddress(char* myIP)
{
    const char* googleDNSServerIPAddress = "8.8.8.8";
    int googleDNSServerPortNumber = 53;

    struct sockaddr_in googleDNServerAddress;
    int socketToAcessGoogleDNSServer = socket(AF_INET, SOCK_DGRAM, 0);
    int returnValue = 0;
    //Socket could not be created
    if((socketToAcessGoogleDNSServer = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        returnValue = errno;
        printf( "socket to acess Google DNS Server [create][ERROR][%s]\n", strerror(errno));
        return returnValue;
        
    }

    memset(&googleDNServerAddress, 0, sizeof(googleDNServerAddress));
    googleDNServerAddress.sin_family = AF_INET;
    googleDNServerAddress.sin_addr.s_addr = inet_addr(googleDNSServerIPAddress);
    googleDNServerAddress.sin_port = htons(googleDNSServerPortNumber);

    if (connect(socketToAcessGoogleDNSServer, (const struct sockaddr*)&googleDNServerAddress, 
    sizeof(googleDNServerAddress)) < 0)
    {
        returnValue = errno;
        printf( "connect to Google DNS Server[ERROR][%s]\n", strerror(errno));
        close(socketToAcessGoogleDNSServer);
        return returnValue;
  
    }

    struct sockaddr_in localHostIPName;
    socklen_t localHostIPNameLength = sizeof(localHostIPName);

    if (getsockname(socketToAcessGoogleDNSServer, (struct sockaddr*)&localHostIPName, 
    &localHostIPNameLength) < 0)
    {
        returnValue = errno;
        printf( "unable to get local IP through getsockname[ERROR][%s]\n", strerror(errno));
        close(socketToAcessGoogleDNSServer);
        return returnValue;
    }      
    char bufferIPv4[INET_ADDRSTRLEN +1];
    //const char* p = inet_ntop(AF_INET, &localHostIPName.sin_addr, bufferIPv4, INET_ADDRSTRLEN);

    if(inet_ntop(AF_INET, &localHostIPName.sin_addr, bufferIPv4, INET_ADDRSTRLEN) == NULL)
    {
        returnValue = errno;
        printf( "unable to convert IP from binary to text dotted-decimal format[ERROR][%s]\n", strerror(errno));
        close(socketToAcessGoogleDNSServer);
        return returnValue;      
    }

    //inet_ntop

    
  	strcpy(myIP,bufferIPv4 );    
    close(socketToAcessGoogleDNSServer);
        
    return 0;
       
}





// Define a custon constructor
void MyServerInit(unsigned short _portNumber_, unsigned short _max_connexion_)
{

	serverPortNumber = _portNumber_;
	serverAddressLength = sizeof(serverAddrress);
	memset(&serverAddrress, 0, sizeof(serverAddrress));
	memset(&clientAddress, 0, sizeof(clientAddress));
    serverAddrress.sin_family = AF_INET;
    serverAddrress.sin_addr.s_addr = INADDR_ANY;
    serverAddrress.sin_port = htons(serverPortNumber);
	clientAddressLength = sizeof(clientAddress);
	MAX_CONNEXION = _max_connexion_;
	isConnexionActive = 0;
}
static void showMaxConnexion()
{
	printf( "Server max connexion: [%d]\n", MAX_CONNEXION);

}
// createServerForBindAndListen a socket 
static void createServerForBindAndListen()
{
	showMaxConnexion();
	printf( "Creating a server socket ...\n");

	socketListenToConnexion = socket(AF_INET, SOCK_STREAM, 0);
	printf( "A client socket created\n");
	if (socketListenToConnexion < 0)
	{	
		printf("Error while creating a server socket[ERROR][%s]\n", strerror(errno));

	}
	else
	{
		
		printf( "A client socket created\n");

	}
}

// Bind a socket to a specific port number
static void  bindServerToPortNumber()
{

	printf( "Binding to a local port ...\n");

    if (bind(socketListenToConnexion,
			(struct sockaddr *) &serverAddrress, 
					sizeof(serverAddrress)) < 0)
    {  	
		printf("Error while binding a socket[ERROR][%s]\n", strerror(errno));
    }
	else
	{
		
		printf( "Bind success\n");
	}
}

static void getClientAddress()
{
    if(inet_ntop(AF_INET, &clientAddress.sin_addr, clientIPv4, 
				INET_ADDRSTRLEN) == NULL)
    {
        printf( "unable to convert IP from binary to text dotted-decimal format[ERROR][%s]\n", strerror(errno)); 
    }

}
// Listen to a new connexion 
static void  listenToNewClientConnexion()
{

	printf( "Listenning to a new connexion...\n");
  	if (listen(socketListenToConnexion, MAX_CONNEXION) < 0)
    {	
    	printf("Error while Listenning to a new connexion[ERROR][%s]\n", strerror(errno));
    }
	else
	{
		printf( "A client connected to a server\n");
	}
}
static void showClientIP()
{

	printf( "Client IP: [%s]\n", clientIPv4);
}
//Close connexion
static void getPort()
{

    if( getsockname(socketListenToConnexion, (struct sockaddr *)&serverAddrress, 
		&serverAddressLength) < 0)
		{
			printf("Fail to get server port number from getsockname[ERROR][%s]\n", strerror(errno));						
		}
	else
	{
		int returnValue = ntohs(serverAddrress.sin_port);  // get the port number
		if(returnValue < 0)
		{
			printf("ntos error with sin_port[ERROR][%s]\n", strerror(errno));					
		}
		else
		{
			serverPortNumber = (unsigned short) returnValue;
		}
	}
}

int showServerPort()
{
	getPort();
	printf( "Server Port Number: [%d]\n", serverPortNumber);
}
static void showServerIP()
{

	if( getLocalAddress(serverIPv4) < 0)
	{

        printf( "Fail to get server address from getLocalAddress[ERROR][%s]\n", strerror(errno)); 
	}
	else
	{
		printf( "Server IP: [%s]\n", serverIPv4);
	}
}
static void showClientPort()
{

	clientPortNumber = ntohs(clientAddress.sin_port);
    if(clientPortNumber < 0)
    {
        printf( "unable to convert Port Number from network to host order [ERROR][%s]\n", strerror(errno));    
    }
	else
	{
		printf( "Client Port Number: [%d]\n", clientPortNumber);

	}

}

// Accept a connexion
static void  acceptToMakeConnexionWithClient()
{

	printf( "Accepting a connexion...\n");
    socketProcessAcceptedConnexion = accept(socketListenToConnexion,
										(struct sockaddr *)&clientAddress,
										&clientAddressLength);
     if (socketProcessAcceptedConnexion < 0)
     {
	
     	printf("Error while accepting a connexion[ERROR][%s]\n", strerror(errno));
     }
	 else
	 {
     	printf("Connexion accepted\n");
		getClientAddress();
		showClientIP();
		showClientPort();
		isConnexionActive = 1;

	 }
}

static void closeAcceptedConnexion()
{

	printf( "A Server closing an accpeted connexion ...\n");
	if(close(socketProcessAcceptedConnexion) < 0)
	{

     	printf("Fail to close a connexion with a server[ERROR][%s]\n", strerror(errno)); 

	}
	else
	{
     	printf("Accepted connexion closed\n");
		 isConnexionActive = 0;

	}
}
// Read a client message
static int readIncomingMessageFromClient()
{

	if(isConnexionActive)
	{
		memset(buffer, 0, sizeof(buffer));
		numberOfByteReadFromBuffer = read(socketProcessAcceptedConnexion, buffer, 
											sizeof(buffer));
		if (numberOfByteReadFromBuffer < 0)
		{		 
			printf("Error while reading client message[ERROR][%s]\n", strerror(errno));
			return errno;
		}
		else if(numberOfByteReadFromBuffer == 0)
		{
			printf( "Closing connexion with client\n");
			closeAcceptedConnexion();
			return errno;
		}
		else
		{
			//got message from client 
			return 0;

		}		
	}

}

// Send back a message to the client
void sendResponseToClient(char *message, int status)
{

	if(isConnexionActive)
	{
			switch (status)
			{
			case 200:
				numberOfByteReadFromBuffer = write(socketProcessAcceptedConnexion,
											message,18);
				if (numberOfByteReadFromBuffer < 0)
				{	
					printf("Fail to send back message to the client[ERROR][%s]\n",  strerror(errno));
					
				}			
				break;
			case 400:
				printf("Connexion with client closed.\n");			
			
			default:
				printf("Unknown sendResponseToClient ERROR.\n");

				break;
			}
	}
	
}



static void closeListenningSocket()
{

		printf( "A Server stopping listen to a new connexion ...\n");
		if(close(socketListenToConnexion) < 0)
		{
     	printf("Fail to close a listenning socket[ERROR][%s]\n", strerror(errno));  
	

		}
		else
		{
     	printf("Listenning socket closed\n");
		printf( "A server stopped listen to a new connexion\n");	

		}
}

//Get current time, not used here
static void getCurrentTimeForFile(char *retTime)
{
	time_t timer;
    char bufferLocal[26];
    struct tm* tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(bufferLocal, 26, "%Y_%m_%d_%H_%M_%S.txt", tm_info);
    //puts(bufferLocal);
    strcpy(retTime, bufferLocal);

}

//Get current time, not used here
static void getCurrentTimeForLog(char *retTime)
{
	time_t timer;
    char bufferLocal[26];
    struct tm* tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(bufferLocal, 26, "%Y-%m-%d %H:%M:%S", tm_info);
    //puts(bufferLocal);
    strcpy(retTime, bufferLocal);
}

//client connxion info, server to connect to IP and port number 
typedef struct pthreadarg_
{
    pthread_t *id;
    int num;
}pthreadArg;

//static void processClientCommand(FILE* clientFilename, int index)
static void processClientCommand(int index)
{

	char filenameWithCurrentTime[200];
	char filenameWithClientNumber[200];
    getCurrentTimeForFile(filenameWithCurrentTime);
	sprintf(filenameWithClientNumber, "Client[%d]_%s", index, filenameWithCurrentTime);
    // open file, name is price.txt
    FILE *fpc = fopen(filenameWithClientNumber, "w"); 
	if (fpc == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }       
	// open file, name is price.txt
	pthread_mutex_lock(&mutex);
    FILE *fpp = fopen(database, "r"); 
	if (fpp == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }   
	// open file, name is price.txt
    FILE *fph = fopen(history, "a"); 
	if (fph == NULL)
    {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }
	pthread_mutex_unlock(&mutex);
	//customer name 
	while(1)
	{
			if( readIncomingMessageFromClient() == 0)break;
	}	
	char customerName[100];
	strcpy(customerName, buffer);
	char timeHistory[100];
	getCurrentTimeForLog(timeHistory);
    printf("[Client %d] Name is [%s]\n", index, customerName);
	pthread_mutex_lock(&mutex);
    fprintf(fph, "[%s][Client %d] Name is [%s]\n", timeHistory, index, customerName);
	pthread_mutex_unlock(&mutex);
    fprintf(fpc, "[%s][Client %d] Name [%s]\n", timeHistory, index, customerName);	

	//number of product type
	while(1)
	{
			if( readIncomingMessageFromClient() == 0)break;
	}
	int totalProductType = atoi(buffer);
	getCurrentTimeForLog(timeHistory);
    printf("[Client %d] number of product type is [%d]\n", index, totalProductType);
	pthread_mutex_lock(&mutex);
    fprintf(fph, "[%s][Client %d] number of product type is [%d]\n", timeHistory, index, totalProductType);
	pthread_mutex_unlock(&mutex);
    fprintf(fpc, "[%s][Client %d] number of product type is [%d]\n", timeHistory, index, totalProductType);
	int findProduct = 0;
	float unitPrice = 0;
	float totalPrice = 0;
	//For each type of product insert name and quantity
    while(totalProductType--)
    {
		findProduct = 0;
		readIncomingMessageFromClient();
        char productName[128];
		strcpy(productName, buffer);
		getCurrentTimeForLog(timeHistory);
        printf("[Client %d] Product name is [%s]\n", index, productName);
		pthread_mutex_lock(&mutex);
		fprintf(fph, "[%s][Client %d] Product name is [%s]\n", timeHistory, index, productName);
		pthread_mutex_unlock(&mutex);
		fprintf(fpc, "[%s][Client %d] Product name is [%s]\n", timeHistory, index, productName);
		readIncomingMessageFromClient();
        char productQuantity[128];
		strcpy(productQuantity, buffer);
		getCurrentTimeForLog(timeHistory);
		printf("[Client %d] Product[%s] quantity is [%s]\n", index, productName, productQuantity);
		pthread_mutex_lock(&mutex);
		fprintf(fph, "[%s][Client %d] Product quantity is [%s]\n", timeHistory, index, productQuantity);
		fprintf(fpc, "[%s][Client %d] Product quantity is [%s]\n", timeHistory, index, productQuantity);
        char line[4095+1];// read the line
        // check if the word is there 
        while (fgets(line, sizeof line, fpp))
        {
            if (strstr(line, productName))
            {
                    // found return 1 else 0
                    findProduct = 1;
					// get last element of the line which is the price
					// and convert it to float
					char *p = strrchr(line, ' ');
					if (p && *(p + 1))
					{
						unitPrice =  atof(p + 1);
					}
					totalPrice = unitPrice*(atoi(productQuantity));
					getCurrentTimeForLog(timeHistory);
					printf("Product [%s] Quantity[%s] Unit Price[%f] Total Price[%f]\n", productName, productQuantity, unitPrice, totalPrice);
					fprintf(fph, "[%s][Client %d]Product [%s] Quantity[%s] Unit Price[%f] Total Price[%f]\n", timeHistory, index, productName, productQuantity, unitPrice, totalPrice);
					fprintf(fpc, "[%s][Client %d]Product [%s] Quantity[%s] Unit Price[%f] Total Price[%f]\n", timeHistory, index, productName, productQuantity, unitPrice, totalPrice);
					break;
            }
        }
		
		if(findProduct == 0)
		{
			printf("Product [%s] does not exit in database\n", productName);
			fprintf(fph, "[%s][Client %d]Product [%s] does not exit in database\n", timeHistory, index, productName);

		}
		pthread_mutex_unlock(&mutex);

    }
	pthread_mutex_lock(&mutex);
	fclose(fph);
	pthread_mutex_unlock(&mutex);
	fclose(fpc);
	fclose(fpp);


}
void *functionThread(void *arg)
{
	pthreadArg * pArgs = (pthreadArg *) arg;
	processClientCommand(pArgs->num);
	closeAcceptedConnexion();
	return NULL;
}

static void createMyThread( pthreadArg *arg)
{


        if( pthread_create( arg->id , NULL ,  functionThread , (void*) arg) < 0)
        {
            perror("could not create thread");

        }
		sleep(3);
}

//client connxion info, server to connect to IP and port number 
typedef struct CONN_INFO
{
    unsigned short serverPort;
    unsigned short max;
}CONN_INFO;
//Initialize conn_info
static CONN_INFO gConnectionInfo = {0};
static void PrintCommandLineOptions()
{
    printf("#########################################################################\n");
    printf("./Server [options]\n");
    printf("[options]\n");
    printf("[M]\t-m [Server Max Client]\t\t\tServer maximum clients.\n");
    printf("[M]\t-p [Server Port Number]\t\tServer to connect Port Number.\n");
    printf("[O]\t-h \t\t\t\tShow usage\n");
}

static int CommandLineParser(int argc, char *argv[], CONN_INFO *conn_info)
{
	//default command options
	//default client number that can connect to the server
	conn_info->max = 5;
	//default server port number is decided by the OS
	conn_info->serverPort = 0;

    int option = 0;
	
    while((option = getopt(argc, argv, "m:p:h")) > 0)
	{
        switch(option) 
		{
          case 'm':
		  	conn_info->max = (unsigned short) strtoul(optarg, NULL, 0);
            printf("Server max clients %d\n", conn_info->max);
            break;
          case 'p':
			conn_info->serverPort = (unsigned short) strtoul(optarg, NULL, 0);
            printf("Server port number from command line[%d]\n", conn_info->serverPort);
            break;
          case 'h':
            return -1;
          default:
            printf("Unknown option %c\n", option);
            return -1;
        }

    }
	
    return 0;
}
int main(int argc, char const *argv[])
{
	/* code */

	CONN_INFO *conn_info = malloc(sizeof *conn_info); 
	pthreadArg *pArgument = malloc(sizeof *pArgument); 
	conn_info = &gConnectionInfo;

	    /* Check command line options */
    int out = CommandLineParser(argc, argv, conn_info);

    if(out < 0)
	{

        printf("CommandLineParser ret[%d]\n", out);
        PrintCommandLineOptions();
        return -1;
    }	


	MyServerInit(conn_info->serverPort, conn_info->max);
	showServerIP();
	createServerForBindAndListen();
	bindServerToPortNumber();
	showServerPort();
	listenToNewClientConnexion();
	pArgument;
	const int maxClient = conn_info->max;
   	pthread_t tid[maxClient];
	int countThread = 0;
  
	while (countThread < maxClient)
	{
		acceptToMakeConnexionWithClient();
		pArgument->id = tid[countThread];
		pArgument->num = countThread + 1;
		createMyThread(pArgument);
	}	
	for (int i = 0; i < maxClient; i++)pthread_join(tid[i], NULL);
	free(conn_info);
	return 0;
}
