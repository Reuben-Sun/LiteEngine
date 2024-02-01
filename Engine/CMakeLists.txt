file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SOURCES "*.cpp")

set(SHADER_DIR "../Shader/Source")
file(GLOB VERT_SHADERS "${SHADER_DIR}/*.vert")
file(GLOB FRAG_SHADERS "${SHADER_DIR}/*.frag")
set(ALL_FILES ${HEADERS} ${SOURCES})

add_library(Engine STATIC ${ALL_FILES} ${VERT_SHADERS} ${FRAG_SHADERS})
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_FILES})
source_group("Shaders" FILES ${VERT_SHADERS} ${FRAG_SHADERS})

find_package(Vulkan REQUIRED)
target_link_libraries(Engine PUBLIC ${Vulkan_LIBRARIES})  
target_include_directories(Engine PUBLIC ${Vulkan_INCLUDE_DIR})

target_link_libraries(Engine PUBLIC glfw)
target_link_libraries(Engine PUBLIC glm)
target_link_libraries(Engine PUBLIC spdlog)
target_link_libraries(Engine PUBLIC tinygltf)
target_link_libraries(Engine PUBLIC VulkanMemoryAllocator)
target_link_libraries(Engine PUBLIC imgui)
target_compile_options(Engine PRIVATE /wd4996)

# Compile shaders
set(SPV_TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Shader/SPV")

foreach(SHADER_FILE ${VERT_SHADERS})
    get_filename_component(FILE_NAME ${SHADER_FILE} NAME)
    string(REPLACE "." "_" OUTPUT_STRING ${FILE_NAME})
    message("Compile vert shader: ${FILE_NAME} => ${OUTPUT_STRING}.spv")
    add_custom_command(TARGET Engine POST_BUILD
        COMMAND glslangValidator -V -S vert ${SHADER_FILE} -o ${SPV_TARGET_DIR}/${OUTPUT_STRING}.spv
        )
endforeach()

foreach(SHADER_FILE ${FRAG_SHADERS})
    get_filename_component(FILE_NAME ${SHADER_FILE} NAME)
    string(REPLACE "." "_" OUTPUT_STRING ${FILE_NAME})
    message("Compile frag shader: ${FILE_NAME} => ${OUTPUT_STRING}.spv")
    add_custom_command(TARGET Engine POST_BUILD
        COMMAND glslangValidator -V -S frag ${SHADER_FILE} -o ${SPV_TARGET_DIR}/${OUTPUT_STRING}.spv
        )
endforeach()