#include <cstdio>

long getNumber(int index)
{
	// Create input
	long input = 0;

	// Get input
	printf("Please enter the value for index %d: ", index);
	scanf("%li", &input);

	// Wejow! I got my input!
	return input;
}

static constexpr int valueArraySize = 10;
long values[valueArraySize];

int main()
{
	// Get each number
	for (int idx = 0; idx < valueArraySize; idx++)
		values[idx] = getNumber(idx);

	int biggestIdx = 0;
	int lowestIdx = 0;

	long long cummulative = 0;
	long average = 0;

	for (int idx = 0; idx < valueArraySize; idx++)
	{
		long value = values[idx];

		// Update values
		biggestIdx = (value > values[biggestIdx]) ? idx : biggestIdx;
		lowestIdx = (value < values[lowestIdx]) ? idx : lowestIdx;
		
		cummulative += value;
	}

	average = cummulative / valueArraySize;

	// Printout statistics
	printf("\n\n** statistics **\n");
	printf("HIGHEST\t\tvalues[%d] = %d\n", biggestIdx, values[biggestIdx]);
	printf("LOWEST\t\tvalues[%d] = %d\n", lowestIdx, values[lowestIdx]);
	printf("CUMMULATIVE\t%d\n", cummulative);
	printf("AVERAGE\t\t%d\n", average);	

	printf("\n\nLast 2 values:\n");
	printf("...\n");
	printf("values[%d] =\t%d\n", valueArraySize - 2, values[valueArraySize - 2]);
	printf("values[%d] =\t%d\n", valueArraySize - 1, values[valueArraySize - 1]);

}
