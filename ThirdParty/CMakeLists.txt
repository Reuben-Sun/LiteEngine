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
