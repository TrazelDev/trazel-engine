#pragma once
class layer
{
public:
	layer() = default;
	virtual void onAttach() {};
	virtual void onDetach() {};
	virtual void onUpdate() {};
	virtual void onEvent() {};

	uint32_t width, height;
	const char* title;
	GLFWwindow* window;

};

