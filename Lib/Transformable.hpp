#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace cmgl
{

class Transformable
{
	public:
		Transformable();

		void setTransform(const glm::vec3& position, const glm::vec3& scale, const glm::mat4& rotation);
		const glm::mat4& getTransform() const;

		void setPosition(float x, float y, float z);
		void setPosition(const glm::vec3& position);
		void setScale(float s);
		void setScale(float x, float y, float z);
		void setScale(const glm::vec3& scale);
		void setRotation(const glm::mat4& rotation);
		void setRotation(const glm::quat& rotation);

		void move(float x, float y, float z);
		void move(const glm::vec3& movement);
		void scale(float s);
		void scale(float x, float y, float z);
		void scale(const glm::vec3& scale);
		void rotate(const glm::mat4& rotation);
		void rotate(const glm::quat& rotation);

		const glm::vec3& getPosition() const;
		const glm::vec3& getScale() const;
		const glm::mat4& getRotation() const;

	protected:
		void markAsDirty();

		bool isUpdated() const;

		// TODO : SIGNALS
		// -> markedDirty
		// -> updated

	private:
		mutable glm::mat4 mTransform;

		glm::mat4 mRotation; // Don't know yet : compute each time quat<->mat4 for memory or as mat4 for speed (avoid recomputation)
		glm::vec3 mPosition;
		glm::vec3 mScale;

		mutable bool mTransformUpdated;
};

} // namespace cmgl