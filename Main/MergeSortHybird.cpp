#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include<math.h>
#include <string.h>
#include <iostream>
using namespace std;


int* merge_two(int* arr1, int n1, int* arr2, int n2)
{
	int* result = (int*)malloc((n1 + n2) * sizeof(int));
	int i = 0;
	int j = 0;
	int k;

	for (k = 0; k < n1 + n2; k++) {
		if (i >= n1)
		{
			result[k] = arr2[j];
			j++;
		}
		else if (j >= n2)
		{
			result[k] = arr1[i];
			i++;
		}

		// Indices in bounds as i < n1
		// && j < n2
		else if (arr1[i] < arr2[j]) {
			result[k] = arr1[i];
			i++;
		}

		// v2[j] <= v1[i]
		else {
			result[k] = arr2[j];
			j++;
		}
	}
	return result;
}

void merge(int *array, int const left, int const mid,
	int const right)
{
	int const length_left = mid - left + 1;
	int const length_right = right - mid;

	// Create temp arrays
	int *leftArray = (int*)malloc(length_left * sizeof(int));
	int	*rightArray = (int*)malloc(length_right * sizeof(int));

	// Copy data to temp arrays leftArray[] and rightArray[]
	for (int i = 0; i < length_left; i++)
		leftArray[i] = array[left + i];
	for (int j = 0; j < length_right; j++)
		rightArray[j] = array[mid + 1 + j];

	int index_left
		= 0, // Initial index of first sub-array
		index_right
		= 0; // Initial index of second sub-array
	int index_total
		= left; // Initial index of merged array

	// Merge the temp arrays back into array[left..right]
	while (index_left < length_left
		&& index_right < length_right) {
		if (leftArray[index_left]
			<= rightArray[index_right]) {
			array[index_total]
				= leftArray[index_left];
			index_left++;
		}
		else {
			array[index_total]
				= rightArray[index_right];
			index_right++;
		}
		index_total++;
	}
	// Copy the remaining elements of
	// left[], if there are any
	while (index_left < length_left) {
		array[index_total]
			= leftArray[index_left];
		index_left++;
		index_total++;
	}
	// Copy the remaining elements of
	// right[], if there are any
	while (index_right < length_right) {
		array[index_total]
			= rightArray[index_right];
		index_right++;
		index_total++;
	}
	free(leftArray);
	free(rightArray);
}

// begin is for left index and end is
// right index of the sub-array
// of arr to be sorted */
void mergeSort(int *array, int const begin, int const end)
{
	if (begin >= end){
		return; // Returns recursively
    }
    // cout<<"calling openmp to sort"<<endl;
	int mid = begin + (end - begin) / 2;
    #pragma omp parallel sections
        {
            #pragma omp section
            {
                // merge_sort(arr, l, m);
                mergeSort(array, begin, mid);
            }
            #pragma omp section
            {
                // merge_sort(arr, m + 1, r);
                mergeSort(array, mid + 1, end);
            }
        }
	merge(array, begin, mid, end);
}


void generate_random_list(int* list, int sz,int seed)
{
	srand(seed);
	for (int i = 0; i < sz; i++)
	{
		list[i] = rand() % 2000;
	}
}
int main(int argc, char* argv[])
{
	int proc, nprocs;
	MPI_Request request;
	MPI_Status status;
	MPI_Init(&argc, &argv);

	//bool match = true;
	MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &proc);
	int n;
	if (argc != 2) {
		if (proc == 0)
			printf("Usage: %s <num of ints>\n", argv[0]);
		MPI_Finalize();
		return -1;
	}
	double total_time = 0.0;
	double start_t = MPI_Wtime();
	if(proc==0)
		n = atoi(argv[1]);

	MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	int local_sz = n / nprocs;
	int* local_list = (int*)malloc(local_sz * sizeof(int));

	generate_random_list(local_list, local_sz,proc+1);
	
	//printf("local size is %d in rank %d\n", local_sz, proc);

	mergeSort(local_list, 0, local_sz - 1);
	double end_t = MPI_Wtime();
	total_time += (end_t - start_t);
	int p = 0;
	int level;
	int shift_nprocs = nprocs;
	while (shift_nprocs >>= 1) p++;


	// the code bellow is used to print data, the printf 
	for (int i = 0; i < nprocs; i++)
	{
		if (i == proc)
		{
			// printf("rank %d:\n", i);
			// for (int j = 0; j < local_sz; j++)
			// 	printf("%d ", local_list[j]);
			// printf("\n");
		}
		MPI_Barrier(MPI_COMM_WORLD);
	}

	if (proc == 0)
		printf("\n");
	int idx;
	int dest;
	if (nprocs == 1)
	{
		printf("\n%d processors sort %d ints elapsed %.6lf seconds\n", nprocs, n, total_time);
		free(local_list);
		MPI_Finalize();
		return 0;
	}
	for (level = 0; level <p; level++)
	{
		int stride = (1 << level);
		for (idx = 0; idx < nprocs; idx += stride)
		{
			if ((idx % (1 << (level + 1))) == 0)
				dest = idx;
			else
				dest = idx - (1 << level);
			//idx 0,2,4,6, dest 0,2,4,6
			//idx 1,3,5,7, dest 0,2,4,6
			if (proc == dest && proc == idx)
			{
				start_t = MPI_Wtime();
				int* received = (int*)malloc(local_sz*stride * sizeof(int));

				MPI_Recv(received, local_sz*stride,
					MPI_INT, dest+stride, 0,
					MPI_COMM_WORLD, &status);
				int* out= merge_two(received, local_sz*stride, local_list, local_sz*stride);
				free(received);
				free(local_list);
				local_list = out;
				end_t = MPI_Wtime();
				total_time += (end_t - start_t);
				// for (int k = 0; k < 2 * local_sz*stride; k++)
				// {
				// 		printf("%d ", local_list[k]);
				// }
				// printf("\n");

			}
			else if(proc!=dest&&proc==idx)
			{

				MPI_Isend(local_list, local_sz*stride, MPI_INT, dest,0, MPI_COMM_WORLD, &request);
			}

		}
		MPI_Barrier(MPI_COMM_WORLD);
	}


	if(proc==0)
	printf("\n%d processors sort %d ints elapsed %.6lf seconds\n", nprocs, n, total_time);
	free(local_list);
	MPI_Finalize();
	return 0;

}