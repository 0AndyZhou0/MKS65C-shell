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
  /* We don't need this part for a shell
     if(argc > 1){
     char * args[argc];
     for(int i = 0;i < argc - 1;i++){
     args[i] = argv[i+1];
     }
     args[argc - 1] = '\0';
     execvp(args[0], args);
     }
  */

  //Input stuff
  char ** args;
  char line[99];
  char temp;
  char buffer[99];
  //Forking stuff
  pid_t child,pid;
  int status;

  
  while(1){
	//USERNAME stuff too hard to find for now
    printf("%s:%s$",getenv("USERNAME"),getcwd(buffer,sizeof(buffer)));

    // Reads charcters in single line until new line
    scanf("%[^\n]",line);
    // Clears new line
    scanf("%c",&temp);
	
    args = parse_args(line);
	//Hard Coded Commands
	if (!strcmp(args[0], "exit")) {exit(0);}  
	else if(!strcmp(args[0], "cd")){
			chdir(args[1]);
	}
    //Running process
    child = fork();
    if(child){
      //Parent Code
      pid = wait(&status);
    }else{
      //Child Code
      execvp(args[0], args);
    }
  }
  return 0;
}

char ** parse_args( char * line ){
  int count = 1;
  char * temp = strchr(line,' ');
  while(temp != 0){
    count++;
    temp = strchr(temp + 1,' ');
  }
  char **args = (char**)malloc(count * sizeof(char*));
  for(int i = 0;i < count + 1;i++){
    args[i] = strsep(&line, " ");
  }
  return args;
}

