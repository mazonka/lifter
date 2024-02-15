# Simple Big Number unsigned arithmetic

This is a C++ (standard) header for unsigned big number arithmetic. 
It is implemented as a "lifter" from an underlying arithmetic type. 
For example, `lifter<uint64_t>` represents a 128-bit unsigned type.

To create a larger type, use the template multiple times. 
To use, simply include `lifter.h` in your file;
no need to build or compile separately.
For an example, refer to the .cpp file.

The class is defined as:
```
template <class U>
struct lifter
{
    U lo, hi;
};
```
