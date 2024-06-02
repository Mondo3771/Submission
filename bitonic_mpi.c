#include <time.h>
// #include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

// this will swap the two values, they will point to their respective values
void swap(int *p1, int *p2)
{
    int temp;
    temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

int partition(int arr[], int low, int high)
{
    // choose the last elemeent as the pivot element
    int pivot = arr[high];

    // this will tell use where the pivot element is going to be placed
    int i = (low - 1);

    for (int j = low; j <= high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    // this is swapping the pivot element with the element at i + 1
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high)
{
    // when low is less than high
    if (low < high)
    {
        // pi is the partition return index of pivot

        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
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

// fisrt i must convert an array into a bitonic sequence
void compAndSwap(int a[], int i, int j, bool dir)
{
    if (dir == (a[i] > a[j]))
        swap(&a[i], &a[j]);
}

/*It recursively sorts a bitonic sequence in ascending order,
  if dir = 1, and in descending order otherwise (means dir=0).
  The sequence to be sorted starts at index position low,
  the parameter cnt is the number of elements to be sorted.*/
void bitonicMerge(int a[], int low, int cnt, bool dir)
{
    if (cnt > 1)
    {
        int k = cnt / 2;
        for (int i = low; i < low + k; i++)
            compAndSwap(a, i, i + k, dir);
        bitonicMerge(a, low, k, dir);
        bitonicMerge(a, low + k, k, dir);
    }
}
void bitonicMerge_Seq(int a[], int low, int cnt, bool dir)
{
    if (cnt > 1)
    {
        int k = cnt / 2;
        for (int i = low; i < low + k; i++)
            compAndSwap(a, i, i + k, dir);
        bitonicMerge_Seq(a, low, k, dir);
        bitonicMerge_Seq(a, low + k, k, dir);
    }
}

/* This function first produces a bitonic sequence by recursively
    sorting its two halves in opposite sorting orders, and then
    calls bitonicMerge to make them in the same order */
void bitonicSort_Seq(int a[], int low, int cnt, bool dir)
{
    if (cnt > 1)
    {
        int k = cnt / 2;
        bitonicSort_Seq(a, low, k, 1);
        bitonicSort_Seq(a, low + k, k, 0);
        bitonicMerge_Seq(a, low, cnt, dir);
    }
}

void recursiveBitonic(int a[], int low, int cnt, int s, int dir)
{
    if (s > 1)
    {
        int half = cnt / 2;
        int half_s = s / 2;
        recursiveBitonic(a, low, half, half_s, 1);
        recursiveBitonic(a, low + half, half, half_s, 0);
    }
    bitonicMerge(a, low, cnt, dir);
}

void bitonicSort(int a[], int low, int cnt, bool dir, int rank, int s)
{
    MPI_Bcast(&cnt, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int *recv = (int *)malloc(cnt / s * sizeof(int));
    MPI_Scatter(a, cnt / s, MPI_INT, recv, cnt / s, MPI_INT, 0, MPI_COMM_WORLD);
    if (s == 1)
    {
        dir = 1;
    }
    else
    {
        dir = rank % (s / 2) == 0 ? 1 : 0;
    }
    bitonicSort_Seq(recv, 0, cnt / s, dir);
    MPI_Gather(recv, cnt / s, MPI_INT, a, cnt / s, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0)
    {
        // this is for 8 processors
        recursiveBitonic(a, 0, cnt, s, dir);
    }
}

int main(int argc, char *argv[])
{
        MPI_Init(&argc, &argv);
        int rank;
        int s;
        int *arr_MPI;
        int n;
        MPI_Comm_size(MPI_COMM_WORLD, &s);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        if (rank != 0)
        {
            bitonicSort(arr_MPI, 0, n, 1, rank, s);
        }
        else
        {
            int p = atoi(argv[1]);
            int size = pow(2, p);
            int *arr_serial = (int *)malloc(size * sizeof(int));
            arr_MPI = (int *)malloc(size * sizeof(int));
            int *array = (int *)malloc(size * sizeof(int));
            int n = size;
            double q_time;
            if (arr_serial == NULL || arr_MPI == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(1);
            }
            srand(time(0));
            for (int i = 0; i < size; i++)
            {
                array[i] = rand() % size + 1;
                arr_serial[i] = array[i];
                arr_MPI[i] = array[i];
            }
            double start, end;
            start = MPI_Wtime();
            quickSort(arr_serial, 0, n);
            end = MPI_Wtime();
            double total_time = end - start;
            q_time = total_time;
            printf("Time taken by Serial: %f\n", total_time);
            isSorted(arr_serial, n) ? printf("Sorted\n") : printf("Not Sorted\n");
            start, end;
            start = MPI_Wtime();
            bitonicSort(arr_MPI, 0, n, 1, rank, s);
            end = MPI_Wtime();
            total_time = end - start;
            printf("Time taken by MPI: %f\n", total_time);
            isSorted(arr_MPI, n) ? printf("Sorted\n") : printf("Not Sorted\n");
            printf("Speedup: %f\n", q_time / total_time);
            printf("\n");
            free(arr_MPI);
            free(arr_serial);
            free(array);
        }
        MPI_Finalize();

    return 0;
}
