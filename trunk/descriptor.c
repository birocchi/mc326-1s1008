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

/**
 * @brief Appends an entry to the similarity list.
 *
 * @param simlist The similarity list being used.
 * @param rrn     The RRN of the record in the base.
 * @param sim     The similarity to a given image.
 */
static void simlist_append (SimilarityList * simlist, int rrn,
                            double sim);

/**
 * @brief Compariso function to order a similarity list.
 *
 * @param a A \a SimilarityRecord.
 * @param b A \a SimilarityRecord.
 *
 * @retval -1 \a a's similarity is bigger than \a b's.
 * @retval 0 \a a's similarity is equal to \a b's.
 * @retval 1 \a a's similarity is smaller than \a b's.
 *
 * It sorts in descending order.
 */
static int simlist_compare (const void *a, const void *b);

/**
 * @brief Allocates more memory for a similarity list.
 *
 * @param simlist The similarity list being used.
 * @param newsize The new size of the list.
 */
static void simlist_inflate (SimilarityList * simlist, size_t newsize);

/**
 * @brief Changes the loaded hash file according to the given hash number.
 *
 * @param desc    The descriptor in use.
 * @param hashnum The hash returned by the hash function.
 */
static void change_hash_file (Descriptor * desc, unsigned int hashnum);

/**
 * @brief Hashes a given key (a character).
 *
 * @param key The key to hash.
 *
 * @return The number of bits 1 in the given byte.
 */
static unsigned int descriptor_hash (unsigned char key);

static void
simlist_append (SimilarityList * simlist, int rrn, double sim)
{
  if (simlist->regnum == simlist->maxregs)
    simlist_inflate (simlist, simlist->maxregs * 2);

  simlist->list[simlist->regnum].rrn = rrn;
  simlist->list[simlist->regnum].similarity = sim;

  simlist->regnum++;
}

static int
simlist_compare (const void *a, const void *b)
{
  double sa = ((*((SimilarityRecord *) a)).similarity);
  double sb = ((*((SimilarityRecord *) b)).similarity);

  if (sa < sb)
    return 1;
  else if (sa > sb)
    return -1;
  else
    return 0;
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

SimilarityList * descriptor_find (Descriptor *desc, Base *base, MemoryIndex *pk, char *imgname)
{
  char baseimg[IMG_LENGTH + 1] = { '\0' };
  char *imgpath;
  char pkname[TITLE_LENGTH + 1] = { '\0' };
  int i;
  MemoryIndexRecord *match;
  SimilarityList *simlist = simlist_new ();
  unsigned char di, ds;
  unsigned int curload;

  assert (file_is_valid (imgname));

  ds = CalculaDescritor (imgname);
  curload = descriptor_hash (ds);

  /* Run the algorithm for Vn-1, V and Vn+1 */
  for (i = (curload - 1); i <= (curload + 1); i++)
    {
      /* Skip invalid values */
      if ((i < 0) || (i > DESC_HASH_NUM))
        continue;

      change_hash_file (desc, i);

      fseek (desc->fp, 0, SEEK_SET);

      while (!feof (desc->fp))
        {
          /* Read an entry */
          di = fgetc (desc->fp);
          fgets (pkname, TITLE_LENGTH + 1, desc->fp);
          stripWhiteSpace (pkname);

          /* Check if it passes the threshold */
          if (descriptor_hash (di ^ ds) <= 2)
            {
              match = memory_index_find (pk, pkname);

              if (match)
                {
                  /* Seek to the image name in the entry */
                  fseek (base->fp, (match->rrn * BASE_REG_SIZE) + (BASE_REG_SIZE - IMG_LENGTH), SEEK_SET);
                  fgets (baseimg, IMG_LENGTH + 1, base->fp);

                  imgpath = base_get_image_path (baseimg);
                  simlist_append (simlist, match->rrn,
                                  ComputaSimilaridade (imgpath, imgname));
                  free (imgpath);
                }
            }
        }
    }

  qsort (simlist->list, simlist->regnum, sizeof (SimilarityRecord),
         simlist_compare);

  return simlist;
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

void
simlist_free (SimilarityList * simlist)
{
  if (simlist)
    {
      free (simlist->list);
      free (simlist);
    }
}

SimilarityList *
simlist_new (void)
{
  SimilarityList *ret = MEM_ALLOC (SimilarityList);

  ret->list = NULL;
  ret->regnum = 0;
  ret->maxregs = 0;

  return ret;
}

