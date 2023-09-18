#-------------------------- degree distribution --------------------------#
add_executable(smaug1-dist distribution.c)
target_link_libraries(smaug1-dist smaug1-hwt)
target_include_directories(smaug1-dist PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(smaug3-dist distribution.c)
target_link_libraries(smaug3-dist smaug3-hwt)
target_include_directories(smaug3-dist PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(smaug5-dist distribution.c)
target_link_libraries(smaug5-dist smaug5-hwt)
target_include_directories(smaug5-dist PUBLIC ${PROJECT_SOURCE_DIR}/include)


# #---------------------- estimate seed size requirement ----------------------#
# add_executable(smaug-seed estimate_required_seed.c)
# target_link_libraries(smaug-seed smaug1-hwt)
# target_include_directories(smaug-seed PUBLIC ${PROJECT_SOURCE_DIR}/include)