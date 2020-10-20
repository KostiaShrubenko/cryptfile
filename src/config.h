#ifndef CONFIG_H
#define CONFIG_H


/*      --- Header Properties ---       */
/* Magic number which is placed to beginning and an end of the header.  */
/* It serves as indication of header presence and its boundaries.       */
#define HEADER_NUMBER 0x7A8B7A7AU
#define HEADER_NUMBER_LENGTH 4  /* Length of header magic number in bytes. */
#define HEADER_CONTENT_LENGTH 8 /* Length of header content in bytes.       */
#define HEADER_FULL_LENGTH (2 * (HEADER_NUMBER_LENGTH) + (HEADER_CONTENT_LENGTH))

#define TRUE    1
#define FALSE   0

/*      --- AES CYPHER PARAMETERS ---       */
#define KEY_SIZE        32
#define NUMBER_ROUNDS   0
#define AES_BLOCK_SIZE      16
/*      --- AES CYPHER PARAMETERS ---       */

/*      --- Path and output files naming ---        */
#define HELP_FILE               "Help.txt"      /* Path to file which contains help information.        */
/* ## The following values are used when path to output file is not specifyed.                          */
#define DEFAULT_DESTINATION     "outfiles\\"    /* Path to output files directory from relative path.   */
#define FILE_PREFIX_DECRYPTED   "decrypted_"    /* Prefix for output files which was decrypted.         */
#define FILE_PREFIX_ENCRYPTED   "encrypted_"    /* Prefix for output files which was encrypted.         */
#define FILE_EXTENSION          ".bin"          /* Default extensions for output files.                 */
/* Characters to form valid path */
#define FOLDER_SEPARATOR    '\\'
#define EXTENSION_SEPARATOR '.'

#define MAX_PATH_LENGTH         150 /* Max character length of path to file.     */
#define MAX_FILE_NAME_LENTGTH   30  /* Max character length of output file name. */   

#endif /* CONFIG_H*/