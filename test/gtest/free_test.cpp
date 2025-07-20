#include <gtest/gtest.h>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <mp/mp.hpp>

TEST(MP, BasicMallocFree) {
  std::size_t nbytes = 2 * 1024 * 1024;

  std::size_t allocation_size = 1024;

  char* base_ptr = (char*) malloc(nbytes);

  mp::init(base_ptr, nbytes);

  std::size_t n_allocations = nbytes / allocation_size;

  std::size_t n_iterations = 10;

  for (std::size_t i = 0; i < n_iterations; i++) {
    std::vector<void*> allocations;

    // Fill up the whole memory pool with allocations.
    for (std::size_t i = 0; i < n_allocations; i++) {
      void* ptr = mp::malloc(allocation_size);
      ASSERT_NE(ptr, nullptr);
      allocations.push_back(ptr);
    }

    // Free every allocation.
    for (void* ptr : allocations) {
      mp::free(ptr);
    }
  }

  free(base_ptr);
}
