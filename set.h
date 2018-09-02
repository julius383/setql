#ifndef SET_H
#define SET_H 1

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

struct avlnode *create_node(int d);
void destroy_node(struct avlnode *node);
struct avlnode *search(struct avlnode *node, void *data, comparefn cfn);
void rebalance_tree(struct avltree* tree);
struct avlnode *find_in_tree(struct avltree *tree, void *data);
void avl_add(struct avltree* tree, void* data);
void avl_remove(struct avltree* tree, void *data);
void print_tree(struct avlnode *root, int level, char *fstring);
int num_compare(const void *a, const void *b);
struct avlnode* split_lt(struct avlnode* root, void* key, comparefn compare);
struct avlnode* split_gt(struct avlnode* root, void* key, comparefn compare);
struct avlnode* join(void* , struct avlnode* left, struct avlnode* right, comparefn);

#endif
