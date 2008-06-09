#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include "mem.h"

typedef struct {
  char *fp_name;
  FILE *fp;
  unsigned int loaded_file;
} Descriptor;

typedef struct {
  double similarity;
  char img[IMG_LENGTH + 1];
} SimilarityRecord;

typedef struct {
  size_t length;
  SimilarityRecord *list;
} SimilarityList;

static int
simlist_compare (void *a, void *b)
{
  double sa = (((SimilarityRecord *)a)->similarity);
  double sb = (((SimilarityRecord *)b)->similarity);

  if (sa < sb)
    return 1;
  else if (sb > sb)
    return -1;
  else
    return 0;
}

static void
change_hash_file (Descriptor *desc, unsigned int hashnum)
{
  char *filename;

  if (hashnum != desc->loaded_file)
    {
      if (desc->loaded_file != -1)
        {
          flush_to_disk (desc);
          fclose (desc->fp);
        }

      filename = hash_get_filename (desc->fp_name, hashnum);

      desc->fp = fopen (filename, "r+");
      desc->loaded_file = hashnum;

      free (filename);
    }
}

MemoryIndex *
memory_index_get_previous (MemoryIndex * index, char *key)
{
  char *filename;
  MemoryIndex *mindex;
  unsigned int hashnum;

  hashnum = index->hash_function (key);

  if (hashnum == 0)
    return NULL;

  filename = hash_get_filename (index->fp_name, hashnum - 1);

  mindex = memory_index_new_with_hash (index->fp_name, index->hash_function);
  load_file (mindex, filename);

  free (filename);

  return mindex;
}

void
descriptor_find (Descriptor *desc, const char *imgname, MemoryIndex *pk, FILE
  *base_fp, size_t maxresults)
{
  char di, ds;
  char imgfile[IMG_LENGTH + 1];
  char pkname[TITLE_LENGTH + 1];
  MemoryIndexRecord *match;
  SimilarityList *simlist = simlist_new ();
  unsigned int curload;

  assert (desc);

  if (!isValidFile (imgname))
    return;

  ds = CalculaDescritor (imgname);

  curload = desc->loaded_file;

  /* Quebra isso em outra funcao, repete pra V e Vn+1 */
  if ((curload - 1) >= 0)
    {
      change_hash_file (desc, curload - 1);

      while (!feof (desc->fp))
        {
          di = fgetc (desc->fp);
          fgets (pkname, TITLE_LENGTH + 1, base_fp);

          if ((di ^ ds) <= 2)
            {
              match = memory_index_find (pk, pkname);
              if (match)
                {
                  fseek (base_fp, (match->rrn * BASE_REG_SIZE) +
                    (BASE_REG_SIZE - IMG_LENGTH), SEEK_SET);

                  fgets (imgfile, IMG_LENGTH + 1, base_fp);

                  simlist_append (simlist, imgfile, CalculaSimilaridade
                    (imgfile, imgname));
                }
              else
                {
                  fseek (base_fp, BASE_REG_SIZE, SEEK_CUR);
                }
            }
        }
    }

  if (simlist->length == 0)
    {
      simlist_free (simlist);

      printf ("   Nao ha imagens semelhantes a \"%s\".\n", imgname);
      return;
    }
  else
    {
      qsort (simlist, simlist->length, sizeof (SimilarityList), simlist_compare);

      for (i = 0; (i < maxresults) && (i < simlist->length); i++)
        {
          /* html_output blablabla */
        }
    }
}

void
descriptor_insert (Descriptor *desc, const char *pkname, char d)
{
  char *filename;
  unsigned int hashnum;

  assert (desc);

  change_hash_file (desc, descriptor_hash (d));

  fseek (desc->fp, 0, SEEK_END);
  fprintf (desc->fp, "%c%-200s", d, pkname);
}

Descriptor *
descriptor_new (const char *fp_name)
{
  Descriptor *desc = MEM_ALLOC (Descriptor);

  d->fp_name = NULL;
  d->fp = NULL;
  d->loaded_file = -1;

  return d;
}
