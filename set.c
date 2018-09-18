#include <stdio.h>
#include <stdlib.h>
#include "set.h"

/* https://stackoverflow.com/questions/3437404/min-and-max-in-c#3437484 */
#define MAX(a,b) \
  ({ __typeof__ (a) _a = (a); \
   __typeof__ (b) _b = (b); \
   _a > _b ? _a : _b; })

struct avlnode *node_create(int d){
  struct avlnode *new_node = malloc(sizeof(struct avlnode));
  new_node->data = d;
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->parent = NULL;
  new_node->height = 0;
  return new_node;
}

void node_destroy_recursive(struct avlnode* node){
  if(node == NULL)
    ;
  else{
    node_destroy_recursive(node->left);
    node_destroy_recursive(node->right);
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

struct avltree* tree_create(comparefn compare, void *data){
  struct avltree* tree = malloc(sizeof(struct avltree));
  if(tree) {
    tree->root = node_create(*(int *)data);
    tree->compare = compare;
    return tree;
  }
  return NULL;
}

void tree_set(struct avltree *tree, struct avlnode* root, comparefn compare){
  tree->root = root;
  tree->compare = compare;
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

struct avlnode* search(struct avltree *tree, void* data){
  struct avlnode *current = tree->root;
  while (current) {
      int compval = tree->compare(&current->data, data);
      if((compval < 0) && current->right){
        current = current->right;
      } else if ((compval > 0) && current->left) {
        current = current->left;
      } else {
        break; /* node found break out of loop to return */
      }
  }
  return current; /* returns found node or position data should be inserted*/
}

void insert(struct avltree* tree,  void* data){
  if(!tree->root){
    tree->root = node_create(*(int *)data);
  }else {
    struct avlnode* root_position = search(tree, data);
    int compval = tree->compare(&root_position->data, data);
    if (compval < 0) {
      root_position->right = node_create(*(int *)data);
      root_position->right->parent = root_position;
      root_position->right->height = root_position->height + 1;
    }else if (compval > 0) {
      root_position->left = node_create(*(int *)data);
      root_position->left->parent = root_position;
      root_position->left->height = root_position->height + 1;
    } else
      return;
  }
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

struct avlnode* avl_remove(struct avltree* tree, void *data){
  if (tree->root){
    struct avlnode* node = search(tree, data);
    if (!tree->compare(&node->data, data)){
      struct avlnode* new_node = replace_node(node);
      if (new_node){
        if (node->parent){
          new_node->parent = node->parent;
        }
        else {
          new_node->parent = NULL;
          tree->root = new_node;
        }
      }
      if (node == node->parent->right)
        node->parent->right = new_node;
      else
        node->parent->left = new_node;
      update_heights(new_node);
      rebalance_tree(tree);
      return node; /* return pointer to memory to be freed when not needed */
    }
    return NULL;
  }
  return NULL;
}

/* http://www.randygaul.net/2015/06/15/printing-pretty-ascii-trees/ */
void print_tree(struct avlnode* root, int level, char* fstring) {
  int spaces = 0;
  if(root){
    for(int i = 1;i < level * 2;i++){
      if (level == 1)
        spaces += 1;
      else if(i % 2)
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

int split_lt(void* key, struct avltree* tree, struct avltree* result){
  if (tree->root) {
    int compval = tree->compare(&(tree->root->data), key);
    if (compval > 0) {
      struct avltree temp_left;
      tree_set(&temp_left, tree->root->left, tree->compare);
      /*temp_left->root = tree->root->left;*/
      /*temp_left->compare = tree->compare;*/
      split_lt(key, &temp_left, result);
      return 0;
    } else if (compval < 0) {
      struct avltree temp_right, temp_left, temp_result;
      tree_set(&temp_left, tree->root->left, tree->compare);
      tree_set(&temp_right, tree->root->right, tree->compare);
      split_lt(key, &temp_right, &temp_result);
      join(&(tree->root->data), &temp_left, &temp_result, result);
      return 0;
    } else {
      tree_set(result, tree->root->left, tree->compare);
      return 0;
    }
  }
  tree_set(result, (struct avlnode*)NULL, tree->compare);
  return -1;
}

int split_gt(void* key, struct avltree* tree, struct avltree* result){
  if (tree->root){
    int compval = tree->compare(&(tree->root->data), key);
    if (compval > 0){
      struct avltree temp_right, temp_left, temp_result;
      tree_set(&temp_left, tree->root->left, tree->compare);
      tree_set(&temp_right, tree->root->right, tree->compare);
      split_gt(key, &temp_left, &temp_result);
      join(&(tree->root->data), &temp_right, &temp_result, result);
      return 0;
    } else if (compval < 0) {
      struct avltree temp_right;
      tree_set(&temp_right, tree->root->right, tree->compare);
      split_gt(key, &temp_right, result);
      return 0;
    } else {
      tree_set(result, tree->root->right, tree->compare);
      return 0;
    }
  }
  tree_set(result, (struct avlnode*)NULL, tree->compare);
  return -1;
}

int join(void* key, struct avltree* left_tree, struct avltree* right_tree, struct avltree* result){
  if (key) {
    struct avlnode* new_root = node_create(*(int *)key);
    new_root->left = left_tree->root;
    new_root->right = right_tree->root;
    tree_set(result, new_root, left_tree->compare);
    if (result->root->right)
      result->root->right->parent = result->root;
    if (result->root->right)
      result->root->right->parent = result->root;
    return 0;
  }
  if (!left_tree->root) {
    tree_set(result, right_tree->root, right_tree->compare);
    /*result->root = right_tree->root;*/
    /*result->compare = right_tree->compare;*/
    return 0;
  }
  else {
    tree_set(result, left_tree->root, left_tree->compare);
    struct avltree temp_right;
    tree_set(&temp_right, left_tree->root->right, left_tree->compare);
    struct avltree temp_result;
    join(key, &temp_right, right_tree, &temp_result);
    result->root->right = temp_result.root;
    if (result->root->right)
      result->root->right->parent = result->root;
    return 0;
  }
  return -1;
}

int set_union(struct avltree *tree1, struct avltree *tree2, struct avltree* result){
  if (!tree1->root){
    tree_set(result, tree2->root, tree2->compare);
    return 0;
  }else if (!tree2->root){
    tree_set(result, tree1->root, tree1->compare);
    return 1;
  }else{
    struct avltree left;
    struct avltree right;
    split_gt(&(tree2->root->data), tree1, &right);
    split_lt(&(tree2->root->data), tree1, &left);

    struct avltree temp_right, temp_left, temp_left_result, temp_right_result;
    tree_set(&temp_left, tree2->root->left, tree2->compare);
    tree_set(&temp_right, tree2->root->right, tree2->compare);

    set_union(&left, &temp_left, &temp_left_result);
    set_union(&right, &temp_right, &temp_right_result);
    join(&(tree2->root->data), &temp_left_result, &temp_right_result, result);
    result->root->height = 0;
    update_heights(result->root);
    rebalance_tree(result);
    return 0;
  }
}

int set_difference(struct avltree* tree1, struct avltree* tree2, struct avltree* result){
  if (!tree1->root){
    tree_set(result, tree1->root, tree1->compare);
    return 0;
  } else if (!tree2->root) {
    tree_set(result, tree1->root, tree1->compare);
    return 0;
  } else {
    struct avltree left, right;
    split_gt(&(tree2->root->data), tree1, &right);
    split_lt(&(tree2->root->data), tree1, &left);

    struct avltree tree2_left, tree2_right;
    struct avltree temp_left_result, temp_right_result;
    tree_set(&tree2_left, tree2->root->left, tree2->compare);
    tree_set(&tree2_right, tree2->root->right, tree2->compare);

    set_difference(&left, &tree2_left, &temp_left_result);
    set_difference(&right, &tree2_right, &temp_right_result);

    join(NULL, &temp_left_result, &temp_right_result, result);

    result->root->height = 0;
    update_heights(result->root);
    rebalance_tree(result);
    return 0;
  }
}

int main() {
  int i = 8, j = 5, k = 2, l = 7, o = 4, m = 11, p = 12, q = 9, r = 6, s = 10;
  struct avltree* test_tree = tree_create(num_compare, &l);
  struct avltree* test_tree2 = tree_create(num_compare, &m);
  /*struct avlnode *lt, *gt;*/
  /*test_tree->compare = num_compare;*/
  /*test_tree2->compare = num_compare;*/
  /*insert(test_tree, &l);*/
  insert(test_tree, &i);
  insert(test_tree, &o);
  struct avlnode* node = avl_remove(test_tree, &o);
  if(node)
    free(node);
  insert(test_tree, &j);
  insert(test_tree, &k);
  print_tree(test_tree->root, 0, "(%d) %d\n");
  puts("\n");

  /*insert(test_tree2, &m);*/
  insert(test_tree2, &p);
  insert(test_tree2, &l);
  insert(test_tree2, &q);
  insert(test_tree2, &r);
  insert(test_tree2, &s);
  print_tree(test_tree2->root, 0, "(%d) %d\n");
  /*avl_remove(test_tree, &l);*/
  /*print_tree(test_tree->root, 0, "(%d) %d\n");*/
  puts("\n");
  /*lt = split_lt(test_tree->root, &j, num_compare);*/
  /*gt = split_gt(test_tree->root, &j, num_compare);*/
  struct avltree union_tree, diff_tree;
  set_union(test_tree, test_tree2, &union_tree);
  set_difference(test_tree, test_tree2, &diff_tree);
  print_tree(union_tree.root, 0, "(%d) %d\n");
  puts("\n");
  print_tree(diff_tree.root, 0, "(%d) %d\n");
  puts("\n");
  /*print_tree(gt, 0, "(%d) %d\n");*/

  node_destroy_recursive(test_tree->root);
  node_destroy_recursive(test_tree2->root);
  return 0;
}

