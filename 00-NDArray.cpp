#include "aare/NDArray.hpp"
#include <iostream>
int main(){

    // N dimensional array, templated on type and number of dimensions

    // Create a 3x3 array of ints
    aare::NDArray<int, 2> arr({3, 3});
    std::cout << "Initial value arr:\n" << arr << std::endl;

    // Set value
    arr(0, 0) = 1;
    arr(0, 1) = 2;
    arr(1, 1) = 7;

    std::cout << "arr:\n" << arr << std::endl;

    // // Initialize a 3x3 array of ints with value 5
    aare::NDArray<int, 2> arr2({3, 3}, 5);
    std::cout << "arr2:\n" <<arr2 << std::endl;

    // Add two arrays
    aare::NDArray<int, 2> arr3 = arr + arr2;
    std::cout << "arr + arr2:\n" << arr3 << std::endl;

    //Subtract two arrays
    aare::NDArray<int, 2> arr4 = arr - arr2;
    std::cout << "arr - arr2:\n" << arr4 << std::endl;

}