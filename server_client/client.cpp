//besic header files


#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<time.h>
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include <sys/ipc.h>
#include <sys/msg.h>
//msg queue

struct msg{ 
	long mesg_type; 
	char mesg_text[100]; 
} message; 


//required function 

//function to clear the screen 
#define clear() printf("\033[H\033[J")


//initialize
void init()
{
	   clear();         
}

void clean()
{
		clear();
}
int varify()  //verification before entering to server
{
	char user[5];
	printf("\nEnter Username : ");
	scanf("%s",user);
	if(strcmp(user,"root")==0)
	{
		printf("\nEnter Password : ");
		scanf("%s",user);
		if(strcmp(user,"toor")==0)
		{
			return 1;
		}
		return 0;
	}
	return 0;
}


int main()
{    
	//verification
	if(varify() == 1 )
	{
		init();
		//get a random msg type for this client
		
		int n = rand() % 100 + 1; //b/w 1  and 100
		
		key_t key; 
		int msgid; 

		// ftok to generate unique key 
		key = ftok("LServer", 65); 

		// msgget creates a message queue 
		// and returns identifier 
		msgid = msgget(key, 0666 | IPC_CREAT); 

		while(1)
		{
			message.mesg_type = 1; 

			printf(">> "); 
			fgets(message.mesg_text,100,stdin); 
			if(message.mesg_text == "clear")
			{
					clear();
			}
			// msgsnd to send message 
			msgsnd(msgid, &message, sizeof(message), 0); 
			 

		}


	}	
    	else{
		printf("\nusername or password is wrong");
	}
	
	
}
