# 查找 test 目录下的所有 .cpp 测试源文件
file(GLOB HWY_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*.cpp")

######################################################################################################################
# DEPENDENCIES
######################################################################################################################

######################################################################################################################
# MAKE TARGETS
######################################################################################################################

##########################################
# "make check-tests"
##########################################
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)

##########################################
# 自动生成测试可执行文件
##########################################
foreach (hwy_test_source ${HWY_TEST_SOURCES})
    # 获取文件名和测试名（不带后缀）
    get_filename_component(hwy_test_filename ${hwy_test_source} NAME)
    string(REPLACE ".cpp" "" hwy_test_name ${hwy_test_filename})

    # 创建可执行目标
    add_executable(${hwy_test_name} ${hwy_test_source})

    # 添加依赖到 build-tests 和 check-tests
    add_dependencies(build-tests ${hwy_test_name})
    add_dependencies(check-tests ${hwy_test_name})

    # 链接共享库
    target_link_libraries(${hwy_test_name} hwy_shared)

    # 设置可执行文件输出路径
    set_target_properties(${hwy_test_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    # 将测试可执行文件添加到 CTest 测试框架中
    add_test(NAME ${hwy_test_name} COMMAND ${CMAKE_BINARY_DIR}/bin/${hwy_test_name})
endforeach()
