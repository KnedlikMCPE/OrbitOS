#include "search.h"

uint16_t binary_search(uint16_t* arr, uint64_t size, uint16_t val) {
    uint64_t left = 0;
    uint64_t right = size - 1;
    while (left <= right) {
        uint64_t mid = (left + right) / 2;
        if (arr[mid] == val) {
            return mid;
        } else if (arr[mid] < val) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return 0;
}