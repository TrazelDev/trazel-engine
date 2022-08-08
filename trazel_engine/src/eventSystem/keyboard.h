#pragma once
#include "pch.h"

namespace tze { static class keyboard {
	friend class windowsWindow;
	class event
	{
	public:
		enum class Type
		{
			press,
			release
		};
	private:
		Type type;
		unsigned char code;
	public:
		event(Type type, unsigned char code)
			:
			type(type),
			code(code)
		{}

		bool ssPress() const
		{
			return type == Type::press;
		}


		bool ssRelease() const
		{
			return type == Type::press;
		}


		unsigned char getCode() const
		{
			return code;
		}
	};
public:
	keyboard(){};
	keyboard(const keyboard&) = delete;
	
	static bool keyIsPressed(unsigned char keycode);
	static std::optional<event> readKey();
	static bool keyIsEmpty();
	static void flushKey();

	// char event stuff:
	static std::optional<char> readChar();
	static bool charIsEmpty();
	static void flushChar();
	static void flush();

	// autorepeat control:
	static void enableAutorepeat();
	static void disableAutorepeat();
	static bool autorepeatIsEnabled();


private:
	static const unsigned int bufferSize = 16u;
	static bool autorepeatEnabled;
	static std::bitset<256u> keystates;
	static std::queue<event> keybuffer;
	static std::queue<char> charbuffer;

	static void onKeyPressed(unsigned char keycode);
	static void onKeyReleased(unsigned char keycode);
	static void onChar(char character);
	static void clearState();

	template<typename T>
	static void trimBuffer(std::queue<T>& buffer);
};
}

