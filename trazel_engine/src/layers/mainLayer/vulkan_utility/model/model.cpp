#include "pch.h"
#include "model.h"
#include "../../vulkan_setup/frameBuffer/frameBuffer.h"

namespace vkUtil
{
	model::model(vk::PhysicalDevice& PhysicalDevice, vk::Device& Device, std::vector<vertex>& vertices) : physicalDevice(PhysicalDevice), device(Device)
	{
		createVertexBuffer(vertices);
	}
	model::~model()
	{
		vkDestroyBuffer(device, vertexBuffer, nullptr);
		vkFreeMemory(device, vertexBufferMemory, nullptr);
	}


	void model::createVertexBuffer(const std::vector<vertex>& vertices)
	{
		vertexCount = uint32_t(vertices.size());
		assert(vertexCount >= 3 && "vertex count must be at least 3");
		vk::DeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

		vkInit::createBuffer
		(
			bufferSize,
			VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			device,
			physicalDevice,
			vertexBuffer,
			vertexBufferMemory
		);
		void* data;
		vkMapMemory(device, vertexBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, vertices.data(), size_t(bufferSize));
		vkUnmapMemory(device, vertexBufferMemory);
	}


	void model::bind(vk::CommandBuffer commandBuffer)
	{
		VkBuffer buffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = {0};
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	}
	void model::draw(vk::CommandBuffer commandBuffer)
	{
		vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
	}

	std::vector<VkVertexInputBindingDescription> model::vertex::getBindingDescriptions()
	{
		std::vector<VkVertexInputBindingDescription> bindingDescription(1);
		bindingDescription[0].binding = 0;
		bindingDescription[0].stride = sizeof(vertex);
		bindingDescription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		
		return bindingDescription;
	}
	std::vector<VkVertexInputAttributeDescription> model::vertex::getAttributeDescriptions()
	{
		std::vector<VkVertexInputAttributeDescription> attributeDescription(2);
		attributeDescription[0].binding = 0;
		attributeDescription[0].location = 0;
		attributeDescription[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescription[0].offset = offsetof(vertex, position);

		attributeDescription[1].binding = 0;
		attributeDescription[1].location = 1;
		attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescription[1].offset = offsetof(vertex, color);
		return attributeDescription;
	}
}