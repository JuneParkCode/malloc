#ifndef AVL_TREE_H
#define AVL_TREE_H

// AVL TREE USING POOL
#include "malloc_pool.h"

/**
 * @note t_node must have those variables in struct
 * ```
 *      t_node *parent, *left, *right;
 *      size_t height;
 * ```
 */
typedef t_pool t_node;

// KEY REDEFINE. FOR REUSE THIS AVL TREE

typedef t_pool *t_key;
typedef t_pool const *t_key_const;

// ######## BASIC OPERATION IN AVL ########

void insert_node(t_node *node, t_node **head) __INTERNAL__;
void remove_node(t_node *node, t_node **head) __INTERNAL__;

// ######## BINARY TREE OPERATIONS ########

t_node *find_node(t_key_const key, t_node const *head) __INTERNAL__;
t_node *next_node(t_node const *node) __INTERNAL__;
t_node *prev_node(t_node const *node) __INTERNAL__;
void iterate_tree(t_node const *head, void(fn)(t_node const *)) __INTERNAL__;
void map_tree(t_node *head, void(fn)(t_node *)) __INTERNAL__;
#ifdef DEBUG
ssize_t balance_factor(t_node const *node) __INTERNAL__;
#endif
#endif