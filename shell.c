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
  int count = 1;
  char * temp = strchr(line,';');
  while(temp != 0){
    count++;
    temp = strchr(temp + 1,';');
    //while(temp[0] = ' '){
    //  temp++;
    //}
  }
  char **args = (char**)malloc((count) * sizeof(char*));
  for(int i = 0;i < count + 1;i++){
    args[i] = strsep(&line, ";");
    char ** commands = parse_args(args[i]);
    run_command(commands[i]);
  }
  free(args);
}

char ** parse_args(char * line){
  int count = 1;
  char * temp = strchr(line,' ');
  while(temp != 0){
    temp = strchr(temp + 1,' ');
    count++;
  }
  char **args = (char**)malloc((count) * sizeof(char*));
  for(int i = 0;i < count + 1;i++){
    args[i] = strsep(&line, " ");
    strcmp(args[i],white_out(args[i]));
    //Double Spaces are broken.
  }
  return args;
}

char * white_out(char * string){
  while(string){
    if(string[0] != ' '){
      return string;
    }
    string++;
  }
}

void run_command(char * command){
  char ** args;

  //Forking stuff
  pid_t child,pid;
  int status;

  args = parse_args(command);
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
  free(args);
}
