cmake_minimum_required(VERSION 3.4.1)

add_definitions(-DHAVE_PTHREADS=1)
add_definitions(-DHAVE_OFF64_T=1) #might cause problem on older systems!
add_definitions(-DOS_PATH_SEPARATOR=47) #ascii code for / (slash)
add_definitions(-DHAVE_SYS_SOCKET_H=1)
add_definitions(-DANDROID_SMP=1) #always needed? (Required by SharedBuffer.cpp/atomic.h)

set(CMAKE_C_FLAGS_DEBUG "-ggdb")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb")
set(CMAKE_C_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_library(zipalign SHARED
            ZipAlign.cpp
            zipalign/ZipAlign.cpp
            zipalign/ZipEntry.cpp
            zipalign/ZipFile.cpp
            android/native/src/utils/SharedBuffer.cpp
            android/native/src/utils/ZipUtils.cpp
            android/native/src/utils/VectorImpl.cpp)

target_include_directories(zipalign BEFORE PUBLIC
                    zipalign/include
                    android/base/include
                    android/core/include
                    android/native/include)

target_link_libraries(zipalign log)

target_link_libraries(zipalign z)
