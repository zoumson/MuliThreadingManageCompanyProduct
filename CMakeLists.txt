cmake_minimum_required(VERSION 3.16.3)

#set the project name
project (arguments)

#Set the compiler
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Release)
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#Set ccache (for speeding up)
find_program(CCACHE_FOUND ccache) 
if(CCACHE_FOUND) 
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache) 
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache) 
endif(CCACHE_FOUND) 
set (CMAKE_CXX_FLAGS "-DPCL_ONLY_CORE_POINT_TYPES=ON -DNO_EXPLICIT_INSTANTIATIONS") 

############### Dependencies #######################
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
############### My Files #########################

#Bring the headers
include_directories(${PROJECT_SOURCE_DIR}/include)

#Bring the sources
#file(GLOB SOURCES_DEMO ${PROJECT_SOURCE_DIR}/src/*.c)

#Build the executable
add_executable(client ${PROJECT_SOURCE_DIR}/src/client.c)
add_executable(server ${PROJECT_SOURCE_DIR}/src/server.c)

target_link_libraries(server PRIVATE Threads::Threads)




