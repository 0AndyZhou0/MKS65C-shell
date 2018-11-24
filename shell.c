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
  char *temp;

  //Forking stuff
  pid_t child,pid;
  int status;
  while(1){
    printf("enter command\n");
    scanf("%[^\n]",line);
    
    args = parse_args(line);

    //Running process
    child = fork();
    if(child){
      //Parent Code
      pid = wait(&status);
    }else{
      //Child Code
      execvp(args[0], args);
    }
    scanf("%s",temp);
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

