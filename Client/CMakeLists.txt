add_executable(Client Engine.h Engine.cpp Render.h Render.cpp Main.cpp)
target_compile_options(Client PRIVATE /wd4996)
target_link_libraries(Client PRIVATE Engine)

# Copy DLL
add_custom_command(TARGET Client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Jolt> $<TARGET_FILE_DIR:Client>)

add_custom_command(TARGET Client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:OptickCore> $<TARGET_FILE_DIR:Client)