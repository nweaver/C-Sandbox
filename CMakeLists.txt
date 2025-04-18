cmake_minimum_required(VERSION 3.10)
project(hello)

set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g \
-Wall -Wformat -Wformat=2 -Wconversion -Wimplicit-fallthrough \
-Werror=format-security \
-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 \
-D_GLIBCXX_ASSERTIONS \
-fstack-clash-protection -fstack-protector-strong \
-Wl,-z,nodlopen -Wl,-z,noexecstack \
-Wl,-z,relro -Wl,-z,now \
-Wl,--as-needed -Wl,--no-copy-dt-needed-entries \
--coverage")
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra --coverage -D_GLIBCXX_ASSERTIONS")



# This is geting gunit so you don't have to...
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(hello main.c
        confuzzle.c
        confuzzle.h)
	
enable_testing()


add_executable(testbinary confuzzle.c confuzzle_test.cpp stringexamples.cpp stringexamples_test.cpp
 stringexamples_c.c stringexamples_c_test.cpp llist.cpp llist_test.cpp graph_test.cpp
 c_list.c c_list_test.cpp fileio_test.cpp tuple_map_test.cpp workqueue_test.cpp badcompile_test.cpp slice_test.cpp) 
target_link_libraries(
  testbinary
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(testbinary)