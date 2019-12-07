#include <iostream>
#include <pthread.h>
#include <ctime>
#include<cstdlib>


#define size 1000//array size
#define max_thread 4

using namespace std;

// array of size MAX
int a[size], b[size];
int part = 0;

// Merges two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
void merge_s(int arr[], int chunk_left, int middle, int chunk_right)
{
    int i, j, k;
    int n1 = middle - chunk_left + 1;
    int n2 =  chunk_right - middle;

    /* create temp arrays */
    int L[n1], R[n2];

    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[chunk_left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[middle + 1+ j];


    i = 0; //starting index of left array
    j = 0; //starting index of right array
    k = chunk_left; // First index of merged subarray
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}


void sort_s(int arr[], int chunk_left, int chunk_right)
{
    if (chunk_left < chunk_right)
    {
        int mid = (chunk_left+chunk_right)/2;

        // Sort left array
        sort_s(arr, chunk_left, mid);
        //sort right array
        sort_s(arr, mid+1, chunk_right);
        //merge left and right
        merge_s(arr, chunk_left, mid,chunk_right);
    }
}




// merge_p function for merging two parts
void merge_p(int chunk_left, int middle, int chunk_right)
{
    int* left = new int[middle - chunk_left + 1];
    int* right = new int[chunk_right - middle];

    // n1 is size of left part and n2 is size
    // of right part
    int n1 = middle - chunk_left + 1, n2 = chunk_right- middle, i, j;

    // storing values in left part
    for (i = 0; i < n1; i++)
        left[i] = a[i + chunk_left];

    // storing values in right part
    for (i = 0; i < n2; i++)
        right[i] = a[i + middle + 1];

    int k = chunk_left;
    i = j = 0;

    // merge_p left and right in ascending order
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }

    // insert remaining values from left
    while (i < n1) {
        a[k++] = left[i++];
    }

    // insert remaining values from right
    while (j < n2) {
        a[k++] = right[j++];
    }
}

// merge_p sort function
void sort_p(int chunk_left, int chunk_right)
{
    // calculating mid point of array
    int middle = (chunk_right + chunk_left) / 2;
    if (chunk_left < chunk_right) {

        // calling first half
        sort_p(chunk_left, chunk_right);

        // calling second half
        sort_p(middle + 1, chunk_right);

        // merging the two halves
        merge_p(chunk_left, middle, chunk_right);
    }
}

// thread function for multi-threading
void* sort_p(void* arg)
{
    // which part out of 4 parts
    int thread_part = part++;

    // calculating low and high
    int chunk_left= thread_part * (size / 4);
    int chunk_right = (thread_part + 1) * (size / 4) - 1;

    // evaluating mid point
    int middle = (chunk_left +  chunk_right) / 2;
    if (chunk_left < chunk_right) {
        sort_p(chunk_left, middle);
        sort_p(middle + 1, chunk_right);
        merge_p(chunk_left, middle, chunk_right);
    }
}

// Driver Code
int main()
{
    // generating random values in array
    for (int i = 0; i < size; i++)
    {
        a[i] = rand() % 100;
        b[i] = a[i];
    }

    cout << "unsorted array: ";
    for (int i = 0; i < size; i++)
        cout << b[i] << " ";
    cout<<endl;

    clock_t t3, t4;

    t3=clock();
    sort_s(b, 0, size-1);
    t4=clock();

    // displaying sorted array
    cout << "Sorted array: ";
    for (int i = 0; i < size; i++)
        cout << b[i] << " ";

    // time taken by merge_p sort in seconds
    cout << "Time taken: " << (t4 - t3) /
                              (double)CLOCKS_PER_SEC << endl;


    // t1 and t2 for calculating time for
    // merge_p sort parallel
    clock_t t1, t2;

    t1 = clock();
    pthread_t threads[max_thread];

    // creating 4 threads
    for (int i = 0; i < max_thread; i++)
        pthread_create(&threads[i], NULL, sort_p,
                       (void*)NULL);

    // joining all 4 threads
    for (int i = 0; i < 4; i++)
        pthread_join(threads[i], NULL);

    // merging the final 4 parts
    merge_p(0, (size/ 2 - 1) / 2, size / 2 - 1);
    merge_p(size/ 2, size/2 + (size-1-size/2)/2, size - 1);
    merge_p(0, (size - 1)/2, size - 1);

    t2 = clock();

    // displaying sorted array
    cout << "Sorted array: ";
    for (int i = 0; i < size; i++)
        cout <<a[i] << endl;

    // time taken by merge_p sort in seconds
    cout << endl<<"Time taken: " << (t2 - t1) /
                              (double)CLOCKS_PER_SEC << endl;

    return 0;
}
