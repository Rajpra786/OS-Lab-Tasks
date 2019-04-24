//besic header files
#include <iostream>
#include<stdio.h> 
#include<string.h>
#include<string> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/resource.h>

//for priority

#include <sys/time.h>
#include <sys/resource.h>


//msg queue
using namespace std;
struct msg { 
	long mesg_type; 
	char mesg_text[100]; 
} message; 


//fun to clear the screen 
#define clear() printf("\033[H\033[J")


//initialize
void init()
{

	printf("******************************************************\n");
	char* user = getenv("USER"); //current user
	printf("\n\t\tHello : @%s", user);
	char cwd[1024]; 
	getcwd(cwd, sizeof(cwd)); // to get current directory
	printf("\nYou can access the  following directory anywhere by giving the following command command in terminal\n");
	printf("\nserver directory : %s\n",cwd);
	printf("command to access it : client\n");
	printf("\n***********************************************************************\n");
	
	//compile client and put in /usr/local/sbin
	system("g++ client.cpp -o client");
	system("sudo cp client /usr/local/sbin");
	/*
	execute client and paste it to bin folder
	sudo rm -rf /usr/bin/ser
	cp ser /usr/local/sbin 

	*/
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

void help()
{

	printf("***************************\tbasic commands ***************************************\n");
	printf("play v_name : to play v_name video file\n");
	printf("open f_name : to see the contents of f_name file\n");
	printf("cpro f_name : to execute c program\n");
	printf("exec command : to execute any command \n");
	printf("mail e-mail file_name : to execute any command \n");

}


void play_video(char *name) 
{ 
	pid_t pid = fork(); 

	if (pid == -1) { 
		printf("\nWent wrong"); 
		return; 
	} else if (pid == 0) { 
		id_t p_id;
		p_id = getpid();
		cout<<p_id<<endl;

		int x = setpriority(PRIO_PROCESS, p_id ,10);
		if(!x)
		{
			int i;
			std::string com;
			std::string s(name);
			com = "vlc " + s;
			const char *command = com.c_str();
			if (system(command)< 0) { 
				printf("\n Something went wrong"); 
			} 
			
			// cout<<"video is playing\n";
			_exit(0);
		}
		else{
			cout<<"error to priority"<<endl;
		}
		 
	} 
	clear();
	cout<<"video is playing\n";
	return ;
}

void exec_pro(string file_name)
{
	pid_t pid = fork(); 
	
	// clear();

	if (pid == -1) { 
		printf("\nFailed forking child.."); 
		return; 
	} else if (pid == 0) { 
		id_t p_id;
		p_id = getpid();
		cout<<p_id<<endl;

		int x = setpriority(PRIO_PROCESS, p_id ,2);
		if(!x)
		{
			string str = "g++";
			str = str + " -o a.out "+ file_name;
			const char *command = str.c_str();
			system(command);
			system("./a.out");
			_exit(0); 
		}
		else{
			cout<<"error to priority"<<endl;
		}

	return ;	
	} 
	
}

void exec_cmd(int nc ,char **cmd) 
{ 
	// Forking a child 
	// clear();

	pid_t pid = fork(); 
	
	if (pid == -1) { 
		printf("\nFailed forking child.."); 
		return; 
	} else if (pid == 0) { 

		id_t p_id;
		p_id = getpid();
		cout<<p_id<<endl;

		int x = setpriority(PRIO_PROCESS, p_id ,0);
		if(!x)
		{
			string str = cmd[0];
			for(int i=1;i<nc;i++)
			{
				str = str +" " + cmd[i];	
			}
			cout<<str<<endl;
			const char *command = str.c_str();
			system(command);//mand
			_exit(0); 
		}
		else{
			cout<<"error to priority"<<endl;
		}
	}

} 

void send_mail(string mail,string file)
{
	pid_t pid = fork(); 
	
	if (pid == -1) { 
		printf("\nFailed forking child.."); 
		return; 
	} else if (pid == 0) {
		id_t p_id;
		p_id = getpid();
		cout<<p_id<<endl;

		int x = setpriority(PRIO_PROCESS, p_id , 5);
		if(!x)
		{
			// clear();
			cout<<"*********************Sending mail..."<<endl;
			string str="ssmtp ";
			str = str + mail +" < "  +file;
			const char *command = str.c_str();
			system(command);
			cout<<"Mail sent."<<endl;
			_exit(0);
		}
		else{
			cout<<"error to priority"<<endl;
		}
		
	return;
	}

}

void open_file(string file)
{
	pid_t pid = fork(); 
	
	if (pid == -1) { 
		printf("\nFailed forking child.."); 
		return; 
	} else if (pid == 0) {

		id_t p_id;
		p_id = getpid();
		cout<<p_id<<endl;

		int x = setpriority(PRIO_PROCESS, p_id ,3);
		if(!x)
		{
			cout<<"Opening "<<file<<endl;
			cout <<"please wait"<<endl;
			string str = "code";
			str = str + " "+ file;
			const char *command = str.c_str();
			system(command);
			// clear();
			cout<<"file open done"<<endl;
			_exit(0);
		}
		else{
			cout<<"error to priority"<<endl;
		}
		return; 
	}
}
int main()
{    
	//verification
	if(varify() == 1 )
	{
		init();
		key_t key; 
		int msgid; 

		// ftok to generate unique key 
		key = ftok("LServer", 65); 

		// msgget creates a message queue 
		// and returns identifier 
		msgid = msgget(key, 0666 | IPC_CREAT); 

		while(1)
		{
			// msgrcv to receive message 
			msgrcv(msgid, &message, sizeof(message), 1, 0); 

			char  *argv[64];    //arguments in command
			char *main_command[1000]; 
			int i;

			char *ptr = strtok(message.mesg_text," ");
			i=0;
			while(ptr !=NULL)
			{
				argv[i] = ptr;
				i++;
				ptr = strtok(NULL," ");
			}
			char *command = argv[0];
			command[4]='\0';
			int cmd = 0;
			if(strcmp(command,"play")==0)
				cmd=1;
			else if(strcmp(command,"cpro")==0)
				cmd = 2; 
			else if(strcmp(command,"open")==0)		
				cmd = 3;
			else if(strcmp(command,"mail")==0)		
				cmd = 4;
			else if(strcmp(command,"exec")==0)		
				cmd = 5;
			printf("%s\n",command);

			switch(cmd)
			{
				case 1:
				{
					char *name = argv[1];
					play_video(name);
					break;
				}
				case 2:
				{
					exec_pro(argv[1]);
					break;
				}
				case 3:
					open_file(argv[1]);
					break;
				
				case 4:{
					send_mail(argv[1],argv[2]);
					break;
				}
				case 5:
				{
					char *comm[10];
					char *x=argv[1];
					long nc=0;
					for(i=0;x!=NULL;i++)
					{
						comm[i] = x;
						std::cout<<comm[i]<<std::endl;
						x=argv[i+2];
						if(x == "|")
						{
							nc++;
						}
					}
					comm[i]=NULL;
					
					exec_cmd(i,comm);
					break;
				}
				default:
				{
					printf("Help \n");
					clear();
					help();
					break;
				}
					

			}
						

		}

		
	}	
    	else{
		printf("\nusername or password is wrong");
	}
}











