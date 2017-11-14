#include "Transformable.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace cmgl
{

Transformable::Transformable()
	: mTransform(glm::mat4())
	, mRotation(glm::mat4())
	, mPosition(glm::vec3(0.0f))
	, mScale(glm::vec3(1.0f))
	, mTransformUpdated(true)
{
}

void Transformable::setTransform(const glm::vec3& position, const glm::vec3& scale, const glm::mat4& rotation)
{
	mPosition = position;
	mScale = scale;
	mRotation = rotation;
	markAsDirty();
}

const glm::mat4& Transformable::getTransform() const
{
	if (!mTransformUpdated)
	{
		mTransform = glm::translate(mPosition) * mRotation * glm::scale(mScale);;
		mTransformUpdated = true;
	}
	return mTransform;
}

void Transformable::setPosition(float x, float y, float z)
{
	mPosition = glm::vec3(x, y, z);
	markAsDirty();
}

void Transformable::setPosition(const glm::vec3& position)
{
	mPosition = position;
	markAsDirty();
}

void Transformable::setScale(float s)
{
	mScale = glm::vec3(s, s, s);
	markAsDirty();
}

void Transformable::setScale(float x, float y, float z)
{
	mScale = glm::vec3(x, y, z);
	markAsDirty();
}

void Transformable::setScale(const glm::vec3& scale)
{
	mScale = scale;
	markAsDirty();
}

void Transformable::setRotation(const glm::mat4& rotation)
{
	mRotation = rotation;
	markAsDirty();
}

void Transformable::setRotation(const glm::quat& rotation)
{
	mRotation = glm::toMat4(rotation);
	markAsDirty();
}

void Transformable::move(float x, float y, float z)
{
	mPosition.x += x;
	mPosition.y += y;
	mPosition.z += z;
	markAsDirty();
}

void Transformable::move(const glm::vec3& movement)
{
	mPosition += movement;
	markAsDirty();
}

void Transformable::scale(float s)
{
	mScale *= s;
	markAsDirty();
}

void Transformable::scale(float x, float y, float z)
{
	mScale.x *= x;
	mScale.y *= y;
	mScale.z *= z;
	markAsDirty();
}

void Transformable::scale(const glm::vec3& scale)
{
	mScale *= scale;
	markAsDirty();
}

void Transformable::rotate(const glm::mat4& rotation)
{
	mRotation *= rotation;
	markAsDirty();
}

void Transformable::rotate(const glm::quat& rotation)
{
	mRotation *= glm::toMat4(rotation);
	markAsDirty();
}

const glm::vec3& Transformable::getPosition() const
{
	return mPosition;
}

const glm::vec3& Transformable::getScale() const
{
	return mScale;
}

const glm::mat4& Transformable::getRotation() const
{
	return mRotation;
}

void Transformable::markAsDirty()
{
	mTransformUpdated = false;
}

bool Transformable::isUpdated() const
{
	return mTransformUpdated;
}

} // namespace cmgl