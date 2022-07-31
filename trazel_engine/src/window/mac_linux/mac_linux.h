// #pragma once
// #include "../window.h"
// #include "glfw3.h"
// 
// 
// namespace tze {
// 	class linuxWin : public window {
// 	public:
// 		linuxWin(const windowProp& props);
// 		virtual ~linuxWin();
// 		
// 		inline unsigned int getWidth() const override {
// 			return winWidth;
// 		}
// 		inline unsigned int getHight() const override {
// 			return winheight;
// 		}
// 
// 		// the attributes of the window
// 		/*inline void setEventCallback(const eventCallbackFn& callback) override
// 		{
// 			m_data.EventCallback = callback;
// 		}*/
// 		void setVsync(bool enabled) override;
// 		bool checkVsynced() const override;
// 	private:
// 		virtual void init(const windowProp& props);
// 
// 		GLFWwindow* window;
// 
// 		unsigned int winWidth;
// 		unsigned int winheight;
// 	};
// }
