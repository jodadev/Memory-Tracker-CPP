#ifndef MEMORY_TRACKER_H
#define MEMORY_TRACKER_H

#include <iostream>
#include <unordered_map>
#include <mutex> 
#include <functional> // For passing func as arg
#include <cstdlib>  // For malloc and free

class MemoryTracker {
private:
    /// @brief Store information about data leak.
    struct Allocation {
        size_t size;
        const char* file;
        int line;
    };

    /// @brief Used to store instances of Allocation struct for each data leak found.
    std::unordered_map<void*, Allocation> allocations;
    /// @brief Used to protect the "allocations" map in a multi-threaded env.
    std::mutex mtx;

    MemoryTracker() {}

    // Prevent copying and assignment for singleton
    MemoryTracker(const MemoryTracker&) = delete;
    MemoryTracker& operator=(const MemoryTracker&) = delete;

public:
    /// @brief Track data leaks by wrapping the "test" function around any function or test points of application using "report" function.
    /// @return Singleton instance of MemoryTracker
    static MemoryTracker& getInstance() {
        static MemoryTracker instance;
        return instance;
    }

    void addAllocation(void* ptr, size_t size, const char* file, int line) {
        std::lock_guard<std::mutex> lock(mtx);
        //std::cout << "Adding allocation: " << ptr << " of size " << size << " at " << file << ":" << line << std::endl;
        allocations[ptr] = {size, file, line};
    }

    void removeAllocation(void* ptr) {
        //std::cout << "Attempting to remove allocation: " << ptr << std::endl;
        std::lock_guard<std::mutex> lock(mtx);  
        auto it = allocations.find(ptr);
        if (it != allocations.end()) {
            //std::cout << "Found ptr: " << ptr << " with size " << it->second.size << " allocated at " << it->second.file << ":" << it->second.line << std::endl;
            allocations.erase(it);
            //std::cout << "Removed ptr: " << ptr << std::endl;
        } else {
            std::cerr << "Warning: attempt to remove unknown allocation: " << ptr 
                << "\n\t(If showing after program termination, it pertains to the release of program memory.)" << std::endl;
        }
        //std::cout << "Unlocked mutex after removing allocation: " << ptr << std::endl;
    }

    void report() {
        std::lock_guard<std::mutex> lock(mtx); // Ensure thread safety
        if (allocations.empty()) {
            std::cout << "\n>>> No memory leaks detected. <<<" << std::endl;
        } else {
            std::cout << "\n>>> Memory leaks detected <<<" << std::endl;
            for (const auto& pair : allocations) {
                std::cout << "\t- Leak at address " << pair.first << " of size " << pair.second.size
                          << " bytes. Allocated at " << pair.second.file << ":line(" << pair.second.line << ")" << std::endl;
            }
        }
    }

    // TEST OVERLOADS
    template<typename R>
    R test(std::function<R()> func) {
        if constexpr (std::is_same_v<R, void>) {
            func();
            report();
        } else {
            R data = func();
            report();
            return data;
        }
    }

    template<typename R, typename A1>
    R test(std::function<R(A1)> func, A1 arg1) {
        if constexpr (std::is_same_v<R, void>) {
            func(arg1);
            report();
        } else {
            R data = func(arg1);
            report();
            return data;
        }
    }

    template<typename R, typename A1, typename A2>
    R test(std::function<R(A1, A2)> func, A1 arg1, A2 arg2) {
        if constexpr (std::is_same_v<R, void>) {
            func(arg1, arg2);
            report();
        } else {
            R data = func(arg1, arg2);
            report();
            return data;
        }
    }

    template<typename R, typename A1, typename A2, typename A3>
    R test(std::function<R(A1, A2, A3)> func, A1 arg1, A2 arg2, A3 arg3) {
        if constexpr (std::is_same_v<R, void>) {
            func(arg1, arg2, arg3);
            report();
        } else {
            R data = func(arg1, arg2, arg3);
            report();
            return data;
        }
    }

    template<typename R, typename A1, typename A2, typename A3, typename A4>
    R test(std::function<R(A1, A2, A3, A4)> func, A1 arg1, A2 arg2, A3 arg3, A4 arg4) {
        if constexpr (std::is_same_v<R, void>) {
            func(arg1, arg2, arg3, arg4);
            report();
        } else {
            R data = func(arg1, arg2, arg3, arg4);
            report();
            return data;
        }
    }

    template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
    R test(std::function<R(A1, A2, A3, A4, A5)> func, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5) {
        if constexpr (std::is_same_v<R, void>) {
            func(arg1, arg2, arg3, arg4, arg5);
            report();
        } else {
            R data = func(arg1, arg2, arg3, arg4, arg5);
            report();
            return data;
        }
    }

};

#define TRACK_NEW_DELETE \
    void* operator new(size_t size) { return MT_New(size, __FILE__, __LINE__); } \
    void* operator new[](size_t size) { return MT_NewArray(size, __FILE__, __LINE__); } \
    void operator delete(void* ptr) noexcept { MT_Delete(ptr); } \
    void operator delete[](void* ptr) noexcept { MT_DeleteArray(ptr); } 

/* Prevent name mangling..
    Name Mangling is a complier process of setting a unique name to a function to ensure
    proper linkage. This looks at the signature of the function from param, return type
    and name. It supports features like namespaces and overloading.

    Why prevent?
    When linking with other languages like C, that language will not recognize the function
    as C does not use mangler. 
*/

extern "C" {
    void* MT_New(size_t size, const char* file, int line);
    void* MT_NewArray(size_t size, const char* file, int line);
    void MT_Delete(void* ptr);
    void MT_DeleteArray(void* ptr);
}

#endif