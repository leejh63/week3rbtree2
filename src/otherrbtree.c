#include "rbtree.h"

#include <stdlib.h>

void insert_fixup(rbtree *, node_t *);
void transplant(rbtree *, node_t *, node_t *);
node_t* tree_minimum(rbtree *, node_t *);
void delete_fixup(rbtree *, node_t *);
int rbtree_to_array_recur(const rbtree *, node_t *, key_t *, int);

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t*)calloc(1, sizeof(node_t));
  if (p == NULL || nil == NULL)
  {
    free(p);
    free(nil);
    return NULL;
  }
  
  nil -> color = RBTREE_BLACK;
  nil -> key = 0;
  nil -> parent = nil;
  nil -> left = nil;
  nil -> right = nil;

  p->nil = nil;
  p->root = nil;

  return p;
}

void delete_rbtree(rbtree *t) {
  if (t == NULL || t->root == t->nil) {
    if (t != NULL) {
      free(t->nil);
      free(t);
    }
    return;
  }

  int stack_size = 100;
  node_t *current = t->root;
  node_t **stack = (node_t **)malloc(sizeof(node_t *) * stack_size);
  int top = 0;
  if (stack == NULL) {
    return;
  }

  stack[top++] = current;
  while (top > 0) {
    current = stack[--top];
    if (current->right != NULL && current->right != t->nil) { 
      if (stack_size <= top) {
        stack_size *= 2;
        node_t **new_stack = (node_t **)realloc(stack, sizeof(node_t *) * stack_size);
        if (new_stack == NULL) {
          free(stack);
          free(t->nil);
          free(t);
          return;
        }
        stack = new_stack;
      }
      stack[top++] = current->right; 
    }
    if (current->left != NULL && current->left != t->nil) { 
      if (stack_size <= top) {
        stack_size *= 2;
        node_t **new_stack = (node_t **)realloc(stack, sizeof(node_t *) * stack_size);
        if (new_stack == NULL) {
          free(stack);
          return;
        }
        stack = new_stack;
      }
      stack[top++] = current->left; 
    }

    free(current);
  }

  free(stack);
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  struct node_t *new_node = (node_t*)calloc(1, sizeof(struct node_t));
  if (new_node == NULL)
  { return NULL; }

  new_node->color = RBTREE_RED;
  new_node->key = key;
  new_node->parent = t->nil;
  new_node->left = t->nil;
  new_node->right = t->nil;

  if (t->root == t->nil)
  {
    t->root = new_node;
    t->root->color = RBTREE_BLACK;
    return new_node;
  }

  struct node_t *curr = t->root;
  struct node_t *prev = t->nil;

  while (curr != t->nil)
  {
    prev = curr;
    if (key < curr->key)
    { curr = curr->left; }
    else
    { curr = curr->right; }
  }
  new_node->parent = prev;

  if (key < prev->key)
  { prev->left = new_node; }
  else
  { prev->right = new_node; }

  insert_fixup(t, new_node);
  return new_node;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *current = t->root;
  
  while (current != t->nil && key != current->key)
  {
    if(key < current->key)
    { current = current->left; }
    else
    { current = current->right; }
  }
  if (current == t->nil) {
    return NULL;
  }
  
  return current;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;

  if (current == t->nil)
  { return t->nil; }

  while (current->left != t->nil)
  {
    current = current->left;
  }

  return current;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *current = t->root;

  if (current == t->nil)
  {
    return t->nil;
  }

  while (current->right != t->nil)
  {
    current = current->right;
  }

  return current;
}

int rbtree_erase(rbtree *t, node_t *p) {
  if (p == NULL || p == t->nil)
  {
    return -1;
  }
  // TODO: implement erase
  node_t *del = p;
  color_t original_color = del->color;
  node_t *base;

  if (p->left == t->nil)
  {
    base = p->right;
    transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    base = p->left;
    transplant(t, p, p->left);
  }
  else
  {
    del = tree_minimum(t, p->right);
    original_color = del->color;
    base = del->right;
    if (del != p->right)
    {
      transplant(t, del, del->right);
      del->right = p->right;
      del->right->parent = del;
    }
    else
    { base->parent = del; }
    transplant(t, p, del);
    del->left = p->left;
    del->left->parent = del;
    del->color = p->color;
  }
  free(p);

  if(original_color == RBTREE_BLACK)
  { delete_fixup(t, base); }

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  if (t->root == t->nil)
  {
    return 0;
  }
  rbtree_to_array_recur(t, t->root, arr, 0);

  return 0;
}

void left_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil) 
  { y->left->parent = x; }
  y->parent = x->parent;
  if (x->parent == t->nil) 
  { t->root = y; }
  else if (x == x->parent->left)
  { x->parent->left = y; }
  else
  { x->parent->right = y; }
  y->left = x;
  x->parent = y;
}
void right_rotate(rbtree *t, node_t *x)
{
  node_t *y = x->left;          
  x->left = y->right;             
  if (y->right != t->nil)         
  { y->right->parent = x; }       
  y->parent = x->parent;          
  if (x->parent == t->nil)        
  { t->root = y; }                
  else if (x == x->parent->right) 
  { x->parent->right = y; }       
  else                            
  { x->parent->left = y; }        
  y->right = x;                   
  x->parent = y;                  
}
void insert_fixup(rbtree *t, node_t *node)
{
  while (node != t->root && node->parent->color == RBTREE_RED)
  {
    if(node->parent == node->parent->parent->left)
    {
      node_t *uncle = node->parent->parent->right;
      if (uncle != NULL && uncle->color == RBTREE_RED)
      {
        node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }
      else
      {
        if (node == node->parent->right)
        {
          node = node->parent;
          left_rotate(t, node);
        }
        else
        {
          node->parent->color = RBTREE_BLACK;
          node->parent->parent->color = RBTREE_RED;
          right_rotate(t, node->parent->parent);
        }
      }
    }
    else
    {
      node_t *uncle = node->parent->parent->left;
      if(uncle != t->nil && uncle->color == RBTREE_RED)
      {
        node->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }
      else
      {
        if(node == node->parent->left)
        {
          node = node->parent;
          right_rotate(t, node);
        }
        else
        {
          node->parent->color = RBTREE_BLACK;
          node->parent->parent->color = RBTREE_RED;
          left_rotate(t, node->parent->parent);
        }
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}
void transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
  { t->root = v; }
  else if (u == u->parent->left)
  { u->parent->left = v; }
  else
  { u->parent->right = v; }
  v->parent = u->parent;
}
node_t* tree_minimum(rbtree *t, node_t *x)
{
  while (x->left != t->nil)
  {
    x = x->left;
  }
  
  return x;
}
void delete_fixup(rbtree *t, node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK) {
    node_t *w;
    if (x == x->parent->left) {
      w = x->parent->right;

      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }

      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }

        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        if (w->right != t->nil) {
          w->right->color = RBTREE_BLACK;
        }
        left_rotate(t, x->parent);
        x = t->root;
      }
    } else {
      w = x->parent->left;

      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }

      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }

        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        if (w->left != t->nil) {
          w->left->color = RBTREE_BLACK;
        }
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }

  if (x != t->nil) {
    x->color = RBTREE_BLACK;
  }
}

int rbtree_to_array_recur(const rbtree *t, node_t *node, key_t *arr, int index)
{
  if (node != t->nil)
  {
    index = rbtree_to_array_recur(t, node->left, arr, index);
    arr[index++] = node->key;
    index = rbtree_to_array_recur(t, node->right, arr, index);
  }
  return index;
}