#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  // TODO: initialize struct if needed
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *nil_node = (node_t*)calloc(1, sizeof(node_t));

  nil_node->color = RBTREE_BLACK;
  p -> root = nil_node;
  p -> nil = nil_node;

  return p;
}

void left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if(y->left != t->nil) {    
    y->left->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil) {
    t->root = y;
  } else if(x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;
  if(y->right != t->nil) {
    y->right->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil) {
    t->root = y;
  } else if(x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }
  y->right = x;
  x->parent = y;
}

void postorder(rbtree *t, node_t *ptr){
    if (ptr == t->nil){
        // free(ptr->parent);
        return;
    }
    postorder(t, ptr->left); 
    postorder(t, ptr->right);
    free(ptr);
}


void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
    postorder(t, t->root);
    free(t->nil);
    free(t);
}


void rbtree_insert_fixup(rbtree *t, node_t * new_node){
  while (new_node->parent->color == RBTREE_RED){
    if(new_node->parent == new_node->parent->parent->left){
      node_t *y = new_node->parent->parent->right;
      if(y->color == RBTREE_RED){
        new_node->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      } else{
          if(new_node == new_node->parent->right){
            new_node = new_node->parent;
            left_rotate(t, new_node);
          }
          new_node->parent->color = RBTREE_BLACK;
          new_node->parent->parent->color = RBTREE_RED;
          right_rotate(t, new_node->parent->parent);
      } 
    }else{
      //"right 와 left를 바꾼 경우와 같다.
      node_t *y = new_node->parent->parent->left;
      if(y->color == RBTREE_RED){
        new_node->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        new_node = new_node->parent->parent;
      }else{
        if(new_node == new_node->parent->left){
          new_node = new_node->parent;
          right_rotate(t, new_node);
        }
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent->color = RBTREE_RED;
        left_rotate(t, new_node->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *x = t -> root;
  node_t *y = t -> nil;

  while (x != t->nil){
    y = x;
    if(key < x->key){
      x = x -> left;
    } else {
      x = x -> right;
    }
  }

  node_t *new_node = (node_t*)calloc(1, sizeof(node_t));
  new_node->key = key;
  new_node->parent = y;

  if(y == t->nil){
    t->root = new_node;
  } else if(key < y->key){
    y->left = new_node;
  } else {
    y ->right = new_node;
  }

  new_node->left = t->nil;
  new_node->right = t->nil;
  new_node->color = RBTREE_RED;

  rbtree_insert_fixup(t, new_node);
  return new_node;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *find_node = t->root;

  //찾는 노드의 주소가 nill의 주소가 아닐때 까지
  while (find_node != t->nil){
    if (find_node->key == key){
      return find_node;
    } else if (find_node->key < key){
      find_node = find_node->right;

    } else if(find_node->key > key){
      find_node = find_node->left;
    }    
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t * node = t->root;
  while(node ->left != t->nil){
    node = node ->left;
  }
  return node;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *node = t->root;
  while (node->right != t->nil){
    node = node->right;
  }
  return node;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil){
      t->root = v;
  }else if(u == u->parent->left){
    u -> parent -> left = v;
  }else{
    u -> parent->right = v;
  } 
  v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree *t, node_t *x){
  node_t *w;
  while(x != t->root && x->color == RBTREE_BLACK){
    if(x == x->parent->left){
      w = x->parent->right;
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if(w->right->color == RBTREE_BLACK){
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }

    } else{
      w = x->parent->left;
      if(w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if(w->left->color == RBTREE_BLACK){
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}


node_t* tree_minimum(node_t *x, rbtree *t){
  while(x->left != t->nil){
    x = x->left;
  }
    return x;
}

int rbtree_erase(rbtree *t, node_t *z) {
  node_t* y = z;
  node_t* x;
  color_t erase_node_original_color = y->color;

  if(z->left == t->nil){
    x = z->right;
    rbtree_transplant(t, z, z->right);
  }else if(z->right == t->nil){
    x = z->left;
    rbtree_transplant(t, z, z->left);      
  }else{
    y = tree_minimum(z->right, t);
    erase_node_original_color = y->color;
    x = y->right;
    if(y->parent == z){
      x->parent = y;
    }else{
      rbtree_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    rbtree_transplant(t,z,y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (erase_node_original_color == RBTREE_BLACK){
    rbtree_delete_fixup(t, x);
  }
  free(z);
  z = NULL;

  return 0;
}

void get_array(node_t *cur, node_t *nil, key_t *arr, int *index) {
  if(cur == nil) 
  return;

  get_array(cur->left, nil, arr, index);
  arr[(*index)++] = cur->key;
  get_array(cur->right, nil, arr, index);
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  size_t *index = calloc(1, sizeof(size_t)); 
  get_array(t->root, t->nil, &arr[0], index);
  free(index);
  return 0;
}