add_executable(Client Main.cpp)
target_link_libraries(Client PRIVATE Engine)

# Copy DLL
add_custom_command(TARGET Client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Jolt> $<TARGET_FILE_DIR:Client>)