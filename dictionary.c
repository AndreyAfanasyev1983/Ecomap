/**
 * Implements a dictionary's functionality.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"
// Alphabet size + apostrofe
#define ARRAY_SIZE 27
typedef struct Node
{
    struct Node* children[ARRAY_SIZE];
    bool is_word;
    
}Node;
Node* dictionary_root = NULL;

unsigned int dictionary_size = 0;

int get_index(char letter)
{
    if(letter == '\'')
    {
        return ARRAY_SIZE - 1;
    }
    return tolower(letter) - 'a';
}

void add_to_dictionary(const char* word)
{
  if (dictionary_root == NULL) 
  {
      dictionary_root = calloc(1, sizeof(Node));
  }
  Node* current_node = dictionary_root;
  for(int i = 0, n = strlen(word); i < n; i++)
  {
      int cell_index = get_index(word[i]);
      if(current_node->children[cell_index] == NULL)
      {
         current_node->children[cell_index] = calloc(1, sizeof(Node));
      }
     current_node = current_node->children[cell_index];
  }
  current_node->is_word = true;
  dictionary_size ++;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    if( word == NULL)
    {
      return false;   
    }
    if (dictionary_root == NULL) 
  {
      return false;
      
  }
     Node* current_node = dictionary_root;
  bool found = true;
     
  for (int i = 0, n = strlen(word); i < n; i++)
  {
      int cell_index = get_index(word[i]);
      if(current_node->children[cell_index] == NULL)
      {
         found = false;
         break;
      }
     current_node = current_node->children[cell_index];
  }
  if(found)
  {
     found = current_node->is_word; 
  }
    return found;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
  
  if (dictionary == NULL)
  {
      return false;
  }
  FILE* inptr = fopen(dictionary, "r");
  if (inptr == NULL)
  {
      return false;
  }
  char word[LENGTH + 1];
  while( fscanf(inptr,"%s\n",word) != EOF)
  {
    add_to_dictionary(word);  
  }
 
  
  fclose (inptr); 
    return true;
}
/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */


unsigned int size(void)
{
    return dictionary_size;
}
/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */


/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
void do_unload(Node* node)
{
    if (node == NULL)
    {
        return;
    }
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        do_unload(node->children[i]);
    }
    free(node);
}
bool unload(void)
{
do_unload(dictionary_root);
    return true;
}