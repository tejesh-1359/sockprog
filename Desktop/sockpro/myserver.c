#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 4444


	struct sockaddr_in server,new;
	int serverfd;
	int newfd;
	socklen_t new_size;
	pthread_t sendt,recvt;
	char msg[500];
	int length;
	int port;
	int clients[10];
	int n =0,k;
	struct Node* head = NULL;
	int i;

struct Node
{
  int data;
  struct Node *next;
};


void printList(struct Node *node)
{
  while (node != NULL)
  {
     printf("  port number \t ip\n");
     printf(" %d  \t \t 127.0.0.1  \n", node->data);
     node = node->next;
  }
}

 

void add(struct Node** head_ref, int new_data)
{
    
    printf("add \n");
    struct Node* new_node = (struct Node*) malloc(sizeof(struct Node));
    new_node->data  = new_data;
    new_node->next = (*head_ref);
    (*head_ref)    = new_node;
    printf(" The updated table \n");
    printList(head);
}





void delete(struct Node **head_ref, long int key)
{
    
    struct Node* temp = *head_ref, *prev;
 printf("delete \n");
    
    if (temp != NULL && temp->data == key)
    {
        *head_ref = temp->next;  
        free(temp);               
        return;
    }
 
    
    while (temp != NULL && temp->data != key)
    {
        prev = temp;
        temp = temp->next;
    }
 
    
    if (temp == NULL) return;

    prev->next = temp->next; 
    free(temp);
    


}


void sendtoall(char *msg,int curr)
{
printf("Sending the update to all clients \n ")	;
int t = (int)msg[0];

	if(t == 49)
			{
				add(&head,ntohs(new.sin_port));
			}
			else if(t == 50)
			{
				delete(&head,ntohs(new.sin_port));
				printf(" The updated table \n"); 
    			printList(head);
			}
	
	for(i = 0; i < n; i++)
	 {
		if(clients[i] != curr) 
		{
			if((k=send(clients[i],msg,strlen(msg),0) ) <0) 
			{
				perror("send to all call is failed \n");

			}
			//else
				//printf("The no of characters sent are %d \n",k);
			//	send(clients[i],cl.sockno,sizeof(cl.sockno),0);
			
		}
	}
	
}


struct client_data 
{
	int sockno;
};


void *routine(void *sock)
{
	struct client_data cl = *((struct client_data *)sock);
	char msg[500];
	int length;
	int i;
	int j;
	
	while((length = recv(cl.sockno,msg,500,0)) > 0) 
	{
		msg[length] = '\0';
		printf(" Client id number - %d  \n",cl.sockno);
		sendtoall(msg,cl.sockno);
		printf(" The message from client id - %d is : %s  \n",cl.sockno,msg);
		memset(msg,'\0',sizeof(msg));

	}
	
	printf("The connection is lost\n");
	for(i = 0; i < n; i++)
	 {
		if(clients[i] == cl.sockno)
		 {
			j = i;
			while(j < n-1)
			 {
				clients[j] = clients[j+1];
				j++;
			}
		}
	}

	n--;
}



	



int main()

{
	int serverid = 0;
	int option =1;
	
	struct client_data cl;
	serverfd = socket(AF_INET,SOCK_STREAM,0);
	setsockopt(serverfd,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(option));
	memset(server.sin_zero,'\0',sizeof(server.sin_zero));
	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = inet_addr("127.0.0.1");

	new_size = sizeof(new);
	printf("Welcome,  I am the server using the local IP 127.0.0.1 with serverid 0 !! \n");


	if(bind(serverfd,(struct sockaddr *)&server,sizeof(server)) != 0) 
	{
		perror("error in binding process");
		exit(1);
	}

	if(listen(serverfd,5) != 0) 
	{
		perror("error in listening process");
		exit(1);
	}

	while(1) 
	{
		if((newfd = accept(serverfd,(struct sockaddr *)&new,&new_size)) < 0) {
			perror("error while accepting");
			exit(1);
		}
		
		printf("connection accepted from client - %d  \n",ntohs(new.sin_port) );

		cl.sockno = newfd;
		clients[n] = newfd;
		n++;
		pthread_create(&recvt,NULL,routine,&cl);

		
	}



		
	return 0;
}
