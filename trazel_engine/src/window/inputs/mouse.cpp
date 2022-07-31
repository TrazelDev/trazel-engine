#include "pch.h"
#include "mouse.h"

namespace tze
{
	int mouse::x;
	int mouse::y;
	bool mouse::leftIsPressed = false;
	bool mouse::rightIsPressed = false;
	bool mouse::midlleIsPressed = false;
	bool mouse::InWindow = false;
	int mouse::wheelDeltaCarry = 0;
	bool mouse::rawEnabled = false;
	std::queue<mouse::event> mouse::buffer;
	std::queue<mouse::RawDelta> mouse::rawDeltaBuffer;


	std::pair<int, int> mouse::getPos()
	{
		return { x,y };
	}
	std::optional<mouse::RawDelta> mouse::readRawDelta()
	{
		if (rawDeltaBuffer.empty())
		{
			return std::nullopt;
		}
		const RawDelta d = rawDeltaBuffer.front();
		rawDeltaBuffer.pop();
		return d;
	}
	int mouse::getPosX()
	{
		return x;
	}
	int mouse::getPosY()
	{
		return y;
	}

	bool mouse::isInWindow()
	{
		return InWindow;
	}

	bool mouse::isLeftPressed()
	{
		return leftIsPressed;
	}
	bool mouse::isRightPressed()
	{
		return rightIsPressed;
	}
	bool mouse::isMidlletPressed()
	{
		return midlleIsPressed;
	}
	std::optional<mouse::event> mouse::read()
	{
		if (buffer.size() > 0u)
		{
			mouse::event e = buffer.front();
			buffer.pop();
			return e;
		}
		return {};
	}
	void mouse::flush()
	{
		buffer = std::queue<event>();
	}

	void mouse::enableRaw()
	{
		rawEnabled = true;
	}
	void mouse::disableRaw()
	{
		rawEnabled = false;
	}
	bool mouse::checkRawEnabled()
	{
		return rawEnabled;
	}

	void mouse::onMouseMove(int newx, int newy)
	{
		x = newx;
		y = newy;

		buffer.push(mouse::event(mouse::event::Type::Move));
		trimBuffer();
	}
	void mouse::onMouseLeave()
	{
		InWindow = false;
		buffer.push(mouse::event(mouse::event::Type::InWindow));
		trimBuffer();
	}
	void mouse::onMouseEnter()
	{
		InWindow = true;
		buffer.push(mouse::event(mouse::event::Type::OutWindow));
		trimBuffer();
	}
	void mouse::onRawDelta(int dx, int dy)
	{
		rawDeltaBuffer.push({ dx,dy });
		trimBuffer();
	}

	void mouse::onLeftPressed(int x, int y)
	{
		leftIsPressed = true;
		buffer.push(mouse::event(mouse::event::Type::LPress));
		trimBuffer();
	}
	void mouse::onLeftReleased(int x, int y)
	{
		leftIsPressed = false;
		buffer.push(mouse::event(mouse::event::Type::LRelease));
		trimBuffer();
	}

	void mouse::onRightPressed(int x, int y)
	{
		rightIsPressed = true;
		buffer.push(mouse::event(mouse::event::Type::RPress));
		trimBuffer();
	}
	void mouse::onRightReleased(int x, int y)
	{
		rightIsPressed = false;
		buffer.push(mouse::event(mouse::event::Type::RRelease));
		trimBuffer();
	}

	void mouse::onMiddlePressed(int x, int y)
	{
		midlleIsPressed = true;
		buffer.push(mouse::event(mouse::event::Type::MPress));
		trimBuffer();
	}
	void mouse::onMiddleRelease(int x, int y)
	{
		midlleIsPressed = false;
		buffer.push(mouse::event(mouse::event::Type::MRelease));
		trimBuffer();
	}

	void mouse::onWheelUp(int x, int y)
	{
		buffer.push(mouse::event(mouse::event::Type::WheelUp));
		trimBuffer();
	}
	void mouse::onWheelDown(int x, int y)
	{
		buffer.push(mouse::event(mouse::event::Type::WheelDown));
		trimBuffer();
	}

	void mouse::trimBuffer()
	{
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}
	void mouse::trimRawInputBuffer()
	{
		while (rawDeltaBuffer.size() > bufferSize)
		{
			rawDeltaBuffer.pop();
		}
	}
	void mouse::onWheelDelta(int x, int y, int delta)
	{
		wheelDeltaCarry += delta;

		// generate events for every 120
		while (wheelDeltaCarry >= WHEEL_DELTA)
		{
			wheelDeltaCarry -= WHEEL_DELTA;
			onWheelUp(x, y);
		}
		while (wheelDeltaCarry <= -WHEEL_DELTA)
		{
			wheelDeltaCarry += WHEEL_DELTA;
			onWheelDown(x, y);
		}
	}
}