#include <cstdio>

long getNumber()
{
	long number = 0;

	printf("Please provide me with a \e[0;32mvalid\e[0;37m int!\t\t\e[0;31m");
	scanf("%li", &number);
	printf("\e[0;37m");

	return number;
}

void showBinary(const char *msg, char bits, long number)
{
	// Dont give me nullptr or i WILL make you sad >:D
	printf("%s\t0b", msg);

	for (char bit = 0; bit < bits; bit++)
		printf("%d", !!(number & (1 << (bits - bit))));

	printf("\n");
}

int main()
{

	printf("Please provide a valid integer! C++ Doesn't like it if you dont!\n");
	printf("+------------------+\n");
	printf("| Please give me a |\n");
	printf("| valid integer or |\n");
	printf("| I will blow up   |\n");
	printf("| in your face!    |\n");
	printf("+--\\/--------------+\n");
	printf("   C++\n");


	long numberOne = getNumber();
	long numberTwo = getNumber();
	// I already am numb
	// Can't get any numb-er, HAHAHAHAHAHAHAHA
	
	// Get or-ed BIATCH
	long ored = numberOne | numberTwo;
	long anded = numberOne & numberTwo;

	// Now print out the numbers
	showBinary("Anded", 32, anded);
	showBinary("Ored", 32, ored);

	return 0;

}
