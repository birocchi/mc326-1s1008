#include <stdio.h>
#include <stdlib.h>
#include "libimg.h"

int main(int argc,char *argv[])
{

  if (argc < 2){
    fprintf(stderr,"Cade o nome da imagem???\n");
    return 1;
  }

  if (argc==2)
    {
      /*Le a imagem do disco*/
      Imagem* im=Imagem_le(argv[1]);
      Hist h;
      
      /*Tamanho da imagem*/
      
      printf("w:%d h:%d c:%d\n",im->w,im->h,im->c);
      
      h = faz_hist(im);
      
      /*      printf("\nHistograma:\nn\tR\tG\tB\n");
      for (i=0;i<256;i++){
	printf("%d\t%d\t%d\t%d\n",i,h.r[i],h.g[i],h.b[i]);
	}*/
      
      libera_memoria(&im);
      
      /*      fhist = fopen("histograma.dat","w");
      
      for (i=0;i<256;i++){
	fprintf(fhist, "%d %d %d\n",h.r[i],h.g[i],h.b[i]);
      }  
      
      fclose(fhist);
      */
      printf("Descritor: (hexa) %X\n",CalculaDescritor(argv[1]));
    }
  if (argc==3)
    {
      printf("Similaridade %f\n",ComputaSimilaridade(argv[1],argv[2]));
    }

return 0;

}
