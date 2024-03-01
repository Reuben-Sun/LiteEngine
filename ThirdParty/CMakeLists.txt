find_package(Vulkan REQUIRED)
if (MSVC)
    add_compile_options(/wd4996)
endif()
# glfw
if (NOT TARGET glfw)
  set(GLFW_USE_HYBRID_HPG OFF)
  set(GLFW_VULKAN_STATIC OFF)
  set(GLFW_BUILD_DOCS OFF)
  set(GLFW_BUILD_EXAMPLES OFF)
  set(GLFW_BUILD_TESTS OFF)
  set(GLFW_INSTALL OFF)
  set(BUILD_SHARED_LIBS OFF)
  add_subdirectory(glfw)
  set_property(TARGET glfw PROPERTY FOLDER "ThirdParty")
endif()

# imgui
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
file(GLOB IMGUI_SOURCES CONFIGURE_DEPENDS ${IMGUI_DIR}/*.cpp)
file(GLOB IMGUI_IMPL_SOURCES CONFIGURE_DEPENDS 
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp 
  ${IMGUI_DIR}/backends/imgui_impl_glfw.h 
  ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
  ${IMGUI_DIR}/backends/imgui_impl_vulkan.h)
add_library(imgui STATIC ${IMGUI_SOURCES} ${IMGUI_IMPL_SOURCES})
target_include_directories(imgui PUBLIC ${IMGUI_DIR})
target_include_directories(imgui PUBLIC ${Vulkan_INCLUDE_DIR})
target_link_libraries(imgui PUBLIC glfw ${Vulkan_LIBRARIES})
set_property(TARGET imgui PROPERTY FOLDER "ThirdParty")

# glm
if(NOT TARGET glm)
  add_subdirectory(glm)
  set_property(TARGET glm PROPERTY FOLDER "ThirdParty")
endif()

# tinygltf
if(NOT TARGET tinygltf)
  add_subdirectory(tinygltf)
  set_property(TARGET tinygltf PROPERTY FOLDER "ThirdParty")
endif()

# spdlog
if(NOT TARGET spdlog)
  add_subdirectory(spdlog)
  set_property(TARGET spdlog PROPERTY FOLDER "ThirdParty")
endif()

# vma
if(NOT TARGET VulkanMemoryAllocator)
  add_subdirectory(vma)
endif()

# JoltPhysics
if(NOT TARGET JoltPhysics)
  set(BUILD_SHARED_LIBS ON)
  add_subdirectory(JoltPhysics/Build)
  set_property(TARGET Jolt PROPERTY FOLDER "ThirdParty")
endif()

# entt
if(NOT TARGET EnTT)
  add_subdirectory(entt)
  set_property(TARGET EnTT PROPERTY FOLDER "ThirdParty")
endif()