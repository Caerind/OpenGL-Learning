#pragma once

#include <map>

#include "Color.hpp"
#include "Texture.hpp"

namespace cmgl
{

class Shader
{
	public:
		enum SpecialUniforms
		{
			CurrentTexture
		};

	public:
		Shader();
		~Shader();

		bool loadFromFile(const std::string& vs, const std::string& fs);

		bool loadFromSource(const std::string& vs, const std::string& fs);

		void bind() const;
		bool isValid() const;

		void setUniform(const std::string& name, float x);
		void setUniform(const std::string& name, float x, float y);
		void setUniform(const std::string& name, float x, float y, float z);
		void setUniform(const std::string& name, float x, float y, float z, float w);
		void setUniform(const std::string& name, const glm::vec2& v);
		void setUniform(const std::string& name, const glm::vec3& v);
		void setUniform(const std::string& name, const glm::vec4& v);
		void setUniform(const std::string& name, const glm::mat3& m);
		void setUniform(const std::string& name, const glm::mat4& m);
		void setUniform(const std::string& name, const Color& color);
		void setUniform(const std::string& name, const Texture& texture);
		void setUniform(const std::string& name, SpecialUniforms type);

		int getAttribLocation(const std::string& name) const;

	private:
		int getUniformLocation(const std::string& name);

		bool isShaderCompiled(unsigned int shader, const std::string& shaderName) const;

		bool isProgramLinked() const;

	private:
		unsigned int mProgram;
		int mCurrentTexture;
		std::map<std::string, int> mUniforms;
		std::map<unsigned int, const Texture*> mTextures;
};

} // namespace cmgl