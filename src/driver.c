#include "rbtree.h"
#include <stdio.h>

void print_tree_structure(rbtree *t, node_t *n, int depth) {
    if (n == t->nil) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        printf("  ");
    }

    printf("Node key: %d, Color: %s, ", n->key, n->color == RBTREE_RED ? "RED" : "BLACK");
    if (n->parent != t->nil) {
        printf("Parent key: %d", n->parent->key);
    } else {
        printf("Parent: NULL");
    }
    printf("\n");

    print_tree_structure(t, n->left, depth+1);
    print_tree_structure(t, n->right, depth+1);
}

int main() {
    // tree init
    rbtree *t = new_rbtree();

    rbtree_insert(t, 11);
    rbtree_insert(t, 2);
    rbtree_insert(t, 14);
    rbtree_insert(t, 1);
    rbtree_insert(t, 7);
    rbtree_insert(t, 15);
    rbtree_insert(t, 5);
    rbtree_insert(t, 8);
    rbtree_insert(t, 4);

    // key_t entries[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};

    // for (int i=0; i < 15; i++) {
    //     rbtree_insert(t, entries[i]);
    // }
    // rbtree_insert(t, 2);
    // rbtree_insert(t, 5);
    // rbtree_insert(t, 8);
    // rbtree_insert(t, 10);
    // rbtree_insert(t, 12);
    // rbtree_insert(t, 23);
    // rbtree_insert(t, 24);
    // rbtree_insert(t, 24);
    // rbtree_insert(t, 25);
    // rbtree_insert(t, 34);
    // rbtree_insert(t, 36);
    // rbtree_insert(t, 67);
    // rbtree_insert(t, 156);
    // rbtree_insert(t, 990);

    printf("\ninit tree structure-----------------------------------\n");
    print_tree_structure(t, t->root, 0);
    
    // 왼쪽 회전 실행
    // node_t *rotate_n = rbtree_find(t, 25);
    // left_rotate(t, rotate_n);  // t->root는 회전 축

    // find node
    // node_t *n = rbtree_find(t, 0);
    // printf("\nrbtree_find!-----------------------------\n");
    // printf("Node key: %d, Color: %s, Parent key: %d\n", n->key, n->color == RBTREE_RED ? "RED" : "BLACK", n->parent->key);

    // printf("\nTree structure after right rotation-------------------\n");
    // print_tree_structure(t, t->root, 0);
    
    // find min
    // node_t *min_node = rbtree_min(t);
    // printf("min_node key: %d, Color: %s, Parent key: %d\n", min_node->key, min_node->color == RBTREE_RED ? "RED" : "BLACK", min_node->parent->key);

    // find max
    // node_t *max_node = rbtree_max(t);
    // printf("max_node key: %d, Color: %s, Parent key: %d\n", max_node->key, max_node->color == RBTREE_RED ? "RED" : "BLACK", max_node->parent->key);

    // erase node



    // tree terminate
    delete_rbtree(t);
    return 0;
}