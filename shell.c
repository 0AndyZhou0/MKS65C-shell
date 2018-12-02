#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/types.h>
#include <time.h>
#include "shell.h"

int main(int argc, char *argv[]){
  //Input stuff
  char ** commands;
  char line[99];
  char temp;
  char buffer[99];
  
  while(1){
    //gets USERNAME
    printf("%s:%s$",getenv("USERNAME"),getcwd(buffer,sizeof(buffer)));

    // Reads charcters in single line until new line
    scanf("%[^\n]",line);
    // Clears new line
    scanf("%c",&temp);

    //checks for semicolon
    if(strchr(line,';')){
      commands = run_semicolon(line);
    }else{
      run_command(line);
    }
  }
  return 0;
}

char ** run_semicolon(char * line){
  char * temp;
  while(line){
    temp = strsep(&line, ";");
    if(temp){
      run_command(temp);
    }
  }
}

char * white_out(char * string){
  char * no_space = string;
  int i = 0;
  int x = 0;
  while(string[i]){
    if(string[i] != ' '){
      no_space[x] = string[i];
      x++;
    }
    i++;
  }
  no_space[x] = 0;
  return no_space;
}

void run_command(char * command){
  //Forking stuff
  pid_t child,pid;
  int status;

  //Splits arguments of command
  //Removed counting
  char **args = (char**)malloc(99 * sizeof(char*));
  int i = 0;
  int arrow = 0;
  while(command){
    args[i] = strsep(&command, " ");
    if(args[i]){
      strcpy(args[i],white_out(args[i]));
	}
    if(args[i][0] == '\0'){
      i--;
    }
    i++;
  }
  args[i] = 0;
  //Hard Coded Commands
  if (!strcmp(args[0], "exit")) {exit(0);}  
  else if(!strcmp(args[0], "cd")){
    chdir(args[1]);
  }
  char output[100];
  for(int i =0;args[i]!='\0';i++){
          if(!strcmp(args[i],">") ||!strcmp(args[i],"<"))
        {      
            args[i]=NULL;
            strcpy(output,args[i+1]);
			arrow=i;
        }   
  }
  //Running process
  child = fork();
  if(child){
    //Parent Code
    pid = wait(&status);
  }else{
    //Child Code
	if(arrow){//runs if using arrows
		int fd1=open(output,O_CREAT|O_WRONLY, 0777);
		dup2(fd1, STDOUT_FILENO);
		close(fd1);
    }
	execvp(args[0], args);
	free(args);
	}
}
/*
  Old Counting Code
  int count = 1;
  char * temp = strchr(command,' ');
  while(temp != 0){
    temp = strchr(temp + 1,' ');
    count++;
  }

 */
