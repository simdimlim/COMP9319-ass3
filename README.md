# COMP9319-ass3
A program that inputs search terms then generates and uses index files to find files with all the search terms and outputs the file names in decreasing order of frequency of the term occurrences.

# Libraries
- uthash
C macros for hash tables by Troy D. Hanson
source: https://github.com/troydhanson/uthash
documentation: http://troydhanson.github.com/uthash/

- stmr
Martin Porter’s Stemming algorithm as a C library by wooorm
source: https://github.com/wooorm/stmr.c

# Generating the index
If an index folder does not currently exist, one is created. The program then parses each file within the target directory. With each file, each word is scanned into a variable where it is then normalised (removed punctuation, all letters made lowercase and then stemmed). We use a hash to keep track of the unique words in the file. If the normalised word does not exist as a key in our hash, we add it in with its count as 1, else, we increment its current count.

An index file is then opened to write, where the index file's name is just the current file we are parsing appended with '.idx'. We then iterate through our hash table and print into the index file the word followed by a space and then the number of times the word occurred in the file. An example of the contents of an index file is as follows:

voyag 63
to 2101
moon 160
by 548
georg 7
tucker 4

# Searching files & using the index
After the index files are generated, we immediately use it to perform our search. We keep track of the file names and total number of occurrences of all the search terms in a hash.

We process through all the index files in the index folder and with each file, compare the word to all of the query terms. If they match, we increment the number of search queries encountered and also add the following count to the total number of occurrences within that file. When we finish processing the index file, if we found all search queries in the file, we add it to our 'result' hash table.

After each index file has been processed, we simply sort our result hash in decreasing order of values and print the file names in the sorted order.

