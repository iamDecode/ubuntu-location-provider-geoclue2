cmake_minimum_required (VERSION 2.6)
project (Tutorial)
find_package(PkgConfig)
pkg_check_modules(DBUS dbus-1 REQUIRED)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -fPIC -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -fno-strict-aliasing -pedantic -Wextra -fPIC -g")

# chrono filesystem context iostreams locale log mpi signals thread timer wave program_options graph_parallel 
find_package(Boost COMPONENTS system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_subdirectory(data)
link_directories(${Boost_LIBRARY_DIRS})

include_directories(
  ${DBUS_INCLUDE_DIRS}
  ${DBUS_CPP_INCLUDE_DIRS}
)
add_executable(ubuntu-location-provider-geoclue2 main.cpp geoclueProvider.cpp ulsProvider.cpp)
link_directories(
    ${DBUS_LIBRARY_DIRS}
    ${DBUS_CPP_LIBRARY_DIRS}
    ${Boost_LIBRARY_DIRS}
)
target_link_libraries(
    ubuntu-location-provider-geoclue2 
    dbus-cpp
    
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    ${DBUS_LIBRARIES}
)
install(TARGETS ubuntu-location-provider-geoclue2 RUNTIME DESTINATION bin)
