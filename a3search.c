#include "a3search.h"

int main(int argc, char *argv[])
{
   char *target = argv[1];
   char *index = argv[2];
   char *concept = argv[3];
   float cnum;
   char *query1, *query2, *query3, *query4, *query5;
   int conceptSearch = FALSE;
   
   if (strcmp(concept, "-c") == 0) {
      cnum = atof(argv[4]);
      conceptSearch = TRUE;
      if (argc >= 6)
         query1 = argv[5];
         
      if (argc >= 7)
         query2 = argv[6];
         
      if (argc >= 8)
         query3 = argv[7];
         
      if (argc >= 9)
         query4 = argv[8];
         
      if (argc >= 10)
         query5 = argv[9];
         
   } else {
      if (argc >= 4) {
         query1 = argv[3];
         process(query1);
      }
      if (argc >= 5) {
         query2 = argv[4];
         process(query2);
      }
      if (argc >= 6) {
         query3 = argv[5];
         process(query3);
      }
      if (argc >= 7) {
         query4 = argv[6];
         process(query4);
      }
      if (argc >= 8) {
         query5 = argv[7];
         process(query5);
      }
   }

   hash *result = NULL;
   
   // if index folder doesn't exist, create one
   if (check_index_exists(index) == FALSE)
      create_index(index, target, conceptSearch);
   
   // read index folder and store all files->occ. of search terms in a "result" hash table
   result = read_index(index, argc, conceptSearch, query1, query2, query3, query4, query5);
   // sort the hash table in decreasing order of search term occurrences
   result = sort(result);
   // print the output
   print_results(result);
   
   return EXIT_SUCCESS;
}

int check_index_exists(char *index) {
   DIR *directory;
   if ((directory = opendir(index)) == NULL)
      // index folder does not exist
      return FALSE;
   closedir(directory);
   return TRUE;
}

void create_index(char *index, char *target, int conceptSearch) {
   DIR *directory = opendir(target);
   struct dirent* file;
   FILE *currFile, *indexFile;
   char filename[MAX_PATH_LENGTH], indexName[MAX_PATH_LENGTH], input[MAX_WORD_LENGTH];
   hash *t, *item, *tmp, *table = NULL;
   
   // make a directory with the index folder name
   mkdir(index, 0700);
   
   // loop through all files in our target directory
   while ((file = readdir(directory)) != NULL) {
      // check if user input the folder with a trailing '/'
      if (target[strlen(target) - 1] != '/') {
         sprintf(filename, "%s/%s", target, file->d_name);
      } else {
         sprintf(filename, "%s%s", target, file->d_name);
      }
      
      // open the file in the target directory to read
      currFile = fopen(filename, "r");
      
      table = NULL;
      while (fscanf(currFile, "%s", input) != EOF) {
         // normalise the word
         process(input);
         
         if (conceptSearch == FALSE) {
            // if the word does not exist in our hash table yet, add it
            HASH_FIND_STR(table, input, t);
            if (t == NULL) {
               t = (hash*)malloc(sizeof(hash));
               strcpy(t->name, input);
               t->num = 1;
               HASH_ADD_STR(table, name, t);
            } else {
               // if the word already exists, increment its occurrence count
               t->num++;
            }
         }
      }
      
      if (strstr(index, "/") == NULL) {
         sprintf(indexName, "%s/%s.idx", index, file->d_name);
      } else {
         sprintf(indexName, "%s%s.idx", index, file->d_name);
      }
      
      // open the index file to write
      indexFile = fopen(indexName, "w");
      
      // loop through our hash table storing all of our words->count
      HASH_ITER(hh, table, item, tmp) {
         fprintf(indexFile, "%s", item->name);
         if (item->num >= 0) {
            fprintf(indexFile, " %d", item->num);
         }
         fprintf(indexFile, "\n");
         free(item);
      }
      fclose(indexFile);
      fclose(currFile);
      HASH_CLEAR(hh, table);
   }
   closedir(directory);
}

hash* read_index(char *index, int argc, int conceptSearch, char* query1, char* query2, char* query3, char* query4, char* query5) {
   DIR *directory = opendir(index);
   struct dirent* file;
   FILE *currFile;
   int total, queriesFound, value, numQueries;
   char input[MAX_WORD_LENGTH], filename[MAX_PATH_LENGTH];
   hash *t, *item, *tmp, *result = NULL;
   
   if (conceptSearch == FALSE) {
      numQueries = argc - 3;
   }
   
   // loop through each file in the index directory
   while ((file = readdir(directory)) != NULL) {
      // check to see if user has input the folder with a trailing '/'
      if (strstr(index,"/") == NULL) {
         sprintf(filename, "%s/%s", index, file->d_name);
      } else {
         sprintf(filename, "%s%s", index, file->d_name);
      }
      
      currFile = fopen(filename, "r");
      total = 0;
      queriesFound = 0;
      
      // loop through all files in the index folder
      while (fscanf(currFile, "%s %d", input, &value) != EOF) {
         if (conceptSearch == FALSE) {
            // compare the input word with each query term, if they match,
            // increment the total query occurence for this file by the corresponding value
            if (argc >= 4) {
               if (strcmp(input, query1) == 0) {
                  total += value;
                  queriesFound++;
               }
            }
            if (argc >= 5) {
               if (strcmp(input, query2) == 0) {
                  total += value;
                  queriesFound++;
               }
            }
            if (argc >= 6) {
               if (strcmp(input, query3) == 0) {
                  total += value;
                  queriesFound++;
               }
            }
            if (argc >= 7) {
               if (strcmp(input, query4) == 0) {
                  total += value;
                  queriesFound++;
               }
            }
            if (argc >= 8) {
               if (strcmp(input, query5) == 0) {
                  total += value;
                  queriesFound++;
               }
            }
         }
      }
      fclose(currFile);
      
      // if there were the same number of query term matches with the number
      // of query terms the user inputted, this file will be outputted as a result
      if (queriesFound == numQueries) {
         // store the file name and total number of query occurrences into our result hash table
         t = (hash*)malloc(sizeof(hash));
         strcpy(t->name, removeExt(file->d_name));
         t->num = total;
         HASH_ADD_STR(result, name, t);
      }
   }
   closedir(directory);
   return result;
}

hash *sort(hash *result) {
   HASH_SORT(result, desc_sort);
   return result;
}

void print_results(hash *result) {
   hash *item, *tmp;
   HASH_ITER(hh, result, item, tmp) {
      printf("%s\n", item->name);
   }
}

// sort the hash by decreasing order of values
int desc_sort(hash *a, hash *b) {
   return (b->num - a->num);
}

void remove_punctuation(char *p) {
   char *src = p, *dst = p;
   while (*src) {
      if (ispunct((unsigned char)*src)) {
         /* Skip this character */
         src++;
      } else if (src == dst) {
         /* Increment both pointers without copying */
         src++;
         dst++;
      } else {
         /* Copy character */
         *dst++ = *src++;
      }
   }
   *dst = 0;
}

void make_lowercase(char *p) {
   char *src = p, *dst = p;
   while (*src) {
      if (isupper((unsigned char)*src)) {
         /* Make it lowercase */
         *dst++ = tolower((unsigned char)*src);
         src++;
      } else if (src == dst) {
         /* Increment both pointers without copying */
         src++;
         dst++;
      } else {
         /* Copy character */
         *dst++ = *src++;
      }
   }
   *dst = 0;
}

// make the word lowercase, remove punctuation and then stem
void process(char *input) {
   make_lowercase(input);
   remove_punctuation(input);
   int end = stem(input, 0, strlen(input) - 1);
   input[end + 1] = 0;
}

// remove the file extension from a given file name
char *removeExt(char* mystr) {
    char *retstr;
    char *lastdot;
    if (mystr == NULL)
         return NULL;
    if ((retstr = malloc (strlen (mystr) + 1)) == NULL)
        return NULL;
    strcpy (retstr, mystr);
    lastdot = strrchr (retstr, '.');
    if (lastdot != NULL)
        *lastdot = '\0';
    return retstr;
}
