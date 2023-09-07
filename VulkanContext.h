#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include <Windows.h>

class VulkanContext
{
public:
    void Init(void *sharedImageHandle);
    void drawFrame();

private:
    void checkExternMemorySupport();
    void createInstance();
    void setupDebugMessenger();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createImageViews();
    void createCommandPool();
    void createCommandBuffer();
    void createSyncObjects();
    void recordCommandBuffer();
    VkShaderModule createShaderModule(const std::vector<char> &code);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    
    VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger);
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    HANDLE sharedImageHandle;

    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;

    VkQueue graphicsQueue;

    VkImage swapChainImage;
    VkFormat swapChainImageFormat = VK_FORMAT_R8G8B8A8_UNORM;
    VkExtent2D swapChainExtent;
    VkImageView swapChainImageView;
    VkFramebuffer swapChainFramebuffer;

    VkRenderPass renderPass;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;

    VkFence inFlightFence;
};