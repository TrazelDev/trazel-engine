#include "pch.h"
#include "keyboard.h"

namespace tze
{
	bool keyboard::autorepeatEnabled = false;
	std::bitset<256u> keyboard::keystates;
	std::queue<keyboard::event> keyboard::keybuffer;
	std::queue<char> keyboard::charbuffer;
	
	bool keyboard::keyIsPressed(unsigned char keycode)
	{
		return keyboard::keystates[keycode];
	}
	std::optional<keyboard::event> keyboard::readKey()
	{
		if (keyboard::keybuffer.size() > 0u)
		{
			keyboard::event Event = keyboard::keybuffer.front();
			keyboard::keybuffer.pop();
			return Event;
		}
		return {};
	}
	bool keyboard::keyIsEmpty()
	{
		return keyboard::keybuffer.empty();
	}
	void keyboard::flushKey()
	{
		keyboard::keybuffer = std::queue<keyboard::event>();
	}

	std::optional<char> keyboard::readChar()
	{
		if (keyboard::charbuffer.size() > 0u)
		{
			unsigned char charcode = keyboard::charbuffer.front();
			keyboard::charbuffer.pop();
			return charcode;
		}
		return {};
	};
	bool keyboard::charIsEmpty()
	{
		return keyboard::charbuffer.empty();
	}
	void keyboard::flushChar()
	{
		keyboard::charbuffer = std::queue<char>();
	}
	void keyboard::flush()
	{
		flushKey();
		flushChar();
	}

	void keyboard::enableAutorepeat()
	{
		keyboard::autorepeatEnabled = true;
	}
	void keyboard::disableAutorepeat()
	{
		keyboard::autorepeatEnabled = false;
	}
	bool keyboard::autorepeatIsEnabled()
	{
		return keyboard::autorepeatEnabled;
	}

	void keyboard::onKeyPressed(unsigned char keycode)
	{
		keystates[keycode] = true;
		keybuffer.push(event(event::Type::press, keycode));
		trimBuffer(keybuffer);
	}
	void keyboard::onKeyReleased(unsigned char keycode)
	{
		keystates[keycode] = false;
		keybuffer.push(event(event::Type::release, keycode));
		trimBuffer(keybuffer);
	}
	void keyboard::onChar(char character)
	{
		charbuffer.push(character);
		trimBuffer(charbuffer);
	}
	void keyboard::clearState()
	{
		keystates.reset();
	}

	template<typename T>
	void keyboard::trimBuffer(std::queue<T>& buffer)
	{
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}
}