#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Gavin Mobley
// lab 8


int extraMemoryAllocated;

// Function to allocate memory with tracking of extra memory
void *Alloc(size_t sz)
{
    // Update extraMemoryAllocated with the size of memory allocated
    extraMemoryAllocated += sz;
    
    // Allocate memory with extra space to store the size
    size_t* ret = malloc(sizeof(size_t) + sz);
    *ret = sz;
    
    // Print a message indicating the size of memory allocated
    printf("Extra memory allocated, size: %ld\n", sz);
    
    // Return a pointer to the allocated memory (excluding the space for size)
    return &ret[1];
}

// Function to deallocate memory and update the extra memory count
void DeAlloc(void* ptr)
{
    // Retrieve the size of memory allocated
    size_t* pSz = (size_t*)ptr - 1;
    
    // Update extraMemoryAllocated by subtracting the size of memory deallocated
    extraMemoryAllocated -= *pSz;
    
    // Print a message indicating the size of memory deallocated
    printf("Extra memory deallocated, size: %ld\n", *pSz);
    
    // Free the allocated memory (including the space for size)
    free((size_t*)ptr - 1);
}

// Function to merge two sorted subarrays into one sorted array
void merge(int pData[], int l, int m, int r) {
    // Calculate the sizes of the two subarrays
    int n1 = m - l + 1;
    int n2 = r - m;

    // Allocate memory for temporary arrays to store the subarrays
    int* L = (int*)Alloc(n1 * sizeof(int));
    int* R = (int*)Alloc(n2 * sizeof(int));

    // Copy data from the original array to the temporary arrays
    for (int i = 0; i < n1; i++)
        L[i] = pData[l + i];
    for (int j = 0; j < n2; j++)
        R[j] = pData[m + 1 + j];

    // Merge the two subarrays back into the original array
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            pData[k] = L[i++];
        } else {
            pData[k] = R[j++];
        }
        k++;
    }

    // Copy any remaining elements from the left subarray
    while (i < n1) {
        pData[k++] = L[i++];
    }

    // Copy any remaining elements from the right subarray
    while (j < n2) {
        pData[k++] = R[j++];
    }

    // Deallocate memory used for temporary arrays
    DeAlloc(L);
    DeAlloc(R);
}

// Function to implement the merge sort algorithm
void mergeSort(int pData[], int l, int r) {
    if (l < r) {
        // Calculate the midpoint of the array
        int m = l + (r - l) / 2;

        // Recursively sort the two halves
        mergeSort(pData, l, m);
        mergeSort(pData, m + 1, r);

        // Merge the sorted halves
        merge(pData, l, m, r);
    }
}

// Function to parse data from a file into an integer array
int parseData(char *inputFileName, int **ppData) {
    FILE* inFile = fopen(inputFileName,"r");
    int dataSz = 0;
    int i, n, *data;
    *ppData = NULL;

    if (inFile) {
        // Read the size of the array from the file
        fscanf(inFile,"%d\n",&dataSz);
        
        // Allocate memory for the array based on the size read
        *ppData = (int *)malloc(sizeof(int) * dataSz);

        // Check if memory allocation was successful
        if (*ppData == NULL) {
            printf("Cannot allocate memory\n");
            exit(-1);
        }

        // Read the elements of the array from the file
        for (i=0;i<dataSz;++i) {
            fscanf(inFile, "%d ",&n);
            data = *ppData + i;
            *data = n;
        }

        fclose(inFile);
    }

    // Return the size of the array
    return dataSz;
}

void printArray(int pData[], int dataSz) {
    int i, sz = (dataSz > 100 ? dataSz - 100 : 0);
    int firstHundred = (dataSz < 100 ? dataSz : 100);
    printf("\tData:\n\t");
    for (i=0;i<firstHundred;++i) {
        printf("%d ",pData[i]);
    }
    printf("\n\t");

    for (i=sz;i<dataSz;++i) {
        printf("%d ",pData[i]);
    }
    printf("\n\n");
}

int main(void) {
    clock_t start, end;
    int i;
    double cpu_time_used;
    char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };

    for (i=0;i<4;++i) {
        int *pDataSrc, *pDataCopy;
        int dataSz = parseData(fileNames[i], &pDataSrc);

        if (dataSz <= 0)
            continue;

        pDataCopy = (int *)malloc(sizeof(int)*dataSz);

        printf("---------------------------\n");
        printf("Dataset Size : %d\n",dataSz);
        printf("---------------------------\n");

        printf("Merge Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
        extraMemoryAllocated = 0;
        start = clock();
        mergeSort(pDataCopy, 0, dataSz - 1);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
        printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        free(pDataCopy);
        free(pDataSrc);
    }

    return 0;
}
