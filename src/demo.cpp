#include "MTPackage.h"
#include <iostream>

// Test Functions
void func1(){
    int* leak1 = new int(5);
    int* leak2 = new int[10];

    int* noLeak1 = new int(10);
    delete noLeak1;

    int* noLeak2 = new int[10];
    delete[] noLeak2;
}

int func2(int x){
    int* leak1 = new int(5);

    int* noLeak2 = new int[10];
    delete[] noLeak2;

    return *leak1 + x;
}

void func3(int x, std::string str){
    int* leak1 = new int(5 + x);
    
    std::string* strPtr = new std::string("Hello");
    delete strPtr;

    std::string* strPtr2 = new std::string("Hello World!");
}

void func4(int x, bool b, float y, double z, std::string str){
    double* leak1 = new double(5.25 + z);
    
    double* arrPtr = new double[5];
    delete arrPtr; 
}


int main() {
    
    MemoryTracker& tracker = MemoryTracker::getInstance(); 
    
    std::cout << "\nTesting function (func1, func2, func3 and func4).." << std::endl;
    
    /* test function (template)..
        The test function is a template allowing you to use different function
        signatures(return type, arg types). 
        
        - There are several overloads to support multiple args.
        - The first defining type is reserved for the return type (note with our example of func1).
        - The following defining types are reserved for args and are in order. 
        - Currently MTcpp supports functions with up to 5 args by value only.
        - Also note, the test function requires a ref to the function you wish to test 
            followed by the values required by your test function, in order.
  
     */
    
    tracker.test<void>(&func1);
    int result = tracker.test<int, int>(&func2, 5);
    tracker.test<void, int, std::string>(&func3, 3, "Hello");
    tracker.test<void, int, bool, float, double, std::string>(&func4, 5, true, 2.5f, 3.8, "Hello");

    std::cout << "\nReporting at a point.." << std::endl;
    // Test the current point rather than a function:
    tracker.report();

    system("pause");
    return 0;
}