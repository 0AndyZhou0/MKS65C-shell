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
  if(argc > 1){
    char * args[argc];
    for(int i = 0;i < argc - 1;i++){
      args[i] = argv[i+1];
    }
    args[argc - 1] = '\0';
    execvp(args[0], args);
  }else{
    char ** args;
    char *line;
    printf("enter command\n");
    scanf("%99[^\n]",line);
    args = parse_args(line);
    execvp(args[0], args);
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

