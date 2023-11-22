#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 100

int array[SIZE];

void fillArrayWithRandomNumbers(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) array[i] = rand()%100;
}

void printArray(int arr[SIZE]) {
    for(int i = 0; i<SIZE; i++) printf("%5d", arr[i]);
    printf("\n");
}

typedef struct StartEndIndexes {
    int start;
    int end;
} StartEndIndexes;

// Runs mergesort on the array segment described in the
// argument. Spawns two threads to mergesort each half
// of the array segment, and then merges the results.
void merge(int arr[], int start, int mid, int end) {
    int temp[SIZE];
    int i = start, j = mid + 1, k = 0;

    while (i <= mid && j <= end) {
        if (arr[i] <= arr[j]) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }

    while (i <= mid) temp[k++] = arr[i++];
    while (j <= end) temp[k++] = arr[j++];

    for (i = start, k = 0; i <= end; i++, k++) arr[i] = temp[k];
}


void* mergeSort(void* args) {
    StartEndIndexes *sei = (StartEndIndexes*) args;
    int start = sei->start;
    int end = sei->end;

    if (start < end) {
        int mid = (start + end) / 2;

        StartEndIndexes leftSei = {start, mid};
        StartEndIndexes rightSei = {mid + 1, end};

        pthread_t leftThread, rightThread;

        pthread_create(&leftThread, NULL, mergeSort, &leftSei);
        pthread_create(&rightThread, NULL, mergeSort, &rightSei);

        pthread_join(leftThread, NULL);
        pthread_join(rightThread, NULL);

        merge(array, start, mid, end);
    }
    return NULL;
}

int main() {
    srand(time(0));
    StartEndIndexes sei;
    sei.start = 0;
    sei.end = SIZE - 1;
    
    // 1. Fill array with random numbers.
    fillArrayWithRandomNumbers(array);
    
    // 2. Print the array.
    printf("Unsorted array: ");
    printArray(array);
    
    // 3. Create a 2 threads for merge sort.
    
    // 4. Wait for mergesort to finish.
    
    // 5. Print the sorted array.
    pthread_t mainThread;
    pthread_create(&mainThread, NULL, mergeSort, &sei);
    pthread_join(mainThread, NULL);

    printf("Sorted array:   ");
    printArray(array);
    return 0;
}
