#include <stdlib.h>
#include <iostream>
#include <fstream> 
#include <chrono>
#include <iomanip>
using namespace std;
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
	if (begin >= end)
		return; // Returns recursively

	int mid = begin + (end - begin) / 2;
	mergeSort(array, begin, mid);
	mergeSort(array, mid + 1, end);
	merge(array, begin, mid, end);
}

// Function to get the largest element from an array
int getMax(int array[], int n) {
  int max = array[0];
  for (int i = 1; i < n; i++)
    if (array[i] > max)
      max = array[i];
  return max;
}

// Using counting sort to sort the elements in the basis of significant places
void countingSort(int array[], int size, int place) {
  const int max = 10;
  int output[size];
  int count[max];

  for (int i = 0; i < max; ++i)
    count[i] = 0;

  // Calculate count of elements
  for (int i = 0; i < size; i++)
    count[(array[i] / place) % 10]++;

  // Calculate cumulative count
  for (int i = 1; i < max; i++)
    count[i] += count[i - 1];

  // Place the elements in sorted order
  for (int i = size - 1; i >= 0; i--) {
    output[count[(array[i] / place) % 10] - 1] = array[i];
    count[(array[i] / place) % 10]--;
  }

  for (int i = 0; i < size; i++)
    array[i] = output[i];
}

// Main function to implement radix sort
void radixsort(int array[], int size) {
  // Get maximum element
  int max = getMax(array, size);

  // Apply counting sort to sort elements based on place value.
  for (int place = 1; max / place > 0; place *= 10)
    countingSort(array, size, place);
}


int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quick_sort(int arr[], int low, int high) {
    if (low < high) {
        int p = partition(arr, low, high);
        quick_sort(arr, low, p - 1);
        quick_sort(arr, p + 1, high);
    }
}

int main(int argc, char** argv)
{

    int n = atoi(argv[1]);
    int *array_1 = (int*)malloc(n*sizeof(int));
    int *array_2 = (int*)malloc(n*sizeof(int));
    int *array_3 = (int*)malloc(n*sizeof(int));
    srand(time(NULL));
	for (int i = 0; i < n; i++)
	{
		array_1[i] =array_2[i] =array_3[i] = rand() % 2000;
	}
    
    cout<<"start Quick Sort"<<endl;
    auto seq_quick_s = std::chrono::high_resolution_clock::now();
    quick_sort(array_1,0,n-1);
    auto seq_quick_e = std::chrono::high_resolution_clock::now();
    auto qd = std::chrono::duration_cast<std::chrono::milliseconds>(seq_quick_e - seq_quick_s);
    cout<<"start Radix Sort"<<endl;
    auto seq_radix_s = std::chrono::high_resolution_clock::now();
    radixsort(array_2,n);
    auto seq_radix_e = std::chrono::high_resolution_clock::now();
    auto radix_dur = std::chrono::duration_cast<std::chrono::milliseconds>(seq_radix_e - seq_radix_s);
    cout<<"start Merge Sort"<<endl;
    auto seq_mer_s = std::chrono::high_resolution_clock::now();;
    mergeSort(array_3,0,n-1);
    auto seq_mer_e = std::chrono::high_resolution_clock::now();
    auto merdur = std::chrono::duration_cast<std::chrono::milliseconds>(seq_mer_e - seq_mer_s);
    double d1 = static_cast<double>(qd.count()) /1000.0;
    double d2 = static_cast<double>(radix_dur.count())/1000.0;
    double d3 = static_cast<double>(merdur.count())/1000.0;
    cout<<"Quick sort sequential time: "<<setprecision(6) <<fixed<< d1<<" seconds"<<endl;
    cout<<"Radix sort sequential time: "<<setprecision(6) <<fixed<< d2<<" seconds"<<endl;
    cout<<"Merge sort sequential time: "<<setprecision(6) <<fixed<< d3<<" seconds"<<endl;

    return 0;
}