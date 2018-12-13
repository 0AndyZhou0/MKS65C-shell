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
    //printf("%s:%s$",getenv("USERNAME"),getcwd(buffer,sizeof(buffer)));

    // Reads charcters in single line until new line
    scanf("%[^\n]",line);
    // Clears new line
    scanf("%c",&temp);

    //checks for semicolon
    if(strchr(line,';')){
      commands = run_semicolon(line);
    }else if(strchr(line,'|')){
      commands = run_pipe(line);
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
      if(strchr(temp,'|')){
	run_pipe(temp);
      }
      else{
	run_command(temp);
      }
    }
  }
}
char** parse_args(char* line, char* parse_at) {
  char** args = calloc(sizeof(char*), 101);
  int index = 0;
  char* token;
  while (line != NULL) {
    token = strsep(&line, parse_at);
    // ignores extra spaces
    if (strcmp(token, "")) {
      args[index] = token;
      index++;
    }
  }
  return args;
}
char ** run_pipe(char * line){
	char** args = calloc(sizeof(char*), 101);
 if (line[0] == '|') {
    printf("| : That looks a lot like not a command xD\n");
    return args;
  }
  if (!strchr(line, '|')) { return 0; }
  if (!fork()) {
    int fds[2];
    pipe(fds);

    char** args = parse_args(line, "|");

    char** left = parse_args(args[0], " ");
    char** right = parse_args(args[1], " ");

    if (fork()) {
      close(fds[0]);
      dup2(fds[1], STDOUT_FILENO);
      execvp(left[0], left);
      return args;
    }
    else {
      wait(NULL);
      close(fds[1]);
      dup2(fds[0], STDIN_FILENO);
      execvp(right[0], right);
      return args;
    }
  }
  else {
    wait(NULL);
    return args;
  }
  return args;
}
  /*while(line && strchr(line,'|')){
    temp = strsep(&line, "|");
    strcpy(backup,line); // backsup line because strcat overwrites line
    if(temp){
      if(start == 0){
	strcat(temp," > something");
	run_command(temp);
	start++;
      }else{
	int backupfile = dup(STDOUT_FILENO);	
	int file = open("something2", O_CREAT | O_RDWR, 0777);
	dup2(file,1);
	strcat(temp," < something");
	run_command(temp);
	dup2(backupfile, 1);
	close(file);

	//Moving contents of file
	FILE * file1 = fopen("something2", "r");
	FILE * file2 = fopen("something", "w");
	int c;
	while((c = fgetc(file1)) != EOF){
	  fputc(c, file2);
	}
	fclose(file1);
	fclose(file2);
      }
      strcpy(line,backup);
    }
  }
  if(line){
    //printf("%s\n",line);
    strcat(line," < something");
	strcat(line,"; rm something");
    run_semicolon(line);
  }
  //char * args[] = {"rm", "rm something"};
  //execvp(args[0], args);
}*/
/*
	char ** run_arrow(char * line, char * arrow){
		char output[100];
		char input[100];
		for(int i =0;args[i];i++){ //goes through args finding any arrows     
		if(!strcmp(args[i],">")){
		  args[i]=NULL;
		  strcpy(output,args[i+1]);
		  rightArr=i;
		}
		else if(!strcmp(args[i],"<")){
		  args[i]=NULL;
		  strcpy(input,args[i+1]);
		  leftArr=i;
		}   
	  }
		if(leftArr){//runs if using left arrow
		  int fd0=open(input,O_RDONLY);
		  dup2(fd0, STDIN_FILENO);
		  close(fd0); 
		}
		if(rightArr){//runs if using right arrow
		  int fd1=open(output,O_CREAT|O_WRONLY, 0777);
		  dup2(fd1, STDOUT_FILENO);
		  close(fd1);
		}
	}
	*/
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
  if (!strcmp(args[0], "exit")) {exit(0); return ;}  
  else if(!strcmp(args[0], "cd")){
    chdir(args[1]);
	return ;
  }
  
  char output[100];
  char input[100];
  int leftArr = 0;
  int rightArr =0;
  for(int i =0;args[i];i++){ //goes through args finding any arrows     
    if(!strcmp(args[i],">")){
      args[i]=NULL;
      strcpy(output,args[i+1]);
      rightArr=i;
    }
    else if(!strcmp(args[i],"<")){
      args[i]=NULL;
      strcpy(input,args[i+1]);
      leftArr=i;
    }   
  }
  //Running process
  child = fork();
  if(child){
    //Parent Code
    pid = wait(&status);
  }else{
    //Child Code
    if(leftArr){//runs if using left arrow
      int fd0=open(input,O_RDONLY);
      dup2(fd0, STDIN_FILENO);
      close(fd0); 
    }
    if(rightArr){//runs if using right arrow
      int fd1=open(output,O_CREAT|O_WRONLY, 0777);
      dup2(fd1, STDOUT_FILENO);
      close(fd1);
    }
    execvp(args[0], args);
  }
  free(args);
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
