PROJECT(PP)

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")

FILE(GLOB_RECURSE HEADER_FILES ./*.h)
FILE(GLOB_RECURSE CPP_FILES ./*.cpp)
FILE(GLOB_RECURSE CC_FILES ./*.cc)


SET(
    ALL_FILES
    ${HEADER_FILES}
    ${CPP_FILES}
    ${CC_FILES}
)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Thirdparty}/glfw/include
    ${Thirdparty}/glew/include
    ${Thirdparty}/glm
    ${Thirdparty}
    ${Thirdparty}/spdlog/include
)

link_directories(
    ${Thirdparty}/glfw/lib-vc2022
    ${Thirdparty}/glew/lib
    ${Thirdparty}/spdlog/lib
)

# IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
#     LINK_DIRECTORIES(
#         ${Thirdparty}/ffmpeg/lib/
#     )
# ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
#     LINK_DIRECTORIES(
#         ${Thirdparty}/ffmpeg_x86/lib/
#     )
# ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)

SOURCE_GROUP_BY_DIR(ALL_FILES)

add_executable(${PROJECT_NAME}
    WIN32
    ${ALL_FILES}
)
#if(${QT_VERSION_MAJOR} GREATER_EQUAL 5)
    
#endif()

# add_definitions("-DSTB_IMAGE_IMPLEMENTATION")


IF(WIN32)
    SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MP")
 ENDIF(WIN32)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    opengl32
    glew32s
    glfw3
    debug spdlogd
    optimized spdlog
)


SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${ProjectDIR}/bin)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
)

