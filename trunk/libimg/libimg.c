
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <png.h>
#include "jpeglib.h"
#include <gif_lib.h>
#include "libimg.h"
#include <math.h>

char CalculaDescritor(char* NomeImagem)
{
  char descritor=0;
  Hist h;
  double sr =0,sg =0,sb =0;
  double msr=0,msg=0,msb=0;
  double mr =0,mg =0,mb =0;
  double cmr=0,cmg=0,cmb=0;
  int maxr=0,maxg=0,maxb=0;
  int imr,img,imb;
  int i;
  Imagem* im=Imagem_le(NomeImagem);
  h=faz_hist(im);
  
  

  for (i=0;i<256;i++)
    {
      /*tudo com R*/
      if (maxr<h.r[i])
	{
	  maxr=h.r[i];
	  imr=i;
	}
      sr+=h.r[i];
      cmr+=h.r[i]*i;
      
      /*tudo com G*/
      if (maxg<h.g[i])
	{
	  maxg=h.g[i];
	  img=i;
	}
      sg+=h.g[i];
      cmg+=h.g[i]*i;
      
      /*tudo com B*/
      if (maxb<h.b[i])
	{
	  maxb=h.b[i];
	  imb=i;
	}
      sb+=h.b[i];
      cmb+=h.b[i]*i;

      /*meios*/
      if (i<128)
	{
	  msr+=h.r[i];
	  msg+=h.g[i];
	  msb+=h.b[i];
	}
    }

  mr=sr/256;
  cmr/=sr;

  mg=sg/256;
  cmg/=sg;

  mb=sb/256;
  cmb/=sb;

  if (imr<128)
    descritor=0x80;
  if (img<128)
    descritor=0x40;

  if ((msr/sr)>0.5)
    descritor|=0x20;
  if ((msg/sg)>0.5)
    descritor|=0x10;
  if ((msb/sb)>0.5)
    descritor|=0x08;

  if (cmr>128)
    descritor|=0x04;
  if (cmg>128)
    descritor|=0x02;
  if (cmb>128)
    descritor|=0x01;

  return descritor;
}
double ComputaSimilaridade(char* im1, char* im2)
{
  Hist h1,h2;
  double resposta;
  int i;
  double dr,dg,db;
  double rr,rg,rb;
  double sr1=0,sr2=0,sg1=0,sg2=0,sb1=0,sb2=0;
  Imagem* i1=Imagem_le(im1);
  Imagem* i2=Imagem_le(im2);
  
  h1=faz_hist(i1);
  h2=faz_hist(i2);
    
  libera_memoria(&i1);
  libera_memoria(&i2);

  rr=rg=rb=0;
  resposta=0;


  for (i=0;i<256;i++)
    {
      sr1+=h1.r[i];
      sr2+=h2.r[i];
      sg1+=h1.g[i];
      sg2+=h2.g[i];
      sb1+=h1.b[i];
      sb2+=h2.b[i];
    }

  for (i=0;i<256;i++)
    {
      dr=(h1.r[i]/sr1-h2.r[i]/sr2);
      dg=(h1.g[i]/sg1-h2.g[i]/sg2);
      db=(h1.b[i]/sb1-h2.b[i]/sb2);
      rr+=pow(dr,2.0);
      rg+=pow(dg,2.0);
      rb+=pow(db,2.0);
    }
  resposta=(sqrt(rr/2)+sqrt(rg/2)+sqrt(rb/2))/3;
  return 1-resposta;
}


NHist NormalizaHistograma(Hist h)
{
  double sr,sg,sb;
  NHist resposta;
  sr=sg=sb=0;
  int i;
  for (i=0;i<256;i++)
    {
      sr+=h.r[i];
      sg+=h.g[i];
      sb+=h.b[i];
    }
  for (i=0;i<256;i++)
    {
      resposta.r[i]=h.r[i]/sr;
      resposta.g[i]=h.g[i]/sg;
      resposta.b[i]=h.b[i]/sb;
    }
  return resposta;
}

/* Daqui pra baixo, reclamar com Victor Matheus :P */
Imagem* criar_img(int w, int h, int c){
  Imagem* resul;
  resul=(Imagem*)malloc(sizeof(Imagem));
  resul->w=w;
  resul->h=h;
  resul->c=c;
  resul->r=(int*)malloc(sizeof(int)*resul->h*resul->w);
  resul->g=(int*)malloc(sizeof(int)*resul->h*resul->w);
  resul->b=(int*)malloc(sizeof(int)*resul->h*resul->w);
  return resul;
}

char* strlwr( char* str ){ /*Transforma os caracteres alfabeticos maiusculos em minusculos*/
  int i = 0;
  int n = strlen(str);
  
  for (i=0; i < n; i++)
    {
      if ( isalpha(str[i]) )
	{
	  str[i] = tolower(str[i]);
	}
    }
  
  return str;
}

int InterlacedOffset[] = { 0, 4, 2, 1 }, InterlacedJumps[] = { 8, 8, 4, 2 }; // GIF - Para ler imagem interlaced

void libera_memoria(Imagem** im)
{
  free((*im)->r);
  free((*im)->g);
  free((*im)->b);
  (*im)->r=NULL;
  (*im)->g=NULL;
  (*im)->b=NULL;
  (*im)->c=0;
  (*im)->h=0;
  (*im)->w=0;
  free(*im);
  *im=0;
}

void escreve_pixel(Imagem* im,unsigned int x, unsigned int y,int r, int g, int b)
{
  int posicao=(im->w*y+x);
  
  if ((x<im->w)&&((y<im->h)))
    {
      im->r[posicao]=r;
      im->g[posicao]=g;
      im->b[posicao]=b;
    }
}
  
  void le_pixel(Imagem* im,unsigned int x,unsigned int y,int* r,int* g,int* b)
{
  int posicao=(im->w*y+x);

  if ((x<im->w)&&(y<im->h))
    {
      *r=im->r[posicao];
      *g=im->g[posicao];
      *b=im->b[posicao];
    }
}


Hist faz_hist(Imagem* im){
  int x, y, i;
  int r,g,b;
  Hist h;

  for (i=0; i<256;i++){
    h.r[i]=0;
    h.g[i]=0;
    h.b[i]=0;
  }

  for (y=0; y < im->h; y++){
    for (x=0; x < im->w; x++){
      le_pixel(im, x, y, &r, &g, &b);
      h.r[r]++;
      h.g[g]++;
      h.b[b]++;
    }
  }
  
  return h;

}



Imagem* Imagem_le(char* nome_arq)
{
  char ext[4];
  int i = strlen(nome_arq) - 1;
  
  ext[0] = nome_arq[i-2];
  ext[1] = nome_arq[i-1];
  ext[2] = nome_arq[i];
  ext[3] = '\0';

  if (strcmp(strlwr(ext),"png")==0){
    return le_png(nome_arq);
  }else if (strcmp(strlwr(ext),"jpg")==0){
    return le_jpeg(nome_arq);
  }else if (strcmp(strlwr(ext),"gif")==0){
    return le_gif(nome_arq);
  }else{
    fprintf(stderr, "Formato de arquivo %s desconhecido!", ext);
    return NULL;
  }

}



Imagem* le_png(char* nome_arq_png){
  png_structp png_ptr;
  png_infop info_ptr,end_info;
  FILE *p;
  png_bytep *row_pointers;
  Imagem* resul;

  int w,h;

  png_uint_32 width,height;
  int bit_depth,color_type,interlace_type,compression_type,filter_method;  

  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, (png_voidp)NULL,NULL,NULL);
  if (!png_ptr)
    return resul;
  
  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    {
      png_destroy_read_struct(&png_ptr,(png_infopp)NULL, (png_infopp)NULL);
      return resul;
    }
  
  end_info = png_create_info_struct(png_ptr);
  if (!end_info)
    {
      png_destroy_read_struct(&png_ptr, &info_ptr,(png_infopp)NULL);
      return resul;
    } 

  p=fopen(nome_arq_png,"rb");
  png_init_io(png_ptr,p);

  png_read_png(png_ptr, info_ptr,PNG_TRANSFORM_STRIP_ALPHA|PNG_TRANSFORM_PACKING,NULL);

  png_get_IHDR(png_ptr, info_ptr, &width, &height,&bit_depth, &color_type, &interlace_type,&compression_type, &filter_method);

  if (color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png_ptr);

  row_pointers = png_get_rows(png_ptr, info_ptr);

	resul = criar_img(width, height, png_get_channels(png_ptr,info_ptr) );

  for (h=0;h<resul->h;h++)
    for (w=0;w<resul->w;w++)
      escreve_pixel(resul,w,h,row_pointers[h][(3*w)],row_pointers[h][(3*w)+1],row_pointers[h][(3*w)+2]);
  
  fclose(p);
  png_destroy_read_struct(&png_ptr, &info_ptr,&end_info);
  return resul;
}


Imagem* le_jpeg(char* nome_arq_jpeg){
  
  struct jpeg_decompress_struct cinfo; /* contem os parametros de descompressao e informacoes da imagem */
  struct jpeg_error_mgr jerr; /* manipulador de excecoes */ 
  FILE * infile;		/* ponteiro para o arquivo */
  JSAMPARRAY buffer; /* buffer de saida (eh um ponteiro para uma scanline) */
  int i, j, num_colunas;
  int R, G, B;

  Imagem* resul;
  
  /* inicializa o manipulador de excecoes */
  cinfo.err = jpeg_std_error(&jerr);
  
  /* Abre o arquivo */
  if ((infile = fopen(nome_arq_jpeg, "rb")) == NULL) {
    fprintf(stderr, "O arquivo %s nao pode ser aberto\n", nome_arq_jpeg);
    return NULL;
  }
  
  /* Passo 1: aloca e inicializa o struct de descompressao JPEG */
  jpeg_create_decompress(&cinfo);

  /* Passo 2: especifica a fonte do arquivo */
  jpeg_stdio_src(&cinfo, infile);

  /* Passo 3: le os parametros do arquivo com a funcao jpeg_read_header() */
  (void) jpeg_read_header(&cinfo, TRUE);

  /* Step 4: Inicia descompressao */
  (void) jpeg_start_decompress(&cinfo);

  /* o numero de colunas eh igual a largura da imagem vezes o numero de bytes por pixel
   * (no caso de RGB, o numero eh 3) */
  num_colunas = cinfo.output_width * cinfo.output_components;

  resul = criar_img( cinfo.output_width,  cinfo.output_height, cinfo.output_components );

  /* Faz um vetor do tamanho necessario para uma linha da imagem */
  buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, num_colunas, 1);

  /* Passo 6: lê as informacoes de cada linha, armazena no buffer e imprime na tela */  
  for(i=0; i < cinfo.output_height; i++){
    (void) jpeg_read_scanlines(&cinfo, buffer, 1);
     for(j=0; j < num_colunas; j++){
       if(j%3 == 0 ){
	 if (j!=0){
	   escreve_pixel(resul,j/3-1,i,R,G,B);
	 }
	 R = buffer[0][j];
       }else if(j%3 == 1){
	 G = buffer[0][j];
       }else{
	 B = buffer[0][j];
       }
       
     }
     
     escreve_pixel(resul,j/3-1,i,R,G,B);
     
  }

  /* Passo 7: Finaliza descompressao */
  (void) jpeg_finish_decompress(&cinfo);

  /* Passo 8: Libera o espaco do struct de descompressao JPEG */
  jpeg_destroy_decompress(&cinfo);

  /* Fecha o arquivo */
  fclose(infile);
  
  return resul;

}



Imagem* le_gif(char *arquivo) {
   int i, j, Tamanho, Lin, Col, ExtCode;
   GifRecordType RecordType;
   GifByteType *Extension;
   GifFileType *oGif;
   GifRowType *oBuffer;
   ColorMapObject *Mapa;
   GifColorType *MapaCores;   
   Imagem* resul;

   if ((oGif = DGifOpenFileName(arquivo)) == NULL) {
     PrintGifError();
     return 0;
   }
   
   if ((oBuffer = (GifRowType *) malloc(oGif->SHeight * sizeof(GifRowType *))) == NULL) // Aloca a memoria necessaria para as linhas e colunas
      printf("Nao foi possivel alocar a memoria necessaria.");

   Tamanho = oGif->SWidth * sizeof(GifPixelType); // Tamanho em bytes de uma linha
   if ((oBuffer[0] = (GifRowType) malloc(Tamanho)) == NULL) // Aloca a primeira linha separadamente para facilitar a copia da cor de fundo
      printf("Nao foi possivel alocar a memoria necessaria.");

   for (i = 0; i < oGif->SWidth; i++) oBuffer[0][i] = oGif->SBackGroundColor; // Seta a cor da primeira linha para a cor do fundo
   for (i = 1; i < oGif->SHeight; i++) {  // Aloca as demais linhas ja com a cor do fundo
	  if ((oBuffer[i] = (GifRowType) malloc(Tamanho)) == NULL) printf("Nao foi possivel alocar a memoria necessaria.");
	  memcpy(oBuffer[i], oBuffer[0], Tamanho);
   }

   do { // Inicia o tratamento do arquivo
      if (DGifGetRecordType(oGif, &RecordType) == GIF_ERROR) { // Obtem o record type do bloco atual
         PrintGifError();
         return 0;
      }
      switch (RecordType) {
         case IMAGE_DESC_RECORD_TYPE: // Bloco de informacoes da imagem
            if (DGifGetImageDesc(oGif) == GIF_ERROR) {
   		       PrintGifError();
   		       return 0;
            }
            Lin = oGif->Image.Top;
            Col = oGif->Image.Left;
			
            if (oGif->Image.Interlace) { // Imagem em modo Interlaced precisa ser passada 4 vezes
               for (i = 0; i < 4; i++)
                  for (j = Lin + InterlacedOffset[i]; j < Lin + oGif->SWidth; j += InterlacedJumps[i]) {
                     if (DGifGetLine(oGif, &oBuffer[j][Col], oGif->SWidth) == GIF_ERROR) {
                        PrintGifError();
                        return 0;
                     }
   			   }
            } else { // Imagem em modo Normal
               for (i = 0; i < oGif->SHeight; i++) {
                  if (DGifGetLine(oGif, &oBuffer[Lin++][Col], oGif->SWidth) == GIF_ERROR) {
                     PrintGifError();
                     return 0;
                  }
               }
            }
            break;
   	   case EXTENSION_RECORD_TYPE: // Desconsidera qualquer bloco Extension na imagem
            if (DGifGetExtension(oGif, &ExtCode, &Extension) == GIF_ERROR) {
               PrintGifError();
               return 0;
            }
            while (Extension != NULL) {
               if (DGifGetExtensionNext(oGif, &Extension) == GIF_ERROR) {
                  PrintGifError();
                  return 0;
               }
            }
            break;
         case TERMINATE_RECORD_TYPE:
            break;
         default:
            break;
      }
   } while (RecordType != TERMINATE_RECORD_TYPE);

   Mapa = (oGif->Image.ColorMap) ? oGif->Image.ColorMap : oGif->SColorMap;

   resul = criar_img( oGif->SWidth,  oGif->SHeight, 3 );

   for (i = 0; i < resul->h; i++) { // Guarda os Pixels na struct Imagem
      for (j = 0; j < resul->w; j++) {
         MapaCores = &Mapa->Colors[oBuffer[i][j]];
	 	escreve_pixel(resul,j,i,MapaCores->Red,MapaCores->Green,MapaCores->Blue);
      }
   }
   
   if (DGifCloseFile(oGif) == GIF_ERROR) {
      PrintGifError();
      return NULL;
   }
   free(oBuffer);
   return resul;
}
