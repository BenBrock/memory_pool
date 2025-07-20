# Memory Pool

`memory_pool` is a basic C++ memory pool implementaiton.  This is intended to be used to handle CPU-side allocation of GPU memory and other resources where your own memory pool is required for performance or other reasons.

## Features

- Header-only implementation
- Basic free list compaction
- Configurable minimum allocation size
- Support for allocations with fancy pointer types

## Building

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

To run tests:
```bash
cd build
ctest
```

## Usage

### Basic Usage

```cpp
#include <mp/mp.hpp>

// Allocate base memory region
std::size_t pool_size = 2 * 1024 * 1024; // 2MB
char* base_ptr = (char*) malloc(pool_size);

// Initialize the memory pool
mp::init(base_ptr, pool_size);

// Allocate memory from pool
void* ptr = mp::malloc(1024);

// Use the memory...

// Free allocated memory
mp::free(ptr);

// Clean up base memory when done
free(base_ptr);
```

### With STL Containers

```cpp
#include <mp/mp.hpp>
#include <vector>

// Initialize pool as shown above...

// Use with STL containers
std::vector<int, mp::allocator<int>> vec;
vec.push_back(42);

// Vector will automatically free its memory
```

## Implementation Details

The memory pool uses a combination of strategies to manage memory efficiently:

- Maintains separate lists for allocated and free chunks
- Coalesces adjacent free blocks to reduce fragmentation
- Rounds allocations to configurable minimum size
- Uses first-fit strategy for allocation
- Splits large free blocks when possible

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.
