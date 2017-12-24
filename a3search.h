#ifndef A3SEARCH_H
#define A3SEARCH_H

#include <dirent.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "stmr.h"
#include "uthash.h"

#define MAX_WORD_LENGTH 1000
#define MAX_FILENAME_LENGTH 256
#define MAX_PATH_LENGTH MAX_FILENAME_LENGTH+144

typedef struct hash_struct {
   char name[MAX_WORD_LENGTH]; // key
   int num; // value
   UT_hash_handle hh;
} hash;

void process(char *input);
int desc_sort(hash *a, hash *b);
char *removeExt(char* mystr);
void create_index(char *index, char *target, int conceptSearch);
hash *read_index(char *index, int argc, int conceptSearch, char* query1, char* query2, char* query3, char* query4, char* query5);
hash *sort(hash *result);
void print_results(hash *result);
int check_index_exists(char *index);
void remove_punctuation(char *p);
void make_lowercase(char *p);

#endif
