
cmake_minimum_required(VERSION 3.10)

set (CMAKE_CXX_STANDARD 11)

project(tempo_dsp VERSION 0.1 LANGUAGES CXX)

file(GLOB cpp_src_files "cpp_src/*.cpp")
add_library(tempo_dsp SHARED ${cpp_src_files})
add_library(tempo_dsp_static STATIC ${cpp_src_files})

install(TARGETS tempo_dsp DESTINATION lib)
install(TARGETS tempo_dsp_static DESTINATION lib)

file(GLOB headers "cpp_src/*.hpp")
foreach(file ${headers})
    install(FILES ${file} DESTINATION include/tempo_dsp)
endforeach()

include_directories("cpp_src")

file(GLOB test_files "cpp_src/test/test_*.cpp")
foreach(file ${test_files})
    string(REGEX REPLACE "(^.*/|\\.[^.]*$)" "" file_without_ext ${file})
    add_executable(${file_without_ext} ${file})
    target_link_libraries(${file_without_ext} tempo_dsp)
    add_test(${file_without_ext} ${file_without_ext})
    set_tests_properties(${file_without_ext}
        PROPERTIES
        PASS_REGULAR_EXPRESSION "Test passed")
    set_tests_properties(${file_without_ext}
        PROPERTIES
        FAIL_REGULAR_EXPRESSION "(Exception|Test failed)")
    set_tests_properties(${file_without_ext}
        PROPERTIES
        TIMEOUT 120)
endforeach()
