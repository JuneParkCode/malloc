// #include <stdlib.h>
#include <unistd.h>

void print(char *s)
{
	write(1, s, strlen(s));
}

int main(void)
{
	int i;
	char *addr;

	i = 0;
	show_alloc_mem();
	while (i < 1024) {
		addr = (char *)malloc(1024);
		if (addr == NULL) {
			print("Failed to allocate memory\n");
			return (1);
		}
		addr[0] = 42;
		i++;
	}
	show_alloc_mem();
	return (0);
}
