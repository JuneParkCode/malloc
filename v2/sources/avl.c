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
static inline t_key _key(t_node const *node);

/* AVL TREE UTILITIES */

static inline size_t max(size_t a, size_t b);
static size_t height(t_node const *node);
#ifndef DEBUG
static inline ssize_t balance_factor(t_node const *node);
#endif
static size_t update_height(t_node *node);
static t_node *rotate_left(t_node *x);
static t_node *rotate_right(t_node *x);
static t_node *rebalance(t_node *node);
static t_node *_insert(t_node *const node, t_node *head);
static t_node *_remove(t_key_const key, t_node *node);

static inline bool less(t_key_const addr, t_node const *node)
{
	return ((uintptr_t)addr < (uintptr_t)_key(node));
}

static inline bool equal(t_key_const addr, t_node const *node)
{
	return ((uintptr_t)_key(node) <= (uintptr_t)addr &&
			(uintptr_t)addr <
				(uintptr_t)add_addr(_key(node), node->user_space_size));
}

static inline bool greater(t_key_const addr, t_node const *node)
{
	return ((uintptr_t)addr >=
			(uintptr_t)add_addr(_key(node), node->user_space_size));
}

static inline t_key _key(t_node const *node)
{
	return node->addr;
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
 */
static t_node *rotate_left(t_node *x)
{
	t_node *y = x->right;
	t_node *w = y->left;

	y->left = x;
	y->parent = x->parent;
	x->right = w;
	x->parent = y;
	if (w)
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
 */
static t_node *rotate_right(t_node *x)
{
	t_node *y = x->left;
	t_node *w = y->right;

	y->right = x;
	y->parent = x->parent;
	x->left = w;
	x->parent = y;
	if (w)
		w->parent = x;
	update_height(x);
	update_height(y);
	return y;
}

static t_node *rebalance(t_node *node)
{
	ssize_t bf = balance_factor(node);

	if (bf < -1) {							   // Right tree is heavy
		if (balance_factor(node->right) > 0) { // RL -> Rotate Right first
			node->right = rotate_right(node->right);
		}
		node = rotate_left(node);
	} else if (bf > 1) {					  // Left tree is heavy
		if (balance_factor(node->left) < 0) { // LR -> Rotate Left first
			node->left = rotate_left(node->left);
		}
		node = rotate_right(node);
	}
	return node;
}
// ######## BASIC OPERATION IN AVL ########

static t_node *_insert(t_node *const node, t_node *head)
{
	if (head == NULL) {
		return node;
	} else if (less(_key(node), head)) {
		head->left = _insert(node, head->left);
		head->left->parent = head;
	} else if (greater(_key(node), head)) {
		head->right = _insert(node, head->right);
		head->right->parent = head;
	}
	update_height(head);
	return rebalance(head);
}

/**
 * @brief insert node to avl tree.
 *
 * @param node
 * @param head
 */
void insert_node(t_node *const node, t_node **root)
{
	*root = _insert(node, *root);
}

t_node *_remove(t_key_const key, t_node *const node)
{
	if (node == NULL)
		return NULL;
	t_node *ret = NULL;

	if (less(key, node)) {
		node->left = _remove(key, node->left);
		update_height(node);
		ret = rebalance(node);
	} else if (greater(key, node)) {
		node->right = _remove(key, node->right);
		update_height(node);
		ret = rebalance(node);
	} else {
		if (node->left && node->right) {
			t_node *X = node;
			t_node *Y = node->right;
			t_node *Q;

			if (Y->left == NULL) {
				ret = Y;
			} else {
				do {
					Q = Y;
					Y = Y->left;
				} while (Y->left);

				Q->left = Y->right;
				if (Q->left) {
					Q->left->parent = Q;
				}
				Y->right = X->right;
				X->right->parent = Y;
				ret = Q;
			}
			Y->left = X->left;
			X->left->parent = Y;
			if (X->parent) {
				if (X->parent->left == X) {
					X->parent->left = Y;
				} else {
					X->parent->right = Y;
				}
			}
			Y->parent = X->parent;
			update_height(Y);
			ret = rebalance(Y);
		} else if (node->left == NULL && node->right == NULL) {
			ret = NULL;
		} else if (node->left == NULL) {
			ret = node->right;
		} else {
			ret = node->left;
		}
	}
	return ret;
}

/**
 * @brief remove node from tree.
 *
 * @param node
 * @param root
 */
void remove_node(t_node *const node, t_node **const root)
{
	*root = _remove(_key(node), *root);
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
 * @param head
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
	// inorder iteration
	if (head == NULL)
		return;
	iterate_tree(head->left, fn);
	fn(head);
	iterate_tree(head->right, fn);
}

/**
 * @brief iterate tree and mapping node by function
 *
 * @param head
 * @param fn function to map
 */
void map_tree(t_node *head, void(fn)(t_node *))
{
	// inorder iteration
	if (head == NULL)
		return;
	map_tree(head->left, fn);
	fn(head);
	map_tree(head->right, fn);
}