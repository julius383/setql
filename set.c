#include <stdio.h>
#include <stdlib.h>
#include "set.h"

/* https://stackoverflow.com/questions/3437404/min-and-max-in-c#3437484 */
#define max(a,b) \
  ({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
   _a > _b ? _a : _b; })

void destroy_node(struct avlnode *node){
  if(node == NULL)
    ;
  else{
    destroy_node(node->left);
    destroy_node(node->right);
    free(node);
  }
}

static void update_heights(struct avlnode *node){
  if(node){
    if(node->right){
      node->right->height = node->height + 1;
      update_heights(node->right);
    }
    if(node->left){
      node->left->height = node->height + 1;
      update_heights(node->left);
    }
  }
}

static void rotate_left(struct avltree *tree, struct avlnode *node){ 
  struct avlnode *right = node->right;
  if (node == tree->root) {
    tree->root = right;
  }
  else if (node == node->parent->left) {
    node->parent->left = right;
  }
  else {
    node->parent->right = right;
  }
  node->right = right->left;
  if(right->left){
    right->left->parent = node;
  }
  right->left = node;
  right->parent = node->parent;
  node->parent = right;

  node->height += 1;
  update_heights(node);
  right->height -= 1;
  update_heights(right);

}

static void rotate_right(struct avltree *tree, struct avlnode *node){
  struct avlnode *left = node->left;
  if (node == tree->root) {
    tree->root = left;
  }
  else if (node == node->parent->left) {
    node->parent->left = left;
  }
  else {
    node->parent->right = left;
  }
  node->left = left->right;
  if(left->right){
    left->right->parent = node;
  }
  left->right = node;
  node->parent = left;

  node->height += 1;
  update_heights(node);
  left->height -= 1;
  update_heights(left);
}

static int get_height(struct avlnode *root){
  while(root){
    int i = get_height(root->left);
    int j = get_height(root->right);
    int k = root->height;
    return max(i, max(j, k));
  }
  return 0;
}

static int balance_factor(struct avlnode *root){
  return get_height(root->right) - get_height(root->left);
}

static void rebalance_tree(struct avltree *tree){
  struct avlnode *current = tree->root;
  while (current){
    struct avlnode *parent = current->parent;
    int balance = balance_factor(current);
    if(balance == -2){ /* left heavy */
      /* check if left subtree is right heavy*/
      int rightbalance = balance_factor(current->left);
      if (rightbalance > 0){
        rotate_right(tree, current);
        rotate_left(tree, current);
      }else {
        rotate_right(tree, current);
      }
    }else if (balance == 2) {/* right heavy */
      /* check if right subtree is left heavy */
      int leftbalance = balance_factor(current->right);
      if (leftbalance < 0){
        rotate_left(tree, current);
        rotate_right(tree, current);
      }else {
        rotate_left(tree, current);
      }
    }
    if(!current->parent)
      tree->root = current;
    current = parent;
  }
}

struct avlnode *search(struct avlnode *node, comparefn cfn, void *data){
  if(node){
    int compval = cfn(&node->data, data);
    if(compval < 0){
      return search(node->right, data, cfn);
    } else if (compval > 0) {
      return search(node->left, data, cfn);
    } else {
      return node;
    }
  }
  return NULL;
}

struct avlnode *find_in_tree(struct avltree *tree, void *data){
  return search(tree->root, tree->compare, data);
}

struct avlnode *insert(struct avlnode *root, comparefn cfn, void *data){
  if(root == NULL){
    root = create_node(*(int *)data);
    return root;
  }else {
    int compval = cfn(&root->data, data);
    struct avlnode *oldnode;
    if(compval > 0){
      oldnode = root->left;
      root->left = insert(root->left, cfn, data);
      root->left->parent = root;
      if (oldnode == NULL)
        root->left->height = root->height + 1;
    }else{ 
      oldnode = root->right;
      root->right = insert(root->right, cfn, data);
      root->right->parent = root;
      if (oldnode == NULL)
        root->right->height = root->height + 1;
    }
  }
  return root;
}

void add(struct avltree *tree, void *data){
  tree->root = insert(tree->root, tree->compare, data);
  rebalance_tree(tree);
}

/* http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/ */
void print_tree(struct avlnode *root, int level, char *fstring) {
  int spaces = 0;
  if(root){
    for(int i = 1;i < level * 2;i++){
      if (level == 1)
        spaces += 1;
      else if(i % 2 != 0)
        printf(" |");
      else
        spaces += 2;
    }
    for(;spaces > 0;spaces--)
      printf(" ");
    level == 0 ? 0 :printf("`--");
    printf(fstring, root->data, root->height);
    print_tree(root->left, level + 1, fstring);
    print_tree(root->right, level + 1, fstring);
  }
}
int num_compare(const void *a, const void *b){
    return *(int *)a == *(int *)b ? 0: *(int *)a >*(int *)b ? 1: -1;
}

int main() {
  struct avltree *left_heavy = malloc(sizeof(struct avltree));
  struct avltree *right_heavy = malloc(sizeof(struct avltree));
  left_heavy->compare = num_compare;
  right_heavy->compare = num_compare;
  int i = 3, j = 5, k = 2, l = 7, n = 1, o = 4;
  add(left_heavy, &l);
  add(right_heavy, &n);

  add(left_heavy, &j);
  add(right_heavy, &k);
  add(left_heavy, &i);
  add(right_heavy, &o);

  print_tree(left_heavy->root, 0, "(%d) %d\n");
  printf("bf: %d \n", balance_factor(left_heavy->root));

  /*rotate_right(left_heavy, left_heavy->root);*/
  /*print_tree(left_heavy->root, 0, "(%d) %d\n");*/
  /*printf("bf: %d \n", balance_factor(left_heavy->root));*/

  printf("\n");

  print_tree(right_heavy->root, 0, "(%d) %d\n");
  printf("bf: %d \n", balance_factor(right_heavy->root));

  /*rebalance_tree(right_heavy);*/
  /*print_tree(right_heavy->root, 0, "(%d) %d\n");*/
  /*printf("bf: %d \n", balance_factor(right_heavy->root));*/

  destroy_node(left_heavy->root);
  destroy_node(right_heavy->root);
  return 0;
}
