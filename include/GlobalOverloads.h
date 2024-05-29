/* Header file Notes..
    This file globally overloads the "new" and "delete" operators to pass through
    the MemoryTracker singleton for tracking memory leaks.
*/

#ifndef GLOBAL_OVERLOAD
#define GLOBAL_OVERLOAD

#include "MemoryTracker.h"

/// @brief Tracks the current memory attempting to delete to prevent duplicate calls on the same memory.
void* CURRENT_DELETE_PTR = nullptr;

/* Important Note on "new" operator.. 
    The "new" operator, by default, has one arg "size" but 
    we added two additional args to support getting the file name 
    and line where the instance occurs.

    This re-defintion happens at end of this file as a preprocessor directive.

    Why? 
    Because it is helpful when tracking memory leaks to know exactly
    what file and at what line the leak was created.
*/

void* MT_New(size_t size, const char* file, int line) {
    void* ptr = malloc(size);
    if (ptr) {
        MemoryTracker::getInstance().addAllocation(ptr, size, file, line);
    }
    return ptr;
}

void* MT_NewArray(size_t size, const char* file, int line) {
    return MT_New(size, file, line);
}

void MT_Delete(void* ptr) {
    if (ptr && CURRENT_DELETE_PTR == nullptr) {
        CURRENT_DELETE_PTR = ptr;
        MemoryTracker::getInstance().removeAllocation(ptr);
        CURRENT_DELETE_PTR = nullptr;
        free(ptr);
    }
    else{
        /* Temp Bug fix..
            ignores any duplicate calls when using the delete function.

            The issue was that both delete(void* ptr) and delete(void* ptr, size_t) 
            were being called causing deadlock.
        */
    }
}

void MT_DeleteArray(void* ptr) {
    MT_Delete(ptr);
}



// Global overloads
/* Why Inline Functions..
    Since we are calling unique function for the overload, within each of the following
    operators we don't want to transfer control flow but instead just have the function
    run inside the overload functions below.

    Each function being called fit the conditions for inline function so the complier
    should NOT ignore the request.
*/
inline void* operator new(size_t size, const char* file, int line) {
    return MT_New(size, file, line);
}

inline void* operator new[](size_t size, const char* file, int line) {
    return MT_NewArray(size, file, line);
}

inline void operator delete(void* ptr) noexcept {
    MT_Delete(ptr);
}

inline void operator delete(void* ptr, const char*, int) noexcept {
    MT_Delete(ptr);
}

inline void operator delete(void* ptr, size_t) noexcept {
    MT_Delete(ptr);
}

inline void operator delete[](void* ptr) noexcept {
    MT_DeleteArray(ptr);
}

inline void operator delete[](void* ptr, const char*, int) noexcept {
    MT_DeleteArray(ptr);
}

inline void operator delete[](void* ptr, size_t) noexcept {
    MT_DeleteArray(ptr);
}

// Include filename and line as args to the "new" operator
#define new new(__FILE__, __LINE__)

#endif