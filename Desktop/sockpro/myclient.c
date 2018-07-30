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


	struct sockaddr_in newc;
	int clientfd;
	int newcfd;
	int newc_size;
	int port;
	pthread_t sendt,recvt;
	char msg[500];
	int len;
	struct Node* head = NULL;
	int mask;


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




void *routine(void *sock)
	{
	int newcfd = *((int *)sock);
	int length;
	int rcvid;

	while((length = recv(newcfd,msg,500,0)) > 0) 
		{
			msg[length] = '\0';
			printf("The message forwarded from the server is : %s \n",msg);
			int t = (int)msg[0];
			
			if(t == 49)
			{
				add(&head,ntohs(newc.sin_port));
			}
			else if(t == 50)
			{
				
				delete(&head,ntohs(newc.sin_port));
				printf(" The updated table \n"); 
				printList(head);
			}
			memset(msg,'\0',sizeof(msg));
		}
	}




int main()
{
	int serverid = 0;
	
	clientfd = socket(AF_INET,SOCK_STREAM,0);
	memset(newc.sin_zero,'\0',sizeof(newc.sin_zero));
	newc.sin_family = AF_INET;
	
	newc.sin_port = htons(PORT);
	newc.sin_addr.s_addr = inet_addr("127.0.0.1");


	if(connect(clientfd,(struct sockaddr *)&newc,sizeof(newc)) < 0) 
	{
		perror("error in establishing the connection \n");
		exit(1);
	}

	printf("This client is connected to the server\n");
	add(&head,serverid);
	pthread_create(&recvt,NULL,routine,&clientfd);
	printf("enter the number of your choice\n 1.add in the table\n  2.delete in the table\n");
	while(fgets(msg,500,stdin) > 0)
		{
		len = write(clientfd,msg,500);
		printf("Sending the message.......\n");
		if(len < 0) 
		{
			perror("Data transfer failed   \n");
			exit(1);
		}
		memset(msg,'\0',sizeof(msg));
		
	}

	pthread_join(recvt,NULL);
	close(clientfd);

return 0;

}