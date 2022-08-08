#pragma once
#include "../layer.h"
#include "pch.h"

class imguiLayer : public layer
{
public:
	imguiLayer(GLFWwindow* Window, const char* Title, uint32_t Width, uint32_t Height)
	{
		window = Window;
		title = Title;
		width = width;
		height = Height;
	}
	~imguiLayer()
	{
		// Cleanup
		err = vkDeviceWaitIdle(g_Device);
		check_vk_result(err);
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		CleanupVulkanWindow();
		CleanupVulkan();

		glfwDestroyWindow(window);
		glfwTerminate();
	}

	virtual void onAttach() override;
	virtual void onDetach() override {};
	virtual void onUpdate() override;
	virtual void onEvent() override {};
private:
	static void check_vk_result(VkResult err);
	// static VKAPI_ATTR VkBool32 VKAPI_CALL debug_report(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData);
	static void SetupVulkan(const char** extensions, uint32_t extensions_count);
    static void SetupVulkanWindow(ImGui_ImplVulkanH_Window* wd, VkSurfaceKHR surface, int width, int height);
    static void CleanupVulkan();
    static void CleanupVulkanWindow();
    static void FrameRender(ImGui_ImplVulkanH_Window* wd, ImDrawData* draw_data);
	static void FramePresent(ImGui_ImplVulkanH_Window* wd);
	static void glfw_error_callback(int error, const char* description);

	static VkAllocationCallbacks*   g_Allocator;
	static VkInstance               g_Instance;
	static VkPhysicalDevice         g_PhysicalDevice;
	static VkDevice                 g_Device;
	static uint32_t                 g_QueueFamily;
	static VkQueue                  g_Queue;
	static VkDebugReportCallbackEXT g_DebugReport;
	static VkPipelineCache          g_PipelineCache;
	static VkDescriptorPool         g_DescriptorPool;
	 
	static ImGui_ImplVulkanH_Window g_MainWindowData;
	static int                      g_MinImageCount;
	static bool                     g_SwapChainRebuild;

	ImGui_ImplVulkanH_Window* wd;
	VkResult err;
	ImGuiIO* ioHolder;

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};
