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
