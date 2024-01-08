#include "avl_tree.h"
#include <stdio.h>
#include <stdlib.h>

// cc
// sources/avl.c test/avl_test.c test/util.sc -o avl_test
// -I includes -g3 -D DEBUG

t_node *create_node(void)
{
	t_node *node = malloc(sizeof(t_node));

	node->addr = node;
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	node->user_space_size = sizeof(t_node);
	node->size = sizeof(t_node);
}

void show_node(t_node const *node)
{
	if (node == NULL)
		return;
	printf("%p | parent %p | left %p | right %p | height %zu | bf %zd\n",
		   node->addr, node->parent, node->left, node->right, node->height,
		   balance_factor(node));
}

void set_size(t_node *node)
{
	if (node == NULL)
		return;
	node->allocated_size = 100;
}

void check_size(t_node const *node)
{
	if (node == NULL)
		return;
	printf("%s\n", node->allocated_size == 100 ? "OK" : "FAILED");
}

int main()
{
	int COUNT = 10;
	t_node *head = create_node();
	t_node *addrs[COUNT + 1];

	// test insertion
	printf("\n========TEST insert_node()========\n\n");
	addrs[0] = head;
	for (int i = 1; i <= COUNT; ++i) {
		insert_node((addrs[i] = create_node()), &head);
		printf("%p\n", addrs[i]);
	}
	iterate_tree(head, show_node);

	printf("\n========TEST find_node()========\n\n");
	// test find
	for (int i = 0; i <= COUNT; ++i) {
		t_node *found = find_node((t_key_const)addrs[i], head);
		if (found != addrs[i]) {
			printf("Error on %p <-> %p\n", addrs[i], found);
		} else {
			printf("FOUND %p == %p\n", addrs[i], found->addr);
		}
	}
	// test map
	printf("\n========TEST map_tree()========\n\n");
	map_tree(head, set_size);
	iterate_tree(head, check_size);
	// test removal
	printf("\n========TEST remove_node()========\n\n");
	for (int i = 0; i <= COUNT; ++i) {
		remove_node(addrs[i], &head);
		printf("TREE head %p\n", head);
		iterate_tree(head, show_node);
		printf("\n");
	}
}