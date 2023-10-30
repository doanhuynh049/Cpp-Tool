file(REMOVE_RECURSE
  "build/Librarycode.o"
  "build/TestRunner.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/my_test.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
