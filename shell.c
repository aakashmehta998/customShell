#include "components/header.h"
int main()
{
  char* buff=(char*)malloc(sizeof(char)*1024);
  buff=getcwd(NULL,0);
  startup();
  char inputString[MAXCOM], *parsedArgs[MAXLIST];
  FILE *fptr;
  char filename[100], c;
  char* parsedArgsPiped[MAXLIST];
  int isEntry = 1;
  int isExit = 1;
  bool loop = true;
  char commandName[100][100];
  char time1[100][100];
  char date1[100][100];
  glob_var = mmap(NULL,sizeof *glob_var, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS , -1 , 0 );
  pid_t pid=1;
  while(*glob_var==0)
  {
    if(isEntry==0)
    {
      printf("myshell>>>>");
    }
	  char command[100];
    char* argv[64];
    gets(command);
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(date1[count],"%d-%d-%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    sprintf(time1[count],"%d:%d:%d",tm.tm_hour, tm.tm_min, tm.tm_sec);
    strcpy(commandName[count],command);
    char com[10][100];
    for(int p=0;p<10;p++)
    {
      com[p][0] = NULL;
    }
    int n=0,i,j=0;
    for(i=0;5>0;i++)
    {
      if(command[i]!=' ')
      {
        com[n][j++] = command[i];
      }
      else
      {
        com[n][j++] = '\0';
        n++;
        j=0;
      }
      if(command[i]=='\0')
      {
        break;
      }
    }
    if(strcmp(com[0],"exit")==0)
     {
      printf("Exiting shell..........\n");
      *glob_var = 1;
      loop = false;
      isExit = 0;
      count++;
      continue;
     }
     else if(strcmp(com[0],"entry")==0 && isEntry==1)
     {

       isEntry = 0;
       printf("*****************Shell started****************\n");
       sprintf(status[count],"success");
       count++;
       continue;
     }
     else if(isEntry==1)
     {
       printf("'entry' command expected\n");
       sprintf(status[count],"fail");
       count++;
       continue;
     }
     if(strcmp(com[1],"|")==0 && strcmp(com[0],"ls")==0 && strcmp(com[2],"grep")!=0)
     {
       stringProcess(command,parsedArgs, parsedArgsPiped);
       int val = executePipe(parsedArgs, parsedArgsPiped);
       if(val == 1 || val==2)
       {
         sprintf(status[count],"success");
       }
       else
       {
         sprintf(status[count],"fail");
       }
       sprintf(commandName[count],"%s",com[0]);
       count++;
       sprintf(commandName[count],"%s",com[2]);
       if(val == 1)
       {
         sprintf(status[count],"success");
       }
       else
       {
         sprintf(status[count],"fail");
       }
       sprintf(date1[count],"%d-%d-%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
       sprintf(time1[count],"%d:%d:%d",tm.tm_hour, tm.tm_min, tm.tm_sec);
       count++;
       continue;
     }
    pid = fork();
    if(pid < 0)
    {
      printf("failed");
      exit(1);
    }
    else if(pid == 0)
    {
    	if(strcmp(com[0],"ls")==0 && isEntry==0 && strcmp(com[1],"|")!=0)
	     {
         if(strcmp(com[1],"")==0)
         {
           bool val = listDirectory();
           if(val == true)
           {
             sprintf(status[count],"success");
           }
           else
           {
             sprintf(status[count],"fail");
           }
         }
         else if(strcmp(com[1],"|")!=0)
         {
           parse(command,argv);
           bool val = listDirectory1(argv);
           if(val == true)
           {
             sprintf(status[count],"success");
           }
           else
           {
             sprintf(status[count],"fail");
           }
         }
	     }
       else if(strcmp(com[0],"find")==0 && isEntry==0)
       {
         parse(command,argv);
         bool val = listDirectory1(argv);
         if(val == true)
         {
           sprintf(status[count],"success");
         }
         else
         {
           sprintf(status[count],"fail");
         }
       }
       else if(strcmp(com[0],"history")==0 && isEntry==0)
       {
         sprintf(status[count],"success");
         printf("No\t command name\t\t\t Date\t\t\t Time\t\t Status\n");
         int i;
         if(count>10)
         {
           i = count - 9;
         }
         else
         {
           i = 1;
         }
         int j = 1;
         for(;i<=count;i++)
         {
           printf("%d\t %-30s\t %s\t\t %-10s\t %s\n",j++,commandName[i],date1[i],time1[i],status[i]);
         }
       }
      else if(strcmp(com[0],"help")==0 && isEntry==0)
	     {
         sprintf(status[count],"success");
         printf("ls / ls [argument] \t\t\t list all files\n");
         printf("help               \t\t\t information about all commands\n");
         printf("pwd                \t\t\t present working directory\n");
         printf("cd [argument]      \t\t\t to change directory\n");
         printf("history            \t\t\t show latest 10 commands history\n");
         printf("man [argument]     \t\t\t manual for each command\n");
         printf("find [argument]    \t\t\t finds file in present working directory\n");
	 printf("ls | find [argument])  \t\t\t find file in current working directory\n");
         printf("exit               \t\t\t exit from shell\n");
	     }
      else if(strcmp(com[0],"exit")==0 && isEntry==0)
	     {

	     }
       else if(strcmp(com[0],"man")==0 && isEntry==0)
       {
         strcpy(filename,buff);
         strcat(filename,"/manual/");
         strcat(filename,com[1]);
         strcat(filename,".txt");
         fptr = fopen(filename, "r");
         if (fptr == NULL)
         {
	   sprintf(status[count],"fail");
           printf("manual file for %s is not exists \n",com[1]);
	   printf("value of count: %s\n",status[count]);
           exit(0);
         }
         c = fgetc(fptr);
         while (c != EOF)
         {
           printf ("%c", c);
           c = fgetc(fptr);
         }
         sprintf(status[count],"success");
         fclose(fptr);
       }
       else if(strcmp(com[0],"pwd")==0 && isEntry==0)
       {
         if(strcmp(com[1],"")==0)
        {
          presentWorkingDirectory();
          sprintf(status[count],"success");
        }
       }
       else if(strcmp(com[0],"cd")==0 && isEntry==0)
       {
         if(strcmp(com[1],"")==0)
         {
           printf("destination is expected\n");
           sprintf(status[count],"fail");
         }
         else
         {
           bool val = changeDirectory(com[1]);
           if(val == true)
           {
             sprintf(status[count],"success");
           }
           else
           {
             sprintf(status[count],"fail");
           }
         }
       }
       else if(strcmp(com[0],"entry")==0 && isEntry==0)
       {
         printf("Shell is already started\n");
         sprintf(status[count],"fail");
       }
      else if(isEntry==0)
	     {
        printf("Not recognized comamand..... type command \"help\" for help\n");
        sprintf(status[count],"fail");
	     }

    }
    else
    {
      wait(NULL);
    }
       count++;

  }
	return 0;

}
