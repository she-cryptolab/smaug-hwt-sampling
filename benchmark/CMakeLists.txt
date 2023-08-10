set(bench_src
    ${CMAKE_CURRENT_SOURCE_DIR}/cpucycles.c
    ${CMAKE_CURRENT_SOURCE_DIR}/speed_print.c
    )

add_executable(bench-smaug1-hwt bench-hwt.c ${bench_src})
target_link_libraries(bench-smaug1-hwt smaug1-hwt)
target_include_directories(bench-smaug1-hwt PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(bench-smaug3-hwt bench-hwt.c ${bench_src})
target_link_libraries(bench-smaug3-hwt smaug3-hwt)
target_include_directories(bench-smaug3-hwt PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(bench-smaug5-hwt bench-hwt.c ${bench_src})
target_link_libraries(bench-smaug5-hwt smaug5-hwt)
target_include_directories(bench-smaug5-hwt PUBLIC ${PROJECT_SOURCE_DIR}/include)


# #-- estimate buffer requirement --#
# add_executable(smaug1-buf estimate_buf_size.c)
# target_link_libraries(smaug1-buf smaug1-hwt)
# target_include_directories(smaug1-buf PUBLIC ${PROJECT_SOURCE_DIR}/include)

# add_executable(smaug3-buf estimate_buf_size.c)
# target_link_libraries(smaug3-buf smaug3-hwt)
# target_include_directories(smaug3-buf PUBLIC ${PROJECT_SOURCE_DIR}/include)

# add_executable(smaug5-buf estimate_buf_size.c)
# target_link_libraries(smaug5-buf smaug5-hwt)
# target_include_directories(smaug5-buf PUBLIC ${PROJECT_SOURCE_DIR}/include)