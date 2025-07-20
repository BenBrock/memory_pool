function(add_example example_name)
  add_executable(${example_name} ${example_name}.cpp)
  target_link_libraries(${example_name} memory_pool fmt)
endfunction()

add_example(simple_example)
