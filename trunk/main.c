#include "menu.h"
#include "io.h"

int main(){

  FILE *file;
  obra_info info;
  char c;

  printWelcome();

  printMenu();
  
  while( scanf("%c", &c) ){
    getchar();
    switch(c){
    case '\n':
      break;
    case 'i':
      file = fopen("base01.dat", "a");
      read_data(&info);
      getchar();
      write_data(file, &info);
      printf("\nDeseja inserir mais uma entrada? (s)im, (n)ao? ");
      
      while( scanf("%c", &c) && c == 's'){
	getchar();
	read_data(&info);
	getchar();
	write_data(file, &info);
	printf("\nDeseja inserir mais uma entrada? (s)im, (n)ao? ");
      }
      getchar();
      fclose(file);
      break;
    case 's':
      return 0;
    }
    printMenu();    
  }
  
  return 0;
}
