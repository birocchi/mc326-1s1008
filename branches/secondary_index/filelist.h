#ifndef __FILELIST_H
#define __FILELIST_H

#define DBFILE      "base01.dat"   /**< The database file name. */
#define HTMLFILE    "lista.html"   /**< The HTML output. */
#define PKFILE      "pkfile.pk"    /**< The primary key file name. */

/* Secondary indexes */
#define SI_AUTHOR_INDEX   "author.sk"
#define SI_AUTHOR_LIST    "author.sl"
#define SI_TYPE_INDEX     "type.sk"
#define SI_TYPE_LIST      "type.sl"
#define SI_YEAR_INDEX     "year.sk"
#define SI_YEAR_LIST      "year.sl"

/* Avail lists */
#define DBFILE_AVAIL      "base01.av"
#define SI_AUTHOR_AVAIL   "author.av"
#define SI_TYPE_AVAIL     "type.av"
#define SI_YEAR_AVAIL     "year.av"

#endif
