#include <stdio.h>
#include <string.h>

int main(void) {
  char * dog=malloc(sizeof(char) * 100);
  char * modify;
  char * fish = "ls | grep main";
  char * convult= "> something";
  int hold = strlen(convult);
  printf("%d",hold);
  int moved =0;
  //strcpy(dog,fish);
    for(int i = 0; i < strlen(fish) ; i++ ){
      if(fish[i]=="|"){
          for(int j = 0; j < hold;j++){
            dog[i+j]=convult[j];
          }
          puts(dog);
          moved=1;
      }
      else{
        if(moved){
        dog[i+hold]=fish[i];
        }
        else{
      dog[i]=fish[i];
        }
      }
  //printf("%c\n",fish[i]);
  printf("%c\n",dog[i]);
  }
puts(dog);
  return 0;
}

