#include "avl_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// cc
// sources/avl.c test/avl_test.c test/util.sc -o avl_test
// -I includes -g3 -D DEBUG

t_node *create_node(void)
{
	t_node *node = malloc(sizeof(t_node));

	node->addr = node;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	node->user_space_size = sizeof(t_node);
	node->size = sizeof(t_node);
}

void *cut(void *ptr)
{
	return (void *)((uintptr_t)(ptr) % (uintptr_t)(1 << 16));
}

void show_node(t_node const *node)
{
	if (node == NULL)
		return;
	printf("%p | parent %p | left %p| right %p| height %zu | bf %zd\n",
		   cut(node->addr), cut(node->parent), cut(node->left),
		   cut(node->right), node->height, balance_factor(node));
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

// https://medium.com/@cdw1432m/이진트리-그리고-콘솔창에-출력하기-f35dc6cbc5d2
typedef enum POS { ROOT, LEFT, RIGHT } POS;
void show_tree(t_node *tree, char *indent, bool is_last, POS position,
			   int depth)
{
	if (tree == NULL)
		return;

	char ind[256];
	char *colors[] = {
		"\e[0;33m", "\e[0;32m", "\e[0;31m", "\e[0;34m", "\e[0;35m",
		// Y G R B M
	};
	char *pos = position == LEFT ? "(L)" : position == RIGHT ? "(R)" : "[ROOT]";

	printf("%s", indent);
	strcpy(ind, indent);

	if (is_last) {
		printf("└──");
		strcat(ind, "  ");
	} else {
		printf("├──");
		strcat(ind, "│ ");
	}

	printf("%s%s %p\e[0m\n", colors[depth % 5], pos, cut(tree->addr));
	if (tree->left && tree->right) {
		show_tree(tree->left, ind, false, LEFT, depth + 1);
		show_tree(tree->right, ind, true, RIGHT, depth + 1);
	} else {
		show_tree(tree->left, ind, true, LEFT, depth + 1);
		show_tree(tree->right, ind, true, RIGHT, depth + 1);
	}
}

int main()
{
	int COUNT = 100;
	t_node *head = NULL;
	t_node *addrs[COUNT];

	// test insertion
	printf("\n========TEST insert_node()========\n\n");
	for (int i = 0; i < COUNT; ++i) {
		insert_node((addrs[i] = create_node()), &head);
	}
	printf("================================\n");
	show_tree(head, " ", true, ROOT, 0);
	iterate_tree(head, show_node);

	printf("\n========TEST find_node()========\n\n");
	// test find
	for (int i = 0; i < COUNT; ++i) {
		t_node *found = find_node((t_key_const)addrs[i], head);
		if (found != addrs[i]) {
			printf("Error on %p <-> %p\n", addrs[i], found);
		} else {
			printf("FOUND %p == %p\n", addrs[i], found->addr);
		}
	}
	// test map
	printf("\n========TEST map_tree()========\n\n");
	show_tree(head, " ", true, ROOT, 0);
	map_tree(head, set_size);
	iterate_tree(head, check_size);
	// test removal
	printf("\n========TEST remove_node()========\n\n");
	for (int i = 0; i < COUNT / 2; ++i) {
		remove_node(addrs[i], &head);
	}
	show_tree(head, " ", true, ROOT, 0);
	printf("================================\n");
	iterate_tree(head, show_node);
	printf("============ HALF ===============\n");
	for (int i = COUNT - 1; i >= COUNT / 2; --i) {
		remove_node(addrs[i], &head);
	}
	printf("================================\n");
	show_tree(head, " ", true, ROOT, 0);
	iterate_tree(head, show_node);
}