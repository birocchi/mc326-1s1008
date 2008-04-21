#ifndef __PK_H
#define __PK_H

#include "base.h"

/** Relative record number: 32-bit integer (4 bytes) */
#define RRN_LENGTH  4
/** 4 bytes for a 32-bit integer */
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
 * Entirely frees the struct pointed by 'index'. 
 * Frees the pointer to the list,
 * and only then frees the struct itselft.
 *
 * \param index Pointer to a PrimaryKeyList struct.
 */
void pkListFree(PrimaryKeyList* index);

/**
 * Takes the pointer to the PrimaryKeyList,
 * and a string, the pk. Adds the new key
 * to the PK index.
 *
 * \param index Pointer the PrimaryKeyList struct.
 *
 * \param name String with the primary key.
 *
 * \return Returns 0 if everything went ok \
 * or 1 if the key was already in the list \
 * or if it had any problems inflating the table \
 * case it was already full.
 */
int pkListInsert(PrimaryKeyList* index, const char* name);

/**
 * Checks if the PK table is empty.
 *
 * \param index Pointer to the PrimaryKeyList struct.
 *
 * \return Returns 1 if the list is empty or 'index' is NULL, 0 otherwise.
 */
int pkListIsEmpty(PrimaryKeyList* index);

/**
 * \brief Loads data for a primary key index.
 *
 * \param base_name Database name.
 * \param pkname Primary keys index file name.
 *
 * \return Returns a new index with the loaded data,
 *         or an empty index if neither \a base_name
 *         nor \a pkname are valid files.
 */
PrimaryKeyList* pkListLoad(const char* base_name, const char* pkname);

/**
 * \brief Load the primary keys from the database.
 *
 * \param base_name The name of the database.
 *
 * \return New index structure with the loaded data.
 */
PrimaryKeyList* pkListLoadFromBase(const char* base_name);

/**
 * \brief Load the primary keys from a previous
 *        primary keys cache.
 *
 * \param pkname The name of the cache file.
 *
 * \return New index structure with the loaded data.
 */
PrimaryKeyList* pkListLoadFromPK(const char* pkname);

/**
 * Allocate memory for a new primary key list and
 * initialize it.
 *
 * It allocates space for \a nelem records, or
 * 40 records if \a nelem is 0.
 *
 * @param nelem Number of elements to allocate memory for.
 *
 * \retval Returns a pointer to the initialized list or NULL
 * if it could not be initialized.
 */
PrimaryKeyList* pkListNew(size_t nelem);

/**
 * Removes an entry from the Primary Key Index.
 *
 * \param index Pointer to the PrimaryKeyList struct.
 *
 * \param name The name (primary key) that will be removed.
 *
 * \return rrn is gonna be the removed rrn. 
 *
 * \retval 0 If removal went ok.
 *
 * \retval 1 If an error occurred.
 */
int pkListRemove(PrimaryKeyList* index, const char* name, int* rrn);

/**
 * Takes the primary key index 'index' and writes it properly
 * to the file 'pkfile'.
 *
 * \param index Pointer to the PrimaryKeyList struct.
 *
 * \param pkfile File pointer to the primary keys file.
 */
void pkListWriteToFile(PrimaryKeyList* index, FILE* pkfile);

#endif