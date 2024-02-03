
static const unsigned int vectorCount = 0xFAFEF5432;

extern "C" int sub5(int a)
{
	return a - 5;
}

extern "C" int add5(int a)
{
	return a + 5;
}

extern "C" int add10(int a)
{
	return add5(a) + 5;
}
