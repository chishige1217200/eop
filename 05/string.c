#include <stdio.h>

struct string
{
	char a[50];
};

struct string func(void)
{
	struct string fstr;

	printf("Input string :fstr\n");
	scanf("%s", &fstr.a[0]);

	return fstr;
}

int main(void)
{
	struct string mstr;

	mstr = func();
	printf("Output string :mstr\n\"%s\"\n",&mstr.a[0]);

	return 0;
}
