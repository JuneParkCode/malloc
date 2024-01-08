#include "avl_tree.h"

/*
	해당 AVL Tree 의 경우 Pool 을 기반의로 작동하도록 설계되었다.
	따라서, 다른 형태에 적용하기 위해서는 less, equal, greater 에 대한 재정의가
	필요하다.
*/

/* MUST BE REDEFINED WHEN IMPLEMENT OTHER PERPOSE */

static inline bool less(t_key_const addr, t_node const *node);
static inline bool equal(t_key_const addr, t_node const *node);
static inline bool greater(t_key_const addr, t_node const *node);

/* AVL TREE UTILITIES */

static inline size_t max(size_t a, size_t b);
static size_t height(t_node const *node);
#ifndef DEBUG
static inline ssize_t balance_factor(t_node const *node);
#endif
static size_t update_height(t_node *node);
static t_node *rotate_left(t_node *x);
static t_node *rotate_right(t_node *x);
static t_node *rebalance(t_node **node);

static inline bool less(t_key_const addr, t_node const *node)
{
	return ((uintptr_t)addr < (uintptr_t)node->addr);
}

static inline bool equal(t_key_const addr, t_node const *node)
{
	return ((uintptr_t)node->addr <= (uintptr_t)addr &&
			(uintptr_t)addr <
				(uintptr_t)add_addr(node->addr, node->user_space_size));
}

static inline bool greater(t_key_const addr, t_node const *node)
{
	return ((uintptr_t)addr >=
			(uintptr_t)add_addr(node->addr, node->user_space_size));
}

ssize_t balance_factor(t_node const *node)
{
	return (ssize_t)height((node)->left) - (ssize_t)height((node)->right);
}

static size_t update_height(t_node *node)
{
	if (node == NULL)
		return -1;
	node->height = (1 + max(height(node->left), height(node->right)));
	return node->height;
}

static inline size_t max(size_t a, size_t b)
{
	return a > b ? a : b;
}
static size_t height(t_node const *node)
{
	if (node == NULL)
		return 0;
	return node->height;
}

/**
 *  TREE STURCTURE  (RR) to balanced
 *     x                           y
 *  T1     y           ->      x        z
 *      w     z             T1   w   T2   T3
 *         T2   T3
 *  DO NOT CARE IN x's PARENT THIS FUNCTION
 */
static t_node *rotate_left(t_node *x)
{
	t_node *y = x->right;
	t_node *z = y->right;
	t_node *w = y->left;

	y->left = x;
	y->parent = x->parent;
	x->parent = y;
	x->right = w;
	if (w != NULL)
		w->parent = x;
	update_height(x);
	update_height(y);
	return y;
}

/**
 *  TREE STURCTURE  (LL) to balanced
 *         x                      y
 *      y    T1       ->      z      x
 *   z    w      		   T2  T3  w   T1
 * T2 T3
 * DO NOT CARE IN x's PARENT THIS FUNCTION
 */
static t_node *rotate_right(t_node *x)
{
	t_node *y = x->left;
	t_node *z = y->left;
	t_node *w = y->right;

	y->left = z;
	y->parent = x->parent;
	x->parent = y;
	x->left = w;
	if (w != NULL)
		w->parent = x;
	update_height(x);
	update_height(y);
	return y;
}

static t_node *rebalance(t_node **node)
{
	ssize_t bf = balance_factor(*node);

	if (bf < -1) {								  // Right tree is heavy
		if (balance_factor((*node)->right) > 0) { // RL -> Rotate Right first
			(*node)->right = rotate_right((*node)->right);
		}
		*node = rotate_left(*node);
	} else if (bf > 1) {						  // Left tree is heavy
		if (balance_factor((*node)->right) < 0) { // LR -> Rotate Left first
			(*node)->left = rotate_left((*node)->left);
		}
		*node = rotate_right(*node);
	}
	return *node;
}
// ######## BASIC OPERATION IN AVL ########

/**
 * @brief insert node to avl tree.
 *
 * @param new_node
 * @param node (head addr)
 */
void insert_node(t_node *const new_node, t_node **const node)
{
	if (*node == NULL) {
		*node = new_node;
		return;
	} else if (less(new_node->addr, (*node)->addr)) {
		insert_node(new_node, &((*node)->left));
		if ((*node)->left == new_node)
			new_node->parent = *node;
	} else if (greater(new_node->addr, (*node)->addr)) {
		insert_node(new_node, &((*node)->right));
		if ((*node)->right == new_node)
			new_node->parent = *node;
	}
	update_height(*node);
	rebalance(node);
}

/**
 * @brief remove node from tree.
 *
 * @param delete_node
 * @param node (head addr)
 */
void remove_node(t_node *const delete_node, t_node **const node)
{
	// traverse from head node.
	if (*node == NULL) {
		return;
	} else if (less(delete_node->addr, (*node)->addr)) {
		remove_node(delete_node, &((*node)->left));
		update_height(*node);
		rebalance(node);
	} else if (greater(delete_node->addr, (*node)->addr)) {
		remove_node(delete_node, &((*node)->right));
		update_height(*node);
		rebalance(node);
	} else { // found, delete node process
		if ((*node)->left == NULL && (*node)->right == NULL) {
			*node = NULL;
		} else if ((*node)->left != NULL && (*node)->right == NULL) {
			(*node)->left->parent = (*node)->parent;
			*node = (*node)->left;
		} else if ((*node)->left == NULL && (*node)->right != NULL) {
			(*node)->right->parent = (*node)->parent;
			*node = (*node)->right;
		} else {
			t_node *prev = prev_node(delete_node); // always exist

			prev->parent = delete_node->parent;
			prev->left = delete_node->left;
			if (delete_node->left != NULL)
				delete_node->left->parent = prev;
			prev->right = delete_node->right;
			if (delete_node->right != NULL)
				delete_node->right->parent = prev;
			*node = delete_node;
		}
	}
	return;
}

// ######## BINARY TREE OPERATIONS ########
/**
 * @brief find node by key (address of block) in pool
 *
 * @param key
 * @param head
 * @return t_node* found node
 */
t_node *find_node(t_key_const key, t_node const *head)
{
	t_node const *node = head;

	while (node) {
		if (less(key, node)) {
			node = node->left;
		} else if (equal(key, node)) {
			return (t_node *)node;
		} else {
			node = node->right;
		}
	}
	return NULL;
}

/**
 * @brief get next node in tree
 *
 * @param node
 * @return t_node*
 */
t_node *next_node(t_node const *node)
{
	if (node == NULL)
		return NULL;
	if (node->right) {
		node = node->right;
		while (node->left) {
			node = node->left;
		}
		return (t_node *)node;
	} else if (node->parent) {
		while (node->parent && node == node->parent->right)
			node = node->parent;
		return node->parent;
	} else {
		return NULL;
	}
}

/**
 * @brief get previous node in tree
 *
 * @param node
 * @return t_node*
 */
t_node *prev_node(t_node const *node)
{
	if (node == NULL)
		return NULL;
	if (node->left) {
		node = node->left;
		while (node->right) {
			node = node->right;
		}
		return (t_node *)node;
	} else if (node->parent) {
		if (node == node->parent->right)
			return node->parent;
		return NULL;
	} else {
		return NULL;
	}
}

/**
 * @brief interate tree. low address to high address
 *
 * @param head
 * @param fn function to iterate
 */
void iterate_tree(t_node const *head, void(fn)(t_node const *))
{
	t_node const *node = head;

	if (node == NULL)
		return;
	// goto minimum
	while (node->left) {
		node = node->left;
	}
	// traverse
	while (node) {
		fn(node);
		node = next_node(node);
	}
	return;
}

/**
 * @brief iterate tree and mapping node by function
 *
 * @param head
 * @param fn function to map
 */
void map_tree(t_node *head, void(fn)(t_node *))
{
	t_node *node = head;

	if (node == NULL)
		return;
	// goto minimum
	while (node->left) {
		node = node->left;
	}
	// traverse
	while (node) {
		fn(node);
		node = next_node(node);
	}
	return;
}