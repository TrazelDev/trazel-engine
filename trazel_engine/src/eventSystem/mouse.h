#pragma once
#include "pch.h"

namespace tze { class mouse
{
	friend class windowsWindow;
public:
	struct RawDelta
	{
		int x, y;
	};
	class event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			MPress,
			MRelease,
			WheelUp,
			WheelDown,
			Move,
			InWindow,
			OutWindow,
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		bool midlleIsPressed;
		int x;
		int y;
	public:
		event(Type type)
			:
			type(type),
			leftIsPressed(mouse::leftIsPressed),
			rightIsPressed(mouse::rightIsPressed),
			x(mouse::x),
			y(mouse::y)
		{}
		Type GetType() 
		{
			return type;
		}
		std::pair<int, int> GetPos() 
		{
			return{ x,y };
		}
		int GetPosX() 
		{
			return x;
		}
		int GetPosY() 
		{
			return y;
		}
		bool LeftIsPressed() 
		{
			return leftIsPressed;
		}
		bool RightIsPressed() 
		{
			return rightIsPressed;
		}
		bool midlletIsPressed()
		{
		return midlleIsPressed;
		}
	};
public:
	mouse() = default;
	mouse(mouse&) = delete;
	mouse& operator=(mouse&) = delete;
	static std::pair<int, int> getPos();
	static std::optional<RawDelta> readRawDelta();
	static int getPosX();
	static int getPosY();
	static bool isInWindow();
	static bool isLeftPressed();
	static bool isRightPressed();
	static bool isMidlletPressed();
	static std::optional<mouse::event> read();
	static bool isEmpty() 
	{
		return buffer.empty();
	}
	static void flush();
	static void enableRaw();
	static void disableRaw();
	static bool checkRawEnabled();

private:
	static void onMouseMove(int x, int y);
	static void onMouseLeave();
	static void onMouseEnter();
	static void onRawDelta(int dx, int dy);
 
	static void onLeftPressed(int x, int y);
	static void onLeftReleased(int x, int y);
 
	static void onRightPressed(int x, int y);
	static void onRightReleased(int x, int y);

	static void onMiddlePressed(int x, int y);
	static void onMiddleRelease(int x, int y);

	static void onWheelUp(int x, int y);
	static void onWheelDown(int x, int y);

	static void trimBuffer();
	static void trimRawInputBuffer();
	static void onWheelDelta(int x, int y, int delta);

private:
	static const unsigned int bufferSize = 16u;
	static int x;
	static int y;
	static bool leftIsPressed;
	static bool rightIsPressed;
	static bool midlleIsPressed;
	static bool InWindow;
	static int wheelDeltaCarry;
	static bool rawEnabled;
	static std::queue<event> buffer;
	static std::queue<RawDelta> rawDeltaBuffer;
};
}

