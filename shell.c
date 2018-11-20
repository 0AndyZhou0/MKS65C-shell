#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "shell.h"

int main(){
  char line[] = "ls -l -a";
  char ** args = parse_args(line);
  execvp(args[0], args);
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

