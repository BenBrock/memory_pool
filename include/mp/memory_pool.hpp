#pragma once

#include <cassert>
#include <map>
#include <unordered_map>

namespace mp {

template <typename T>
using pointer_type_t = T*;

template <typename T, typename U>
inline T* pointer_cast(U* ptr) {
  return (T*) ptr;
}

struct chunk_t {
  size_t ptr = 0;
  size_t size = 0;
};

inline std::unordered_map<std::size_t, chunk_t> allocd_chunks_;
inline std::map<size_t, chunk_t> free_chunks_;

inline pointer_type_t<char> base_ptr_ = nullptr;

inline std::size_t heap_ptr_ = 0;
inline std::size_t nbytes_ = 0;
inline std::size_t smallest_allocation_ = 1024;

// Initialize memory pool.
inline void init(pointer_type_t<void> base_ptr, std::size_t nbytes,
                 std::size_t smallest_allocation = 1024) {
  base_ptr_ = pointer_cast<char>(base_ptr);
  nbytes_ = nbytes;
  smallest_allocation_ = smallest_allocation;
}

inline pointer_type_t<void> malloc(std::size_t nbytes) {
  // Round up nbytes to nearest multiple of smallest_allocation.
  std::size_t size =
      smallest_allocation_ *
      ((nbytes + smallest_allocation_ - 1) / smallest_allocation_);

  if (size == 0) {
    return nullptr;
  }

  // Search for a free chunk of at least `size` bytes.
  auto free_chunk_iter = free_chunks_.end();
  for (auto iter = free_chunks_.begin(); iter != free_chunks_.end(); ++iter) {
    auto [chunk_ptr, free_chunk] = *iter;
    if (free_chunk.size >= size) {
      free_chunk_iter = iter;
      break;
    }
  }

  // New chunk that will be obtained, either from free list or heap.
  chunk_t new_chunk;

  if (free_chunk_iter != free_chunks_.end()) {
    // We found a free chunk.
    auto [chunk_ptr, chunk] = *free_chunk_iter;

    // Delete the chunk from the free list.
    free_chunks_.erase(free_chunk_iter);

    // If the chunk is big enough to support two allocations, split the chunk.
    if (chunk.size - size >= smallest_allocation_) {
      chunk_t leftover_chunk{chunk.ptr + size, chunk.size - size};
      free_chunks_[leftover_chunk.ptr] = leftover_chunk;
      chunk.size = size;
    }

    new_chunk = chunk;
  } else {
    // No free chunk was available.  Create a new chunk by advancing the heap
    // pointer.

    // If out of memory, return nullptr.
    if (heap_ptr_ + size > nbytes_) {
      return nullptr;
    }

    new_chunk = chunk_t{heap_ptr_, size};
    heap_ptr_ += size;
  }

  allocd_chunks_[new_chunk.ptr] = new_chunk;

  return pointer_cast<void>(base_ptr_ + new_chunk.ptr);
}

inline void compact_free_list_() {
  for (auto iter = free_chunks_.begin(); iter != free_chunks_.end();) {
    auto next_iter = iter;
    ++next_iter;

    if (next_iter != free_chunks_.end()) {
      auto&& [chunk_ptr, chunk] = *iter;
      auto&& [next_chunk_ptr, next_chunk] = *next_iter;

      if (next_chunk.ptr == chunk.ptr + chunk.size) {
        chunk.size += next_chunk.size;
        free_chunks_.erase(next_iter);
        continue;
      }
    }
    ++iter;
  }
}

inline void free(pointer_type_t<void> ptr) {
  if (ptr == nullptr) {
    return;
  }

  std::size_t chunk_ptr = pointer_cast<char>(ptr) - base_ptr_;

  auto iter = allocd_chunks_.find(chunk_ptr);

  assert(iter != allocd_chunks_.end());

  auto [free_chunk_ptr, free_chunk] = *iter;

  allocd_chunks_.erase(iter);

  free_chunks_[free_chunk.ptr] = free_chunk;

  compact_free_list_();
}

} // namespace mp
