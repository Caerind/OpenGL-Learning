#pragma once

#include <GL/glew.h>

#include "Lib/Camera.hpp"
#include "Lib/Window.hpp"
#include "Lib/ImGuiWrapper.hpp"

#include "Engine.hpp"

class Application
{
	public:
		Application();

		bool init();

		bool run();

		void clear();

	private:
		void update(float dt);
		void render();

	private:
		cmgl::Window mWindow;
		cmgl::ImGuiWrapper mImGui;
		cmgl::Camera mCamera;
		GLuint mVAO;

		ImVec4 mClearColor;

		cmgl::Texture mTexture;
		cmgl::Shader mShader;
		cmgl::Mesh mMesh;

		ModelAsset mAsset;
		ModelInstance mInstance;
		ModelInstance mInstance2;

		glm::vec3 mPosition;
		glm::vec3 mDirection;
		glm::vec3 mRight;

		ImVec4 mAmbient;
		ImVec4 mLightColor;
		float mShininess;
		float mStrength;
		float mConstantAttenuation;
		float mLinearAttenuation;
		float mQuadraticAttenuation;
};