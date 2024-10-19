# SimpleVector

`SimpleVector` is a custom implementation of a dynamic array (vector) in C++. It provides similar functionality to `std::vector`, including dynamic resizing, insertion, and deletion of elements. The class uses a smart pointer called `ArrayPtr` to manage memory allocation and deallocation.

## Features

- Dynamic resizing of the vector.
- Support for copying and moving elements.
- Safe element access with bounds checking.
- Ability to reserve memory in advance.
- Insertion and deletion of elements at arbitrary positions.

## Dependencies

- C++11 or later

## Classes

### SimpleVector

- **Template Type**: `Type`
  
#### Constructor

- `SimpleVector() noexcept`
  - Default constructor that creates an empty vector.

- `SimpleVector(size_t size)`
  - Creates a vector with the specified size, initialized with default values.

- `SimpleVector(size_t size, const Type& value)`
  - Creates a vector with the specified size, initialized with the given value.

- `SimpleVector(std::initializer_list<Type> init)`
  - Creates a vector from an initializer list.

- `SimpleVector(const SimpleVector& other)`
  - Copy constructor.

- `SimpleVector(SimpleVector&& other) noexcept`
  - Move constructor.

#### Assignment Operators

- `SimpleVector& operator=(const SimpleVector& other)`
  - Copy assignment operator.

- `SimpleVector& operator=(SimpleVector&& other) noexcept`
  - Move assignment operator.

#### Public Methods

- `size_t GetSize() const noexcept`
  - Returns the current number of elements in the vector.

- `size_t GetCapacity() const noexcept`
  - Returns the current allocated capacity of the vector.

- `bool IsEmpty() const noexcept`
  - Checks if the vector is empty.

- `Type& operator[](size_t index) noexcept`
  - Access element by index (non-const).

- `const Type& operator[](size_t index) const noexcept`
  - Access element by index (const).

- `Type& At(size_t index)`
  - Access element by index with bounds checking.

- `const Type& At(size_t index) const`
  - Access element by index with bounds checking (const).

- `void Clear() noexcept`
  - Clears the vector, making it empty.

- `void Resize(size_t new_size)`
  - Resizes the vector to the new size.

- `void Reserve(size_t new_capacity)`
  - Reserves memory for at least the new capacity.

- `void PopBack() noexcept`
  - Removes the last element from the vector.

- `Iterator Erase(ConstIterator pos)`
  - Removes the element at the specified position.

- `void PushBack(const Type& value)`
  - Adds a new element to the end of the vector.

- `void PushBack(Type&& value)`
  - Moves a new element to the end of the vector.

- `Iterator Insert(ConstIterator pos, const Type& value)`
  - Inserts an element at the specified position.

- `Iterator Insert(ConstIterator pos, Type&& value)`
  - Moves an element to the specified position.

### ArrayPtr

- **Template Type**: `Type`
  
#### Constructor

- `ArrayPtr()`
  - Default constructor initializes to a null pointer.

- `ArrayPtr(size_t size)`
  - Initializes the pointer to a newly allocated array of the specified size.

- `ArrayPtr(Type* raw_ptr) noexcept`
  - Initializes the pointer from a raw pointer.

#### Move Semantics

- `ArrayPtr(ArrayPtr&& other) noexcept`
  - Move constructor.

- `ArrayPtr& operator=(ArrayPtr&& other) noexcept`
  - Move assignment operator.

#### Public Methods

- `Type& operator[](size_t index) noexcept`
  - Access element by index (non-const).

- `const Type& operator[](size_t index) const noexcept`
  - Access element by index (const).

- `Type* Release() noexcept`
  - Releases ownership of the raw pointer.

- `void swap(ArrayPtr& other) noexcept`
  - Swaps the contents of this object with another `ArrayPtr`.

## Example Usage

```cpp
#include "simple_vector.h"

int main() {
    SimpleVector<int> vec;

    vec.PushBack(10);
    vec.PushBack(20);
    vec.PushBack(30);

    vec.Resize(5);

    for (size_t i = 0; i < vec.GetSize(); ++i) {
        std::cout << vec[i] << " "; // Output: 10 20 30 0 0
    }

    return 0;
}
