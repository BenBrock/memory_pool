#pragma once

#include <iterator>
#include <mp/memory_pool.hpp>

namespace mp {

template <typename T>
class allocator {
public:
  using pointer = mp::pointer_type_t<T>;
  using const_pointer = mp::pointer_type_t<const T>;
  using void_pointer = mp::pointer_type_t<void>;
  using const_void_pointer = mp::pointer_type_t<const void>;

  using value_type = T;

  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;

  using reference = std::iter_reference_t<pointer>;
  using const_reference = std::iter_reference_t<const_pointer>;

  allocator() noexcept = default;
  allocator(const allocator&) noexcept = default;
  allocator& operator=(const allocator&) noexcept = default;
  ~allocator() noexcept = default;

  using is_always_equal = std::true_type;

  pointer allocate(std::size_t size) {
    pointer ptr = mp::pointer_cast<T>(mp::malloc(sizeof(T) * size));

    if (ptr == nullptr) {
      throw std::bad_alloc{};
    } else {
      return ptr;
    }
  }

  void deallocate(pointer ptr, std::size_t n) {
    mp::free(ptr);
  }

  bool operator==(const allocator&) const = default;
  bool operator!=(const allocator&) const = default;

  template <typename U>
  struct rebind {
    using other = allocator<U>;
  };
};

} // namespace mp
