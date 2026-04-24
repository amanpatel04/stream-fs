include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(stream_fs_test
    test_streamfs.cpp
)

target_link_libraries(stream_fs_test PRIVATE stream_fs_lib GTest::gtest_main)

gtest_discover_tests(stream_fs_test)
