add_executable(Tools Main.cpp)
target_compile_options(Tools PRIVATE /wd4996)
target_link_libraries(Tools PRIVATE Engine)

# Copy DLL
add_custom_command(TARGET Tools POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Jolt> $<TARGET_FILE_DIR:Tools>)

add_custom_command(TARGET Tools POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:OptickCore> $<TARGET_FILE_DIR:Tools>)