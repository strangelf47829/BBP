#include <cstdio>

int main()
{
	// Get input from shell
	printf("Please enter a character: ");
	unsigned char input = getchar();

	// Print binary
	printf("Binary representation: ");
	for (int radix = 0; radix < 8; radix++)
		printf("%d", !!(input & (1 << (7 - radix))));
	printf("\n");

	return 0;
}
