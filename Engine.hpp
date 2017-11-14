#pragma once

#include "Lib/Mesh.hpp"
#include "Lib/Shader.hpp"
#include "Lib/Texture.hpp"
#include "Lib/Transformable.hpp"

class ModelAsset
{
	public:
		ModelAsset()
			: mMesh(nullptr)
			, mShader(nullptr)
			, mTexture(nullptr)
		{
		}

		void setMesh(cmgl::Mesh& mesh)
		{
			mMesh = &mesh;
		}

		void setShader(cmgl::Shader& shader)
		{
			mShader = &shader;
		}

		void setTexture(cmgl::Texture& texture)
		{
			mTexture = &texture;
		}

		cmgl::Mesh* getMesh()
		{
			return mMesh;
		}

		cmgl::Shader* getShader()
		{
			return mShader;
		}

		cmgl::Texture* getTexture()
		{
			return mTexture;
		}

		void draw()
		{
			if (mMesh != nullptr)
			{
				if (mShader != nullptr)
				{
					mShader->bind();
					if (mTexture != nullptr)
					{
						mTexture->bind();
					}
				}
				mMesh->draw();
			}
		}

	private:
		cmgl::Mesh* mMesh;
		cmgl::Shader* mShader;
		cmgl::Texture* mTexture;
};

class ModelInstance : public cmgl::Transformable
{
	public:
		ModelInstance()
			: mAsset(nullptr)
		{
		}

		void setAsset(ModelAsset& asset)
		{
			mAsset = &asset;
		}

		void draw(const glm::mat4& v, const glm::mat4& p)
		{
			if (mAsset != nullptr)
			{
				if (mAsset->getShader() != nullptr)
				{
					glm::mat4 m = getTransform();
					glm::mat4 mv = v * m;
					glm::mat4 mvp = p * mv;
					glm::mat3 n = glm::transpose(glm::inverse(mv));
					mAsset->getShader()->setUniform("MV", mv);
					mAsset->getShader()->setUniform("N", n);
					mAsset->getShader()->setUniform("MVP", mvp);
				}

				mAsset->draw();
			}
		}

	private:
		ModelAsset* mAsset;
};