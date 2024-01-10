add_executable(Client Main.cpp)
target_link_libraries(Client PRIVATE Engine)

# Copy Shader
add_custom_command(TARGET Client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Shader/SPV $<TARGET_FILE_DIR:Client>/Shaders)