#pragma once
#include "pch.h"
#include "../vulkan_utility/model/model.h"

namespace tze
{
	struct trasform2dComponent
	{
		glm::vec2 translation{};
		glm::vec2 scale{ 1.0f, 1.0f };
		float rotation;

		glm::mat2 mat2() 
		{ 
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix{ {c, s}, {-s, c} };

			glm::mat2 scaleMat{ {scale.x, 0.0f}, {0.0f, scale.y} };
			return rotMatrix * scaleMat;
		}
	};

	class gameObject
	{
	public:
		using id_t = unsigned int;

		static gameObject createGameObj()
		{
			static id_t currId = 0;
			return gameObject{currId++};
		}

		gameObject(const gameObject&) = delete;
		gameObject &operator=(const gameObject&) = delete;
		gameObject(gameObject&&) = default;
		gameObject &operator=(gameObject&&) = default;

		id_t getId() { return id; }

		std::shared_ptr<vkUtil::model> model;
		glm::vec3 color{};
		trasform2dComponent transform2d;
	private:
		gameObject(id_t objId) : id(objId) {};

		id_t id;
	};
}

