#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

namespace cmgl
{

Camera::Camera()
{
	perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	lookAt(glm::vec3(), glm::vec3());
}


void Camera::perspective(float fov, float ratio, float near, float far)
{
	mFov = fov;
	mRatio = ratio;
	mNear = near;
	mFar = far;
	updateProjection();
}

void Camera::setNear(float near)
{
	mNear = near;
	updateProjection();
}

void Camera::setFar(float far)
{
	mFar = far;
	updateProjection();
}

void Camera::setFOV(float fov)
{
	mFov = fov;
	updateProjection();
}

void Camera::setRatio(float ratio)
{
	mRatio = ratio;
	updateProjection();
}

const glm::mat4& Camera::getProjectionMatrix() const
{
	if (!mProjectionUpdated)
	{
		mProjection = glm::perspective(mFov, mRatio, mNear, mFar);
		mProjectionUpdated = true;
	}
	return mProjection;
}

float Camera::getNear() const
{
	return mNear;
}

float Camera::getFar() const
{
	return mFar;
}

float Camera::getFOV() const
{
	return mFov;
}

float Camera::getRatio() const
{
	return mRatio;
}

void Camera::lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up)
{
	mPosition = position;
	mTarget = target;
	mUp = up;
	updateView();
}

void Camera::setPosition(const glm::vec3& position)
{
	mPosition = position;
	updateView();
}

void Camera::setTarget(const glm::vec3& target)
{
	mTarget = target;
	updateView();
}

void Camera::setUp(const glm::vec3& up)
{
	mUp = up;
	updateView();
}

const glm::mat4& Camera::getViewMatrix() const
{
	if (!mViewUpdated)
	{
		mView = glm::lookAt(mPosition, mTarget, mUp);
		mViewUpdated = true;
	}
	return mView;
}

const glm::vec3& Camera::getPosition() const
{
	return mPosition;
}

const glm::vec3& Camera::getTarget() const
{
	return mTarget;
}

const glm::vec3& Camera::getUp() const
{
	return mUp;
}

void Camera::updateProjection()
{
	mProjectionUpdated = false;
}

void Camera::updateView()
{
	mViewUpdated = false;
}

} // namespace cmgl
