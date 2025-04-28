#include <iostream>
#include <cstdlib>
#include <chrono>
#include <vector>
#include <ctime>

using namespace std;
using namespace chrono;

// Function to fill the array with random values between 0 to 99
void fillWithRandom(int arr[], int n) {
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 100; // random number from 0 to 99
    }
}

// Function to perform vector addition in chunks
void performAddition(unsigned long n, int step) {
    // Dynamically allocating memory for three arrays
    int* arr1 = (int*)malloc(n * sizeof(int));
    int* arr2 = (int*)malloc(n * sizeof(int));
    int* result = (int*)malloc(n * sizeof(int));

    // Filling arr1 and arr2 with random numbers
    fillWithRandom(arr1, n);
    fillWithRandom(arr2, n);

    // Starting the timer before the addition starts
    auto begin = high_resolution_clock::now();

    // Loop through arrays in chunks
    for (int start = 0; start < n; start += step) {
        int end = start + step;
        if (end > n) end = n; // to avoid going out of bounds

        // Perform addition for each element in the current chunk
        for (int idx = start; idx < end; ++idx) {
            result[idx] = arr1[idx] + arr2[idx];
        }
    }

    // Ending the timer after addition is complete
    auto finish = high_resolution_clock::now();

    // Calculating and displaying time taken in seconds
    double time_taken = duration_cast<duration<double>>(finish - begin).count();
    cout << "Execution Time for chunk size " << step << ": " << time_taken << " seconds" << endl;

    // Freeing the memory
    free(arr1);
    free(arr2);
    free(result);
}

int main() {
    // Seeding random number generator
    srand(time(0));

    // Total number of elements in each array
    unsigned long dataSize = 100000000;

    // Different chunk sizes to test performance
    vector<int> steps = {10, 100, 1000, 10000};

    // Running the experiment for each chunk size
    for (int chunk : steps) {
        performAddition(dataSize, chunk);
    }

    return 0;
}