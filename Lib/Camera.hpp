#pragma once

#include <glm/glm.hpp>

// TODO : Add Transformable
// TODO : Direction and distance
// TODO : FreeCam

namespace cmgl
{

class Camera
{
	public:
		Camera();

		void perspective(float fov, float ratio, float near, float far);

		void setNear(float near);
		void setFar(float far);
		void setFOV(float fov);
		void setRatio(float ratio);

		const glm::mat4& getProjectionMatrix() const;

		float getNear() const;
		float getFar() const;
		float getFOV() const;
		float getRatio() const;

		void lookAt(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f));

		void setPosition(const glm::vec3& position);
		void setTarget(const glm::vec3& target);
		void setUp(const glm::vec3& up);

		const glm::mat4& getViewMatrix() const;

		const glm::vec3& getPosition() const;
		const glm::vec3& getTarget() const;
		const glm::vec3& getUp() const;

	protected:
		void updateProjection();
		void updateView();

	protected:	
		mutable glm::mat4 mProjection;
		mutable glm::mat4 mView;

		glm::vec3 mPosition;
		glm::vec3 mTarget;
		glm::vec3 mUp;

		float mNear;
		float mFar;
		float mRatio;
		float mFov;

		mutable bool mProjectionUpdated;
		mutable bool mViewUpdated;
};

} // namespace cmgl