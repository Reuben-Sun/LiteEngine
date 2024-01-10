#include "RHI/Public/RHIDevice.h"

namespace ToolEngine
{
    QueueFamilyIndices QueueFamilyIndices::getQueueFamilyIndices(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        QueueFamilyIndices indices;

        uint32_t queue_family_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, nullptr);
        std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queue_family_count, queue_families.data());

        // find one and only one queue which suport VK_QUEUE_GRAPHICS_BIT
        int family_index = 0;
        for (const auto& queue_family : queue_families)
        {
            if (queue_family.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            {
                indices.graphics_family = family_index;
            }
            VkBool32 present_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, family_index, surface, &present_support);
            if (present_support)
            {
                indices.present_family = family_index;
            }
            if (indices.isComplete())
            {
                break;
            }
            family_index++;
        }

        return indices;
    }

    SwapChainSupportDetails SwapChainSupportDetails::getSwapChainSupportDetails(VkPhysicalDevice physical_device, VkSurfaceKHR surface)
    {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, surface, &details.capabilities);
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &formatCount, nullptr);
        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physical_device, surface, &formatCount, details.formats.data());
        }
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentModeCount, nullptr);
        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physical_device, surface, &presentModeCount, details.presentModes.data());
        }
        return details;
    }

    RHIDevice::RHIDevice(RHIInstance& instance) : m_instance(instance)
    {
        createPhysicalDevice();
        createLogicalDevice();
    }
    RHIDevice::~RHIDevice()
    {
        if (m_logical_device != VK_NULL_HANDLE)
        {
            vkDestroyDevice(m_logical_device, nullptr);
        }
    }

    void RHIDevice::createPhysicalDevice()
    {
        uint32_t device_count = 0;
        vkEnumeratePhysicalDevices(m_instance.getHandle(), &device_count, nullptr);
        if (device_count == 0)
        {
            LOG_ERROR("failed to find GPUs with Vulkan support!");
        }
        std::vector<VkPhysicalDevice> devices(device_count);
        vkEnumeratePhysicalDevices(m_instance.getHandle(), &device_count, devices.data());

        for (const auto& device : devices)
        {
            if (checkDeviceSupport(device))
            {
                m_physical_device = device;
                break;
            }
        }

        if (m_physical_device == VK_NULL_HANDLE)
        {
            LOG_ERROR("failed to find a suitable GPU!");
        }

        LOG_INFO("Create Physical Device!");
    }
    void RHIDevice::createLogicalDevice()
    {
        QueueFamilyIndices indices = QueueFamilyIndices::getQueueFamilyIndices(m_physical_device, m_instance.getSurfaceHandle());

        std::vector<VkDeviceQueueCreateInfo> queue_create_infos;
        std::set<uint32_t> unique_queue_families =
        {
            indices.graphics_family.value(),
            indices.present_family.value()
        };
        float queuePriority = 1.0f;
        for (uint32_t queue_family_index : unique_queue_families)
        {
            VkDeviceQueueCreateInfo queue_create_info{};
            queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queue_create_info.queueFamilyIndex = queue_family_index;
            queue_create_info.queueCount = 1;
            queue_create_info.pQueuePriorities = &queuePriority;
            queue_create_infos.push_back(queue_create_info);
        }

        VkPhysicalDeviceFeatures device_features{};
        device_features.samplerAnisotropy = VK_TRUE;

        VkDeviceCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        create_info.queueCreateInfoCount = static_cast<uint32_t>(queue_create_infos.size());
        create_info.pQueueCreateInfos = queue_create_infos.data();
        create_info.pEnabledFeatures = &device_features;
        create_info.enabledExtensionCount = static_cast<uint32_t>(m_device_extensions.size());
        create_info.ppEnabledExtensionNames = m_device_extensions.data();

        if (m_instance.isEnableValidationLayers())
        {
            create_info.enabledLayerCount = static_cast<uint32_t>(m_instance.m_validation_layers.size());
            create_info.ppEnabledLayerNames = m_instance.m_validation_layers.data();
        }
        else
        {
            create_info.enabledLayerCount = 0;
        }

        if (vkCreateDevice(m_physical_device, &create_info, nullptr, &m_logical_device) != VK_SUCCESS)
        {
            LOG_ERROR("failed to create logical device!");
        }

        LOG_INFO("Create Logical Device!");

        if (checkPresentSupport(m_physical_device, indices.graphics_family.value()))
        {
            vkGetDeviceQueue(m_logical_device, indices.graphics_family.value(), 0, &m_graphics_queue);
        }
        if (checkPresentSupport(m_physical_device, indices.present_family.value()))
        {
            vkGetDeviceQueue(m_logical_device, indices.present_family.value(), 0, &m_present_queue);
        }
        LOG_INFO("Create Queue!");

        m_command_pool = std::make_unique<RHICommandPool>(m_logical_device, indices.graphics_family.value());
        LOG_INFO("Create Command Pool!");
    }

    VkFormat RHIDevice::getDepthFormatDetail()
    {
        VkFormat depth_format = VK_FORMAT_UNDEFINED;
        VkFormat formats[] = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
        for (VkFormat format : formats)
        {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(m_physical_device, format, &props);

            if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
            {
                depth_format = format;
                break;
            }
        }
        return depth_format;
    }


    bool RHIDevice::checkPresentSupport(VkPhysicalDevice device, uint32_t queue_family_index)
    {
        VkBool32 present_supported{ VK_FALSE };

        if (m_instance.getSurfaceHandle() != VK_NULL_HANDLE)
        {
            vkGetPhysicalDeviceSurfaceSupportKHR(m_physical_device, queue_family_index, m_instance.getSurfaceHandle(), &present_supported);
        }

        return present_supported;
    }
    bool RHIDevice::checkDeviceSupport(VkPhysicalDevice device)
    {
        QueueFamilyIndices indices = QueueFamilyIndices::getQueueFamilyIndices(device, m_instance.getSurfaceHandle());
        bool extension_supported = checkDeviceExtensionSupport(device);
        bool swap_chain_adequate = false;
        if (extension_supported)
        {
            SwapChainSupportDetails swap_chain_support = SwapChainSupportDetails::getSwapChainSupportDetails(device, m_instance.getSurfaceHandle());
            swap_chain_adequate = !swap_chain_support.formats.empty() && !swap_chain_support.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supported_features;
        vkGetPhysicalDeviceFeatures(device, &supported_features);

        return indices.isComplete() && extension_supported && swap_chain_adequate && supported_features.samplerAnisotropy;
    }
    bool RHIDevice::checkDeviceExtensionSupport(VkPhysicalDevice device)
    {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());
        std::set<std::string> requiredExtensions(m_device_extensions.begin(), m_device_extensions.end());
        for (const auto& extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }
        return requiredExtensions.empty();
    }
}