/*
 * Name of file: Words16.c
 * Written by: Jason Chu
 Words16 uses a WEPL-balanced binary search tree to store words extracted from files. You can insert words from
 a file, delete words from a file, print out the words in the tree in preorder, print out the count, key pairs
 in the tree in inorder, and print out the count and WEPL value at the root.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include "getLine.h"
#include "Tree.h"


// readWords takes in a file, parses through each line to extract words (words are separated by non-alphanumeric
// characters). Depending on the bool ins, either insert or delete the words from the Tree t.
// Tree *t is a WEPL-balanced binary search tree, file is some file opened with fopen, improvFactor is IF, and
// bool ins is true when we want to insert and false when we want to delete
void readWords(Tree *t, FILE *file, int improvFactor, bool ins)
{
    char *line = getLine(file);
    while (line != NULL)
    {
        bool inWord = false;
        int index = 0;
        int start = 0;
        while (line[index] != '\n' && line[index] != '\0')
        {
            // if not alphanumeric and in a word, then the word has ended
            if (!isalnum(line[index]) && inWord)
            {
                inWord = false;
                //char *str = malloc((index - start + 1) * sizeof(char));
                char *str = calloc(index - start + 1, sizeof(char));
                strncpy(str, line+start, index - start);
                if (ins)
                {
                    insert(t, str, improvFactor);
                }
                else
                {
                    delete(t, str, improvFactor);
                }
                free(str);
            }
            // if not in a word and is alphanumeric, then word begins
            if (!inWord && isalnum(line[index]))
            {
                inWord = true;
                start = index;
            }
            // if alphanumeric, convert to lower case
            if (isalnum(line[index]))
            {
                line[index] = tolower(line[index]);
            }
            index++;
        }
        // if '\n' is seen yet still in a word, extract that word
        if (inWord)
        {
            inWord = false;
            char *str = calloc(index - start + 1, sizeof(char));
            //char *str = malloc((index - start + 1) * sizeof(char));
            strncpy(str, line+start, index - start);
            if (ins)
                insert(t, str, improvFactor);
            else
                delete(t, str, improvFactor);
            free(str);
        }
        free(line);
        line = getLine(file);
    }
}

int main(int argc, char* argv[])
{
    Tree t;
    create(&t);
    long improvFactor = 0;
    // iterate through the command line arguments
    for (int i = 1; i < argc; i++)
    {
        // read words from TEXTFILE and puts into tree
        if (argv[i][0] != '-')
        {
            FILE *f;
            f = fopen(argv[i], "r");
            if (f == NULL)
            {
                fprintf(stderr, "%s %s\n", "Words16: cannot open ", argv[i]);
                exit(EXIT_FAILURE);
            }
            readWords(&t, f, improvFactor, true);
            fclose(f);
        }
        else
        {
            // found a -d flag and thus determines if the next argument is a valid file name and if so, deletes words from tree
            if (strcmp("-d", argv[i]) == 0)
            {
                if (argc <= i + 1)
                {
                    fprintf(stderr, "%s %s\n", "Words16: cannot open ", argv[i]);
                    exit(EXIT_FAILURE);
                }
                FILE *f;
                f = fopen(argv[i + 1], "r");
                if (f == NULL)
                {
                    fprintf(stderr, "%s %s\n", "Words16: cannot open ", argv[i + 1]);
                    exit(EXIT_FAILURE);
                }
                readWords(&t, f, improvFactor, false);
                fclose(f);
                i += 1;
            }
            // print flag
            else if (strcmp("-print", argv[i]) == 0)
            {
                inorder(t);
            }
            // dump flag
            else if (strcmp("-dump", argv[i]) == 0)
            {
                preorder(t);
            }
            // epl flag
            else if (strcmp("-epl", argv[i]) == 0)
            {
                epl(&t);
            }
            // set improvFactor
            else if (strcmp("-set", argv[i]) == 0)
            {
                if (argc <= i + 1)
                {
                    fprintf(stderr, "%s %s\n", "Words16: cannot open ", argv[i]);
                    exit(EXIT_FAILURE);
                }
                improvFactor = strtol(argv[i + 1], NULL, 10);
                i+=1;
            }
            else
            {
                // try to see if the argument starts with a "-" and is a valid file name; if so, open/reads words
                FILE *f;
                f = fopen(argv[i], "r");
                if (f == NULL)
                {
                    fprintf(stderr, "%s %s\n", "Words16: cannot open ", argv[i]);
                    exit(EXIT_FAILURE);
                }
                readWords(&t, f, improvFactor, true);
            }
        }
    }
    exit(0);
    return 0;
}
