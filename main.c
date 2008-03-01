
#include "io.h"

int main(){

  FILE *file;
  obra_info info;

  read_data(&info);
  file = fopen("base01.dat", "a");
  write_data(file, &info);
  fclose(file);

  return 0;
}
