cmake_minimum_required(VERSION 3.16)

project(HelloDemo LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT ProgramDatabase)

# ==============================
# 第三方库路径（支持缓存修改）
# ==============================
set(PCL_ROOT "D:/OpenSource/PCL1151" CACHE PATH "PCL installation root")
set(OPENCV_ROOT "D:/OpenSource/OPENCV490" CACHE PATH "OpenCV installation root")

# 构建子路径
set(OPENCV_BUILD_DIR "${OPENCV_ROOT}/opencv/build")

# ==============================
# 源码与可执行文件
# ==============================
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

# ==============================
# 头文件路径（仅作用于当前 target）
# ==============================
target_include_directories(${PROJECT_NAME} PRIVATE
    ${SRC_DIR}
    ${SRC_DIR}/sum
    ${SRC_DIR}/diff

    # PCL 及其依赖
    ${PCL_ROOT}/include/pcl-1.15
    ${PCL_ROOT}/3rdParty/Boost/include/boost-1_87
    ${PCL_ROOT}/3rdParty/Eigen3/include/eigen3
    ${PCL_ROOT}/3rdParty/FLANN/include
    ${PCL_ROOT}/3rdParty/Qhull/include
    ${PCL_ROOT}/3rdParty/VTK/include/vtk-9.4

    # OpenCV
    ${OPENCV_BUILD_DIR}/include
)

# ==============================
# 编译选项
# ==============================
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:/Zi>
    $<$<CONFIG:Release>:/O2>
    /utf-8
    /arch:AVX2  # 注意：确保目标 CPU 支持 AVX2
    /DUNICODE
    /D_UNICODE
)

#仅在 Debug 模式定义调试宏
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
    MY_TSET_SHOW_PRINTF
    )
endif()

# 链接选项
target_link_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:/DEBUG>
    $<$<CONFIG:Release>:/OPT:REF /OPT:ICF>
)

# ==============================
# 库后缀设置（Debug vs Release）
# ==============================
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OPENCV_LIB_SUFFIX "d")
    set(PCL_LIB_SUFFIX "d")
    set(VTK_LIB_SUFFIX "-9.4-gd")
else()
    set(OPENCV_LIB_SUFFIX "")
    set(PCL_LIB_SUFFIX "")
    set(VTK_LIB_SUFFIX "-9.4")
endif()

# ==============================
# OpenCV 库（完整路径）
# ==============================
set(OPENCV_LIB_PATH "${OPENCV_BUILD_DIR}/x64/vc16/lib/opencv_world490${OPENCV_LIB_SUFFIX}.lib")

# ==============================
# PCL 库（完整路径）
# ==============================
set(PCL_LIB_BASE "${PCL_ROOT}/lib")
set(PCL_LIBS
    "${PCL_LIB_BASE}/pcl_common${PCL_LIB_SUFFIX}.lib"
    "${PCL_LIB_BASE}/pcl_io${PCL_LIB_SUFFIX}.lib"
    "${PCL_LIB_BASE}/pcl_kdtree${PCL_LIB_SUFFIX}.lib"
    "${PCL_LIB_BASE}/pcl_features${PCL_LIB_SUFFIX}.lib"
    "${PCL_LIB_BASE}/pcl_filters${PCL_LIB_SUFFIX}.lib"
    "${PCL_LIB_BASE}/pcl_visualization${PCL_LIB_SUFFIX}.lib"
    # 可按需添加：pcl_segmentation, pcl_registration 等
)

# ==============================
# VTK 库（特殊处理 vtksys）
# ==============================
set(VTK_LIB_BASE "${PCL_ROOT}/3rdParty/VTK/lib")
set(VTK_MODULES
    CommonCore
    CommonDataModel
    CommonExecutionModel
    CommonMath
    CommonMisc
    CommonSystem
    CommonTransforms
    FiltersCore
    FiltersGeneral
    FiltersGeometry
    RenderingCore
    RenderingOpenGL2
    RenderingUI
    InteractionStyle
    InteractionWidgets
    vtksys  # 明确写出，便于识别
)

set(VTK_LIBS "")
foreach(module IN LISTS VTK_MODULES)
    if(module STREQUAL "vtksys")
        list(APPEND VTK_LIBS "${VTK_LIB_BASE}/${module}${VTK_LIB_SUFFIX}.lib")
    else()
        list(APPEND VTK_LIBS "${VTK_LIB_BASE}/vtk${module}${VTK_LIB_SUFFIX}.lib")
    endif()
endforeach()

# ==============================
# 链接所有库
# ==============================
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${OPENCV_LIB_PATH}
    ${PCL_LIBS}
    ${VTK_LIBS}
)

# ==============================
# 输出目录
# ==============================
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_CURRENT_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)