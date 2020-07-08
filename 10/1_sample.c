#include <stdio.h>

void swap(int *a, int *b)
{
  int tmp;

  tmp = *a;
  *a = *b;
  *b = tmp;
}

/**
 * stupid sort for testing.
 */
void b_sort(int p[], int left, int right)
{
  int i, j;

  for (i = left; i <= right; i++) {
    printf("%d,%d,%d,%d,%d\n", p[0], p[1], p[2], p[3], p[4]);
    for (j = left; j <= right - 1; j++) {
      if (p[j] > p[j + 1])
        swap(&p[j], &p[j + 1]);
    }
  }
}

int main()
{
  int data[] = {3, 2, 1, 4, 5};
  int length = sizeof(data) / sizeof(data[0]);  /* See K&R p.164 */
  int i;

  b_sort(data, 0, length - 1);

  for (i = 0; i < length; i++) {
    printf("%d ", data[i]);
  }
  printf("\n");

  return 0;
}
