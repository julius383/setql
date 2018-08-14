#include <stdio.h>
#include <stdlib.h>
#include "set.h"

/* https://stackoverflow.com/questions/3437404/min-and-max-in-c#3437484 */
#define MAX(a,b) \
  ({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
   _a > _b ? _a : _b; })

struct avlnode *create_node(int d){
  struct avlnode *new_node = malloc(sizeof(struct avlnode));
  new_node->data = d;
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->parent = NULL;
  new_node->height = 0;
  return new_node;
}

void destroy_node(struct avlnode* node){
  if(node == NULL)
    ;
  else{
    destroy_node(node->left);
    destroy_node(node->right);
    free(node);
  }
}

/* updates the heights of all nodes below the one given
 * assuming the passed in node has the correct height
 * this is done so balance factor can be computed accurately*/
static void update_heights(struct avlnode* node){
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

/* rotate_left and rotate_right are the functions used to
 * maintain the avl tree invariants
 * */
static void rotate_left(struct avltree* tree, struct avlnode* node){ 
  struct avlnode* right = node->right;
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

static void rotate_right(struct avltree* tree, struct avlnode* node){
  struct avlnode* left = node->left;
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

/* returns the height of the entire subtree under root
 * by finding the maximum value of all the nodes below it
 */
static int get_height(struct avlnode* root){
  while(root){
    int i = get_height(root->left);
    int j = get_height(root->right);
    int k = root->height;
    return MAX(i, MAX(j, k));
  }
  return 0;
}

/* balance factor is given by the formula B-A where
 * B is the height of the right subtree and A the height
 * of the left subtree. A value > 1 indicates the tree
 * is right heavy and a value < 1 the tree is left heavy
 * */
static int balance_factor(struct avlnode* root){
  return get_height(root->right) - get_height(root->left);
}

void rebalance_tree(struct avltree* tree){
  struct avlnode* current = tree->root;
  while (current){
    struct avlnode* parent = current->parent;
    int balance = balance_factor(current);
    if(balance == -2){ /* left heavy so rotate right*/
      /* check if left subtree is right heavy
       * and apply additional left rotation
       * */
      int rightbalance = balance_factor(current->left);
      if (rightbalance > 0){
        rotate_right(tree, current);
        rotate_left(tree, current);
      }else {
        rotate_right(tree, current);
      }
    }else if (balance == 2) {/* right heavy */
      /* check if right subtree is left heavy 
       * and apply additional right rotation
       * */
      int leftbalance = balance_factor(current->right);
      if (leftbalance < 0){
        rotate_left(tree, current);
        rotate_right(tree, current);
      }else {
        rotate_left(tree, current);
      }
    }
    if(!current->parent) /* change tree root */
      tree->root = current;
    current = parent;
  }
}

struct avlnode* search(struct avlnode* node, void* data, comparefn cfn){
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

struct avlnode* find_in_tree(struct avltree* tree, void* data){
  return search(tree->root, data, tree->compare);
}

struct avlnode* insert(struct avlnode* root, comparefn cfn, void* data){
  if(root == NULL){
    root = create_node(*(int *)data);
    return root;
  }else {
    int compval = cfn(&root->data, data);
    struct avlnode* oldnode;
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

void avl_add(struct avltree* tree, void* data){
  tree->root = insert(tree->root, tree->compare, data);
  rebalance_tree(tree);
}

static struct avlnode* replace_node(struct avlnode* current){
  if (current){
    if (current->left && current->right){
      struct avlnode* temp;
      temp = current->right;
      struct avlnode* subtree_min = temp;
      /* find min of current->right and set its left node to
       * the current's left*/
      for (;subtree_min->left; subtree_min = subtree_min->left) 
        ;
      current->left->parent = subtree_min;
      subtree_min->left = current->left;
      current->right->height -= 1;
      return temp;
    }else if (current->right){
      current->right->height -= 1; /* goes up in tree so reduce height*/
      update_heights(current->right);
      return current->right; /* replace with right since no left*/
    }else if (current->left){
      current->left->height -= 1;
      update_heights(current->left);
      return current->left; /* replace with left since no right*/
    }
    else{
      return NULL; /* since has no children replace with nothing*/
    }
  }
  /* should never get here */
  return NULL;
}

void avl_remove(struct avltree* tree, void *data){
  if (tree->root){
    struct avlnode* node = search(tree->root, data, tree->compare);
    if (node){
      struct avlnode* new_node = replace_node(node);
      if (new_node){
        if (node->parent){
          new_node->parent = node->parent;
          if (node == node->parent->right)
            node->parent->right = new_node;
          else
            node->parent->left = new_node;
        }
        else {
          new_node->parent = NULL;
          tree->root = new_node;
        }
      }
      update_heights(new_node);
      rebalance_tree(tree);
      free(node); /* release memory occupied by removed node */
    }
  }
}

/* http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/ */
void print_tree(struct avlnode* root, int level, char* fstring) {
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
int num_compare(const void* a, const void* b){
  return *(int *)a == *(int *)b ? 0: *(int *)a >*(int *)b ? 1: -1;
}

int main() {
  struct avltree* test_tree = malloc(sizeof(struct avltree));
  test_tree->compare = num_compare;
  int i = 8, j = 5, k = 2, l = 7, o = 4;
  avl_add(test_tree, &l);
  avl_add(test_tree, &i);
  avl_add(test_tree, &o);
  avl_add(test_tree, &j);
  avl_add(test_tree, &k);
  print_tree(test_tree->root, 0, "(%d) %d\n");
  avl_remove(test_tree, &l);
  print_tree(test_tree->root, 0, "(%d) %d\n");

  destroy_node(test_tree->root);
  return 0;
}
