file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SOURCES "*.cpp")

set(SHADER_DIR "../Shader/Source")
file(GLOB HLSL_HEADERS "${SHADER_DIR}/ShaderLibrary/*.hlsl")
file(GLOB HLSL_SOURCES "${SHADER_DIR}/*.hlsl")
set(ALL_FILES ${HEADERS} ${SOURCES})

add_library(Engine STATIC ${ALL_FILES} ${HLSL_SOURCES} ${HLSL_HEADERS})
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add shader in folder
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ALL_FILES})
source_group("Shaders" FILES ${HLSL_SOURCES})
source_group("Shaders/ShaderLibrary" FILES ${HLSL_HEADERS})
# HLSL only need to be included in the project, but not compiled
set_source_files_properties(${HLSL_SOURCES} ${HLSL_HEADERS} PROPERTIES HEADER_FILE_ONLY TRUE)

# Link libraries
find_package(Vulkan REQUIRED)
target_link_libraries(Engine PUBLIC ${Vulkan_LIBRARIES})  
target_include_directories(Engine PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(Engine PUBLIC glfw)
target_link_libraries(Engine PUBLIC glm)
target_link_libraries(Engine PUBLIC spdlog)
target_link_libraries(Engine PUBLIC tinygltf)
target_link_libraries(Engine PUBLIC VulkanMemoryAllocator)
target_link_libraries(Engine PUBLIC imgui)
target_link_libraries(Engine PUBLIC Jolt)
target_link_libraries(Engine PUBLIC EnTT)
target_link_libraries(Engine PUBLIC OptickCore)
target_compile_options(Engine PRIVATE /wd4996)

# Compile shaders
add_custom_target(CompileShader ALL)
set(SPV_TARGET_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Shader/SPV")
set(DXC_DIR "$ENV{VULKAN_SDK}/bin/dxc.exe")

foreach(SHADER_FILE ${HLSL_SOURCES})
    get_filename_component(FILE_NAME ${SHADER_FILE} NAME_WE)
    message("Compile shader: ${SHADER_FILE}")
    add_custom_command(TARGET CompileShader POST_BUILD
        COMMAND ${DXC_DIR} -E MainVS -spirv -Zi -Qembed_debug -O0 -T vs_6_0 -Fo ${SPV_TARGET_DIR}/${FILE_NAME}.vert.spv ${SHADER_FILE}
        COMMAND ${DXC_DIR} -E MainPS -spirv -Zi -Qembed_debug -O0 -T ps_6_0 -Fo ${SPV_TARGET_DIR}/${FILE_NAME}.frag.spv ${SHADER_FILE}
        )
endforeach()

add_dependencies(Engine CompileShader)

