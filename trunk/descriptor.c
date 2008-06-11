#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "base.h"
#include "descriptor.h"
#include "file.h"
#include "filelist.h"
#include "hash.h"
#include "html.h"
#include "io.h"
#include "libimg/libimg.h"
#include "mem.h"
#include "memindex.h"

static void simlist_append (SimilarityList * simlist, ArtworkInfo artwork,
                            double sim);
static int simlist_compare (const void *a, const void *b);
static void simlist_free (SimilarityList * simlist);
static void simlist_inflate (SimilarityList * simlist, size_t newsize);
static SimilarityList *simlist_new (void);

static void change_hash_file (Descriptor * desc, unsigned int hashnum);
static void find_similarities (Descriptor * desc, SimilarityList * simlist,
                               char *imgname, unsigned char ds,
                               MemoryIndex * pk, Base * base, int hashnum);
static unsigned int descriptor_hash (unsigned char key);
static unsigned int descriptor_hash (unsigned char key);

static void
simlist_append (SimilarityList * simlist, ArtworkInfo artwork, double sim)
{
  if (simlist->regnum == simlist->maxregs)
    simlist_inflate (simlist, simlist->maxregs * 2);

  simlist->list[simlist->regnum].artwork = artwork;
  simlist->list[simlist->regnum].similarity = sim;

  simlist->regnum++;
}

static int
simlist_compare (const void *a, const void *b)
{
  double sa = (((SimilarityRecord *) & a)->similarity);
  double sb = (((SimilarityRecord *) & b)->similarity);

  if (sa < sb)
    return 1;
  else if (sb > sb)
    return -1;
  else
    return 0;
}

static void
simlist_free (SimilarityList * simlist)
{
  if (simlist)
    {
      free (simlist->list);
      free (simlist);
    }
}

static void
simlist_inflate (SimilarityList * simlist, size_t newsize)
{
  SimilarityRecord *tmp;

  if (newsize == 0)
    newsize = 50;

  if (newsize > simlist->maxregs)
    {
      tmp = realloc (simlist->list, newsize * sizeof (SimilarityRecord));
      assert (tmp);

      simlist->list = tmp;
      simlist->maxregs = newsize;
    }
}

static SimilarityList *
simlist_new (void)
{
  SimilarityList *ret = MEM_ALLOC (SimilarityList);

  ret->list = NULL;
  ret->regnum = 0;
  ret->maxregs = 0;

  return ret;
}

static void
change_hash_file (Descriptor * desc, unsigned int hashnum)
{
  char *filename;

  if (hashnum != desc->loaded_file)
    {
      if (desc->loaded_file != -1)
        fclose (desc->fp);

      filename = hash_get_filename (desc->fp_name, hashnum, DESC_HASH_NUM);

      desc->fp = fopen (filename, "r+");
      assert (desc->fp);

      desc->loaded_file = hashnum;

      free (filename);
    }
}

static unsigned int
descriptor_hash (unsigned char key)
{
  unsigned int hashnum = 0;
  unsigned char comp = 1;

  while (comp)
    {
      if (key & comp)
        hashnum++;
      comp <<= 1;
    }

  return hashnum;
}

static void
find_similarities (Descriptor * desc, SimilarityList * simlist, char *imgname,
                   unsigned char ds, MemoryIndex * pk, Base * base,
                   int hashnum)
{
  ArtworkInfo artwork;
  char *imgpath;
  char pkname[TITLE_LENGTH + 1];
  MemoryIndexRecord *match;
  unsigned char di;

  if ((hashnum < 0) || (hashnum >= DESC_HASH_NUM))
    return;

  change_hash_file (desc, hashnum);

  fseek (desc->fp, 0, SEEK_SET);

  while (!feof (desc->fp))
    {
      di = fgetc (desc->fp);
      fgets (pkname, TITLE_LENGTH + 1, desc->fp);
      stripWhiteSpace (pkname);

      if (descriptor_hash (di ^ ds) <= 2)
        {
          match = memory_index_find (pk, pkname);

          if (match)
            {
              fseek (base->fp, match->rrn * BASE_REG_SIZE, SEEK_SET);
              base_read_artwork_record (base, &artwork);

              imgpath = base_get_image_path (artwork.img);
              simlist_append (simlist, artwork, ComputaSimilaridade (imgpath, imgname));
              free (imgpath);
            }
        }
    }
}

void
descriptor_find (Descriptor * desc, char *imgname, MemoryIndex * pk, Base
                 *base, size_t maxresults)
{
  FILE *htmlfile;
  int i;
  SimilarityList *simlist = simlist_new ();
  unsigned char ds;
  unsigned int curload;

  assert (desc);

  if (!file_is_valid (imgname))
    return;

  ds = CalculaDescritor (imgname);
  change_hash_file (desc, descriptor_hash (ds));

  curload = desc->loaded_file;
  find_similarities (desc, simlist, imgname, ds, pk, base, curload - 1);
  find_similarities (desc, simlist, imgname, ds, pk, base, curload);
  find_similarities (desc, simlist, imgname, ds, pk, base, curload + 1);

  if (simlist->regnum == 0)
    printf ("   Nao ha imagens semelhantes a \"%s\".\n", imgname);
  else
    {
      qsort (simlist->list, simlist->regnum, sizeof (SimilarityRecord),
             simlist_compare);

      htmlfile = fopen (HTMLFILE, "w+");

      html_begin (htmlfile);
      fprintf (htmlfile, "<tr><td><img src=\"%s\"></td><td></td></tr>\n",
               imgname);

      for (i = 0; (i < maxresults) && (i < simlist->regnum); i++)
        html_write_record_info (htmlfile, &(simlist->list[i].artwork));

      html_end (htmlfile);
      fclose (htmlfile);
    }

  simlist_free (simlist);
}

void
descriptor_free (Descriptor * desc)
{
  if (desc)
    {
      if (desc->fp)
        fclose (desc->fp);

      free (desc->fp_name);
      free (desc);
    }
}

void
descriptor_insert (Descriptor * desc, const char *pkname, unsigned char d)
{
  assert (desc);

  change_hash_file (desc, descriptor_hash (d));

  fseek (desc->fp, 0, SEEK_END);
  fprintf (desc->fp, "%c%-200s", d, pkname);
}

Descriptor *
descriptor_new (const char *fp_name)
{
  Descriptor *desc = MEM_ALLOC (Descriptor);

  desc->fp_name = str_dup (fp_name);
  desc->fp = NULL;
  desc->loaded_file = -1;

  return desc;
}
