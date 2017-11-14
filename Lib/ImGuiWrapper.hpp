#pragma once

#include "External/imgui/imgui.h"

namespace cmgl
{

class Window;

class ImGuiWrapper
{
	public:
		ImGuiWrapper();

		bool init(Window& window);
		void newFrame();
		void render();
		void shutdown();

	private:
		static const char* getClipboardText(void* user_data);
		static void setClipboardText(void* user_data, const char* text);

	private:
		bool createDeviceObjects();
		void invalidateDeviceObjects();

		bool createFontsTexture();

	private:
		Window* mWindow;
		float mTime;
		bool mMouseJustPressed[3];
		float mMouseWheel;
		unsigned int mFontTexture;
		int mShaderHandle, mVertHandle, mFragHandle;
		int mAttribLocationTex, mAttribLocationProjMtx;
		int mAttribLocationPosition, mAttribLocationUV, mAttribLocationColor;
		unsigned int mVboHandle, mVaoHandle, mElementsHandle;
};

} // namespace cmgl