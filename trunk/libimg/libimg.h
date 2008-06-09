/* victor matheus de araujo oliveira
   Ra072589
   grupo 10
   disciplina MC326 - estrutura de arquivos
   15/03/2008
   
   Biblioteca de manipulacao de imagens JPG, GIF e PNG

*/

#ifndef LIBIMG_H
#define LIBIMG_H

/* Definicao do tipo Imagem */
typedef struct imagem
{
  /* Largura, altura e numero de canais */
  unsigned int w,h,c;

  /* Dados: NUNCA ACESSE DIRETAMENTE - use le_pixel/escreve_pixel */
  int* r;
  int* g;
  int* b;
}Imagem;

typedef struct hist
{
  unsigned int r[256];
  unsigned int g[256];
  unsigned int b[256];
}Hist;

typedef struct nh
{
  double r[256];
  double g[256];
  double b[256];
} NHist;

Imagem* Imagem_le(char* nome_arq_png);

NHist NormalizaHistograma(Hist h);

Imagem* le_png(char*);
Imagem* le_jpeg(char*);
Imagem* le_gif(char*);

Hist faz_hist(Imagem*);
unsigned char CalculaDescritor(char* NomeImagem);
double ComputaSimilaridade(char* im1,char* im2);

void libera_memoria(Imagem** im);

void le_pixel(Imagem* im,unsigned int x,unsigned int y,int* r,int* g,int* b);

void escreve_pixel(Imagem* im,unsigned int x, unsigned int y,int r, int g, int b);

#endif
