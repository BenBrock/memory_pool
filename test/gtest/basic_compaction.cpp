#include <gtest/gtest.h>

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <mp/mp.hpp>

void test_compaction(std::size_t allocation_size,
                     std::vector<void*> allocations) {
  if (allocations.size() >= 2) {
    for (void* ptr : allocations) {
      mp::free(ptr);
    }

    std::size_t n_allocations = allocations.size() / 2;
    allocation_size *= 2;

    allocations.clear();

    for (std::size_t i = 0; i < n_allocations; i++) {
      void* ptr = mp::malloc(allocation_size);
      ASSERT_NE(ptr, nullptr);
      allocations.push_back(ptr);
    }

    test_compaction(allocation_size, allocations);
  }
}

TEST(MP, BasicCompaction) {
  std::size_t nbytes = 2 * 1024 * 1024;

  std::size_t allocation_size = 1024;

  char* base_ptr = (char*) malloc(nbytes);

  mp::init(base_ptr, nbytes);

  std::size_t n_allocations = nbytes / allocation_size;

  std::vector<void*> allocations;

  // Fill up the whole memory pool with allocations.
  for (std::size_t i = 0; i < n_allocations; i++) {
    void* ptr = mp::malloc(allocation_size);
    ASSERT_NE(ptr, nullptr);
    allocations.push_back(ptr);
  }

  test_compaction(allocation_size, allocations);

  free(base_ptr);
}
