#ifndef SET_H
#define SET_H 1

typedef int (*comparefn)(const void *, const void *);

union Item {
  char *strval;
  int *intval;
  struct set *setval;
};

struct avlnode {
  unsigned height;
  int data;
  struct avlnode *parent;
  struct avlnode *right;
  struct avlnode *left;
};

struct avltree {
  struct avlnode *root;
  comparefn compare;
};

/* Prototypes */
struct avlnode *node_create(int d);
void node_destroy_recursive(struct avlnode *node);
struct avltree *tree_create(void *data, comparefn compare);
void tree_set(struct avltree *tree, struct avlnode *root, comparefn compare);
int tree_member(void *data, struct avltree *tree);
void tree_rebalance(struct avltree *tree);
struct avlnode *tree_search(void *data, struct avltree *tree);
void tree_insert(void *data, struct avltree *tree);
struct avlnode *tree_remove(void *data, struct avltree *tree);
void tree_print(struct avlnode *root, int level, char *fstring);
int num_compare(const void *a, const void *b);
int split_lt(void *key, struct avltree *tree, struct avltree *result);
int split_gt(void *key, struct avltree *tree, struct avltree *result);
int join(void *key, struct avltree *left_tree, struct avltree *right_tree, struct avltree *result);
int set_union(struct avltree *tree1, struct avltree *tree2, struct avltree *result);
int set_difference(struct avltree *tree1, struct avltree *tree2, struct avltree *result);
int set_intersection(struct avltree *tree1, struct avltree *tree2, struct avltree *result);

#endif
