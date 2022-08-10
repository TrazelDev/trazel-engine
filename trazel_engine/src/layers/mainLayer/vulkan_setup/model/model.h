#pragma once
#include "pch.h"

namespace vkUtil
{
	class model
	{
	public:
		struct vertex
		{
			glm::vec2 position;
			glm::vec3 color;

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		};

		model(vk::PhysicalDevice& PhysicalDevice, vk::Device& Device, const std::vector<vertex>& vertices);
		~model();

		model(const model&) = delete;
		model& operator=(const model&) = delete;
		
		void bind(vk::CommandBuffer commandBuffer);
		void draw(vk::CommandBuffer commandBuffer);
	
	private:
		void createVertexBuffer(const std::vector<vertex>& vertices);

		vk::Device& device;
		vk::PhysicalDevice& physicalDevice;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		uint32_t vertexCount;
	};
}

