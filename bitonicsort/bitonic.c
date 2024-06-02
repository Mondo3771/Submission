#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
void swap(int *p1, int *p2)
{
	int temp;
	temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}
bool isSorted(int arr[], int n)
{
	for (int i = 1; i < n; i++)
	{
		if (arr[i] < arr[i - 1])
		{
			return false;
		}
	}
	return true;
}
void bitonicMerge(int a[], int low, int cnt, bool dir)

{

	if (cnt > 1)
	{
		int k = cnt / 2;
		for (int i = low; i < low + k; i++)
		{
			if (dir == (a[i] > a[i + k]))
			{
				swap(&a[i], &a[i + k]);
			}
		}
		bitonicMerge(a, low, k, dir);
		bitonicMerge(a, low + k, k, dir);
	}
}
void bitonicSort(int a[], int low, int cnt, bool dir)
{
	if (cnt > 1)
	{
		int k = cnt / 2;
		bitonicSort(a, low, k, 1);
		bitonicSort(a, low + k, k, 0);
		bitonicMerge(a, low, cnt, dir);
	}
}

int main(int argc, char *argv[])
{
	int p = atoi(argv[1]);
	int num_rows = atoi(argv[2]);
	int size = pow(2, p);
	int *arr_serial = (int *)malloc(size * sizeof(int));
	double total_time_s = 0;
	double start, end;
	if (arr_serial == NULL)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(1);
	}
	srand(time(0));
	for (int j = 0; j < num_rows; j++)
	{
		for (int i = 0; i < size; i++)
		{
			arr_serial[i] = rand() % size + 1;
		}
		start = omp_get_wtime();
		bitonicSort(arr_serial, 0, size, 1);
		end = omp_get_wtime();
		total_time_s += end - start;
	}
	double q_time = total_time_s / num_rows;
	printf("Time taken by Bitonic Serial: %f\n", q_time);
	free(arr_serial);
	return 0;
}
