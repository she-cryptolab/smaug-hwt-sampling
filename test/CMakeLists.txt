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

# #-- degree distribution --#
# add_executable(smaug1-dist distribution.c)
# target_link_libraries(smaug1-dist smaug1-hwt)
# target_include_directories(smaug1-dist PUBLIC ${PROJECT_SOURCE_DIR}/include)

# add_executable(smaug3-dist distribution.c)
# target_link_libraries(smaug3-dist smaug3-hwt)
# target_include_directories(smaug3-dist PUBLIC ${PROJECT_SOURCE_DIR}/include)

# add_executable(smaug5-dist distribution.c)
# target_link_libraries(smaug5-dist smaug5-hwt)
# target_include_directories(smaug5-dist PUBLIC ${PROJECT_SOURCE_DIR}/include)


#-- degree distribution --#
add_executable(smaug-coef-encoding coef_encoding_32.c)
target_link_libraries(smaug-coef-encoding smaug1-hwt)
target_include_directories(smaug-coef-encoding PUBLIC ${PROJECT_SOURCE_DIR}/include)