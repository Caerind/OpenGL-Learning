#include "Application.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

Application::Application()
{
}

bool Application::init()
{
	if (!mWindow.create(1024, 768, "GL4"))
	{
		return false;
	}

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);

	if (!mImGui.init(mWindow))
	{
		return false;
	}

	mCamera.perspective(45.0f, ((float)mWindow.getSize().x) / ((float)mWindow.getSize().y), 0.1f, 100.0f);
	mCamera.lookAt(glm::vec3(-2, 1, -2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	mClearColor = cmgl::Color::LightBlue;

	if (!mTexture.loadFromFile("suzanne.png"))
	{
		return false;
	}

	if (!mShader.loadFromFile("MainShader.vert", "MainShader.frag"))
	{
		return false;
	}

	if (!mMesh.loadFromFile("suzanne.obj"))
	{
		return false;
	}

	mAsset.setMesh(mMesh);
	mAsset.setShader(mShader);
	mAsset.setTexture(mTexture);

	mInstance.setAsset(mAsset);

	mInstance2.setAsset(mAsset);
	mInstance2.setPosition(3, 0, 3);

	mPosition = mCamera.getPosition();
	mDirection = glm::normalize(glm::vec3() - mPosition);
	mRight = glm::cross(mDirection, glm::vec3(0, 1, 0));

	mAmbient = ImVec4(0.2f, 0.3f, 0.3f, 1.0f);
	mLightColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
	mShininess = 5.0f;
	mStrength = 1.0f;
	mConstantAttenuation = 0.5f;
	mLinearAttenuation = 0.5f;
	mQuadraticAttenuation = 0.5f;

	return true;
}

bool Application::run()
{
	float lastTime = (float)glfwGetTime();
	while (mWindow.isOpen())
	{
		float time = (float)glfwGetTime();
		float dt = lastTime - time;
		lastTime = time;

		mWindow.pollEvents();
		mImGui.newFrame();
		update(dt);

		mWindow.clear(mClearColor);
		render();
		mImGui.render();
		mWindow.display();
	}
	return true;
}

void Application::clear()
{
	mImGui.shutdown();
}

void Application::update(float dt)
{
	if (mWindow.isKeyPressed(GLFW_KEY_ESCAPE))
	{
		mWindow.close();
	}

	if (mWindow.isKeyPressed(GLFW_KEY_DOWN))
	{
		mPosition += mDirection * 2.0f * dt;
	}
	if (mWindow.isKeyPressed(GLFW_KEY_UP))
	{
		mPosition -= mDirection * 2.0f * dt;
	}
	if (mWindow.isKeyPressed(GLFW_KEY_RIGHT))
	{
		mPosition += mRight * 10.0f * dt;
	}
	if (mWindow.isKeyPressed(GLFW_KEY_LEFT))
	{
		mPosition -= mRight * 10.0f * dt;
	}
	mDirection = glm::normalize(glm::vec3() - mPosition);
	mRight = glm::cross(mDirection, glm::vec3(0, 1, 0));
	mCamera.setPosition(mPosition);

	// Debug Window
	{
		ImGui::ColorEdit3("Clear color", (float*)&mClearColor);
		ImGui::ColorEdit3("Ambient color", (float*)&mAmbient);
		ImGui::ColorEdit3("Light color", (float*)&mLightColor);
		ImGui::SliderFloat("Shininess", &mShininess, 0.0f, 10.0f);
		ImGui::SliderFloat("Strength", &mStrength, 0.0f, 10.0f);
		ImGui::SliderFloat("Catt", &mConstantAttenuation, 0.0f, 10.0f);
		ImGui::SliderFloat("Latt", &mLinearAttenuation, 0.0f, 10.0f);
		ImGui::SliderFloat("Qatt", &mQuadraticAttenuation, 0.0f, 10.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}

	mInstance.setRotation(glm::rotate(mInstance.getRotation(), 0.3f * dt, glm::vec3(0, 1, 0)));
}

void Application::render()
{
	mShader.bind();
	mShader.setUniform("Texture", cmgl::Shader::CurrentTexture);

	mShader.setUniform("Ambient", cmgl::Color(mAmbient));
	mShader.setUniform("LightColor", cmgl::Color(mLightColor));
	mShader.setUniform("LightPosition", glm::vec3(mCamera.getViewMatrix() * glm::vec4(mCamera.getPosition(), 1.0)));
	mShader.setUniform("EyeDirection", glm::vec3() - glm::vec3(mCamera.getViewMatrix() * glm::vec4(mCamera.getPosition(), 1.0)));
	mShader.setUniform("Shininess", mShininess);
	mShader.setUniform("Strength", mStrength);
	mShader.setUniform("ConstantAttenuation", mConstantAttenuation);
	mShader.setUniform("LinearAttenuation", mLinearAttenuation);
	mShader.setUniform("QuadraticAttenuation", mQuadraticAttenuation);

	mInstance.draw(mCamera.getViewMatrix(), mCamera.getProjectionMatrix());
	mInstance2.draw(mCamera.getViewMatrix(), mCamera.getProjectionMatrix());
}
