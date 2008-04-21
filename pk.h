#ifndef __PK_H
#define __PK_H

#include "base.h"

/** Relative record number: 32-bit integer (4 bytes) */
#define RRN_LENGTH 4
/** Total size of a primary key entry */
#define PK_REG_SIZE (TITLE_LENGTH + RRN_LENGTH)

/**
 * \brief Internal structure used to store the primary keys list.
 *
 * This structure actually stores the information about each
 * record: its name and RRN.
 */
typedef struct {
  /** The Relative Register Number. */
  int rrn;
  /** The primary key itself (the name) */
  char name[TITLE_LENGTH+1]; 
} PrimaryKeyRecord;

/**
 * \brief Structure used to handle primary keys.
 *
 * This is a wrapper around the real structure,
 * \a PrimaryKeyRecord, and should be used instead
 * of it.
 */
typedef struct {
  /** Current number of records. */
  int regnum;
  /** Maximum possible number of records */
  int maxregs;
  /** The primary key list */
  PrimaryKeyRecord* pklist;
} PrimaryKeyList;

/**
 * \brief   Searches for the key 'key' in the 'index' list.
 * \param   index Pointer to a PrimaryKeyList struct.
 * \param   key The string to be searched as primary key.
 * \return  Returns the key's rrn if found or -1 case not.
 */
int pkListFindByName(PrimaryKeyList* index, const char* key);

/**
 * \brief Free memory for a PrimaryKeyList.
 * \param index Pointer to a PrimaryKeyList struct.
 */
void pkListFree(PrimaryKeyList* index);

/**
 * \brief Inserts an entry into the list.
 * \param index Pointer the PrimaryKeyList struct.
 * \param name String with the primary key.
 * \retval 0 Success.
 * \retval 1 Error.
 */
int pkListInsert(PrimaryKeyList* index, const char* name);

/**
 * \brief Checks if the PK table is empty.
 * \param index Pointer to the PrimaryKeyList struct.
 * \retval 0 The table is not empty.
 * \retval 1 The table is empty.
 */
int pkListIsEmpty(PrimaryKeyList* index);

/**
 * \brief Loads data for a primary key index.
 * \param base_name Database name.
 * \param pkname Primary keys index file name.
 * \return Returns a new index with the loaded data,
 *         or an empty index if neither \a base_name
 *         nor \a pkname are valid files.
 */
PrimaryKeyList* pkListLoad(const char* base_name, const char* pkname);

/**
 * \brief Load the primary keys from the database.
 * \param base_name The name of the database.
 * \return New index structure with the loaded data.
 */
PrimaryKeyList* pkListLoadFromBase(const char* base_name);

/**
 * \brief Load the primary keys from a previous primary keys cache.
 * \param pkname The name of the cache file.
 * \return New index structure with the loaded data.
 */
PrimaryKeyList* pkListLoadFromPK(const char* pkname);

/**
 * \brief Initialize a new primary keys list.
 * \param nelem Number of elements to allocate memory for.
 * \return Returns a pointer to the initialized list or NULL
 *         if it could not be initialized.
 *
 * This function allocates space for \a nelem records, or
 * 40 records if \a nelem is 0.
 */
PrimaryKeyList* pkListNew(size_t nelem);

/**
 * \brief Removes an entry from the primary key index.
 * \param index Pointer to the PrimaryKeyList struct.
 * \param name The name (primary key) that will be removed.
 * \param rrn The removed RRN.
 * \retval 0 If removal went ok.
 * \retval 1 If an error occurred.
 */
int pkListRemove(PrimaryKeyList* index, const char* name, int* rrn);

/**
 * \brief Dump an index to a file.
 * \param index Pointer to the PrimaryKeyList struct.
 * \param pkfile File where the data is going to be dumped.
 */
void pkListWriteToFile(PrimaryKeyList* index, FILE* pkfile);

#endif
