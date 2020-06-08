project(pam_zcwkey)

cmake_minimum_required(VERSION 3.0)

add_definitions(-g)

#set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(include)
add_subdirectory(sm3)

#pam_zcwkey库
aux_source_directory(${PROJECT_SOURCE_DIR}  SRC_LIB  )
aux_source_directory(sm3 SRC_LIB  )
list(REMOVE_ITEM SRC_LIB ${PROJECT_SOURCE_DIR}/test.c ${PROJECT_SOURCE_DIR}/ukey_tool.c)
message(SRC_LIB= ${SRC_LIB})

add_library(pam_skf SHARED ${SRC_LIB}   )
target_link_libraries(pam_skf  m  pam pam_misc   pthread
                        -Wl,--whole-archive  ${PROJECT_SOURCE_DIR}/libes_3000gm.a  -Wl,--no-whole-archive)

#初始化工具
ADD_EXECUTABLE(ukey_tool ukey_tool.c)
target_link_libraries(ukey_tool  pam_skf )


#测试程序
ADD_EXECUTABLE(test test.c  )
target_link_libraries(test pam_skf )


# 将库文件，可执行文件，头文件安装到指定目录
install(TARGETS pam_skf
        LIBRARY DESTINATION /lib64/security
        )
