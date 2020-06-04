project(pam_zcwkey)

cmake_minimum_required(VERSION 3.0)

add_definitions(-Wall)
add_definitions(-g)
set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(include)
add_subdirectory(sm3)

#pam_zcwkey库
aux_source_directory(${PROJECT_SOURCE_DIR}  SRC_LIB  )
aux_source_directory(sm3 SRC_LIB  )
add_library(pam_zcwkey SHARED ${SRC_LIB}   )
#target_link_libraries(pam_zcwkey ${PROJECT_SOURCE_DIR}/libes_3000gm.a )
target_link_libraries(pam_zcwkey  -Wl,--whole-archive  ${PROJECT_SOURCE_DIR}/libes_3000gm.a  -Wl,--no-whole-archive)

#测试程序

ADD_EXECUTABLE(test test.c  )
target_link_libraries(test   pam_zcwkey pam pam_misc  m pthread )
#

# 将库文件，可执行文件，头文件安装到指定目录
install(TARGETS pam_zcwkey
        LIBRARY DESTINATION /lib64/security
        )
