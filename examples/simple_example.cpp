#include <mp/mp.hpp>

#include <vector>

int main(int argc, char** argv) {

  std::size_t nbytes = 2 * 1024 * 1024;
  std::size_t n_elem = (nbytes / sizeof(int)) / 3;

  char* base_ptr = (char*) malloc(nbytes);

  // Initialize memory pool.
  mp::init(base_ptr, nbytes);

  {
    std::vector<int, mp::allocator<int>> v;

    for (std::size_t i = 0; i < n_elem; i++) {
      v.push_back(i);
    }
  }

  // Ensure objects allocated using the memory pool are
  // destroyed before freeing the memory pool itself.
  mp::free(base_ptr);

  return 0;
}
