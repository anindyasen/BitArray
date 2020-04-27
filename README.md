# BitArray
A bit array is an array data structure that compactly stores bits. The C programming language does not provide support for array of bits.
But, C programming language provides all the necessary operations to allow the programmer (= you) to implement an "array of bits" data structure.

This program is a example of BitArray, where you can read/write/display the bitArray. Here, each word is of size 26 bits. and max 8(can be changed by changin the value of SIZE) such words can be stored. Therefore, we need a array of 26\*8=208 bits, ie,a uint32 array of size 7(32\*7=224). This read/write/display interfaces  parse this 7bytes of uint32 array and get the proper result.  
