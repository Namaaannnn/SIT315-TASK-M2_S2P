#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <iomanip>  // For fixed precision formatting

using namespace std;
using namespace std::chrono;

// Function to fill an array with random integers
void generateRandomArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100;  // Random number between 0 and 99
    }
}

// Global atomic index to keep track of which part to process next
atomic<int> current_index(0);

// Thread function to perform vector addition in chunks
void addVectors(int *a, int *b, int *c, int stepSize, int totalSize) {
    while (true) {
        // Atomically get the next start index
        int start = current_index.fetch_add(stepSize);
        if (start >= totalSize) break;

        // Calculate end index safely
        int end = min(start + stepSize, totalSize);
        for (int i = start; i < end; i++) {
            c[i] = a[i] + b[i];  // Perform addition
        }
    }
}

// Function to run one experiment with given chunk size
void startTest(unsigned long totalSize, int stepSize) {
    // Allocate memory for the vectors
    int *a = (int *)malloc(totalSize * sizeof(int));
    int *b = (int *)malloc(totalSize * sizeof(int));
    int *c = (int *)malloc(totalSize * sizeof(int));

    // Fill input vectors with random data
    generateRandomArray(a, totalSize);
    generateRandomArray(b, totalSize);

    // Start measuring time
    auto start = high_resolution_clock::now();

    // Number of threads equal to number of available cores
    int threadCount = thread::hardware_concurrency();
    vector<thread> threadList;

    current_index = 0;  // Reset global index before starting threads

    // Launch threads
    for (int i = 0; i < threadCount; i++) {
        threadList.push_back(thread(addVectors, a, b, c, stepSize, totalSize));
    }

    // Wait for all threads to finish
    for (auto &t : threadList) {
        t.join();
    }

    // Stop measuring time
    auto stop = high_resolution_clock::now();
    double elapsed = duration_cast<duration<double>>(stop - start).count();

    // Print the time taken in the required format
    cout << "Execution Time for chunk size " << stepSize << ": "
         << fixed << elapsed << " seconds" << endl;

    // Free allocated memory
    free(a);
    free(b);
    free(c);
}

int main() {
    unsigned long totalSize = 100000000;  // Size of the vectors
    srand(time(0));  // Seed for randomness

    // Different chunk sizes to test
    vector<int> stepSizes = {10, 100, 1000, 10000};

    // Run the experiment for each chunk size
    for (int stepSize : stepSizes) {
        startTest(totalSize, stepSize);
    }

    return 0;
}