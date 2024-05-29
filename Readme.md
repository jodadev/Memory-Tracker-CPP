# Memory Tracker C++(MTcpp) - Prevent Data Leaks
### Version 1.0
## Overview
The MemoryTracker program is designed to track memory allocations and deallocations in a C++ application. It provides custom new and delete operators to keep track of memory usage and identify potential memory leaks. This is particularly useful for debugging and ensuring the stability of your applications.

Additionally, the MemoryTracker allows users to test functions for memory leaks by passing the function into the "test" method, which supports functions with up to five arguments by value.
## Files
- <b>MemoryTracker.h:</b> Defines the MemoryTracker class for tracking memory allocations and deallocations.
- <b>GlobalOverloads.h:</b> Provides global overloads for new and delete operators to integrate with the MemoryTracker.
- <b>MTPackage.h:</b> Includes both MemoryTracker.h and GlobalOverloads.h.
- <b>main.cpp:</b> Example usage of the MemoryTracker.
- <b>build.bat:</b> Used to build a simple lightweight .exe file for the demo(main.cpp) program.
## Prerequisites
- A C++ complier (GCC, Clang, MSVC, etc)
## Demo Setup
1. Clone the repository:<br>
```
git clone https://github.com/jodadev/Memory-Tracker-CPP.git
```
2. Change directory:<br>
```
cd ./Memory-Tracker-CPP
```
3. Build and Run the project:<br>
For Windows:<br>
```
./build_demo.bat
```
<br>For Linux/Mac:<br>
```
./build_demo.sh
```

## Project Setup
If you wish to use in you own project, follow the steps to ensure proper setup:
- Copy <code>GlobalOverloads.h</code> and <code>MemoryTracker.h</code> into your include folder.
- Copy <code>MTPackage.h</code> into your header folder.
- Simply include the <code>MTPackage.h</code> into your cpp file to use!

## Usage
### Including Memory Tracker
Include the MTPackage.h in your source files to enable memory tracking:<br>
```cpp
#include "MTPackage.h"
```
### Example - Testing Functions
Here's an example of how to use the memory tracking system to test functions:
```cpp
#include "MTPackage.h"

void myFunc(int x) {
    // We create leak, MT will detect and track this.
    int* x = new int(25 + x);

    // No leak, MT will not track this.
    int* arr = new int[10];
    delete[] arr;
}

int anotherFunc(int x) {
    // We create leak, MT will detect and track this.
    int* x = new int(25 + x);

    // No leak, MT will not track this.
    int* arr = new int[10];
    delete[] arr;

    return* x;
}

int main() {
    // Get instance of the tracker
    MemoryTracker& tracker MemoryTracker::getInstance();

    /* 
        - Call MemoryTracker's test function
        - Define return and arg types for your function 
        - Pass reference to your function as first arg
        - Pass any args required for function preceding the first arg
    */
    tracker.test<void, int>(&myFunc, 25);
    
    int value = tracker.test<int, int>(&anotherFunc, 25);

    system("pause");
    return 0;
}
```
### Example - Testing Points of Program
Here's an example of how to use the memory tracking system at any point:
```cpp
#include "MTPackage.h"

void myFunc(int x) {
    // We create leak, MT will detect and track this.
    int* x = new int(25 + x);

    // No leak, MT will not track this.
    int* arr = new int[10];
    delete[] arr;
}

int main() {
    // Get instance of the tracker
    MemoryTracker& tracker MemoryTracker::getInstance();

    // Some function
    myFunc(25);

    // Point in program to check for any leaks
    tracker.report();

    system("pause");
    return 0;
}
```
## Output
The program will output any memory leaks detected:
```
>>> Memory leaks detected <<<
    - Leak at address x0.. of size 4 bytes. Allocated at ./src/demo.cpp:line(3)
```
## Troubleshooting
- <b>Compilation Issues:</b> Ensure all header and source files are correctly included and paths are set up correctly.
- <b>Debugging:</b> Use the logging statements to trace memory allocation and deallocation calls.
## Contributing
Currently contributing is not allowed and any pull request will be denied. This project's purpose is portfolio based.
## License
This project is licensed under the MIT License.