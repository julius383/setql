#ifndef SET_H
#define SET_H

#include <stdlib.h>

enum color { RED, BLACK };
typedef int (*comparefn)(const void *, const void *);

union Item {
  char *strval;
  int *intval;
  struct set *setval;
};

struct avlnode {
  struct avlnode *parent;
  struct avlnode *right;
  struct avlnode *left;
  int height;
  int data;
};

struct avltree {
  struct avlnode *root;
  comparefn compare;
};

struct avlnode *create_node(int d){
  struct avlnode *new_node = malloc(sizeof(struct avlnode));
  new_node->data = d;
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->parent = NULL;
  new_node->height = 0;
  return new_node;
}
void destroy_node(struct avlnode *node);
struct avlnode *search(struct avlnode *node, comparefn cfn, void *data);
struct avlnode *find_in_tree(struct avltree *tree, void *data);
struct avlnode *insert(struct avlnode *root, comparefn cfn, void *data);
void print_tree(struct avlnode *root, int level, char *fstring) ;
int num_compare(const void *a, const void *b);

#endif
