#include "Shader.hpp"

#include <GL/glew.h>

#include <fstream>
#include <sstream>

namespace cmgl
{

Shader::Shader()
	: mProgram(0)
	, mCurrentTexture(-1)
{
}

Shader::~Shader()
{
	if (isValid())
	{
		glDeleteProgram(mProgram);
	}
}

bool Shader::loadFromFile(const std::string& vs, const std::string& fs)
{
	bool error = false;

	std::ifstream vsFile(vs, std::ios::in | std::ios::binary);
	if (!vsFile)
	{
		fprintf(stderr, "Failed to open vertex shader : %s\n", vs.c_str());
		error = true;
	}
	std::stringstream vsBuffer;
	vsBuffer << vsFile.rdbuf();

	std::ifstream fsFile(fs, std::ios::in | std::ios::binary);
	if (!fsFile)
	{
		fprintf(stderr, "Failed to open fragment shader : %s\n", fs.c_str());
		error = true;
	}
	std::stringstream fsBuffer;
	fsBuffer << fsFile.rdbuf();

	if (error)
	{
		return false;
	}

	return loadFromSource(vsBuffer.str(), fsBuffer.str());
}

bool Shader::loadFromSource(const std::string& vs, const std::string& fs)
{
	if (glIsProgram(mProgram))
	{
		glDeleteProgram(mProgram);
	}
	mProgram = 0;
	mCurrentTexture = -1;
	mUniforms.clear();
	mTextures.clear();

	GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
	const char* vertex_source = vs.c_str();
	glShaderSource(vertex, 1, &vertex_source, nullptr);
	glCompileShader(vertex);
	if (!isShaderCompiled(vertex, "Vertex"))
	{
		return false;
	}

	GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragment_source = fs.c_str();
	glShaderSource(fragment, 1, &fragment_source, nullptr);
	glCompileShader(fragment);
	if (!isShaderCompiled(fragment, "Fragment"))
	{
		return false;
	}

	mProgram = glCreateProgram();
	glAttachShader(mProgram, vertex);
	glAttachShader(mProgram, fragment);
	glLinkProgram(mProgram);
	if (!isProgramLinked())
	{
		return false;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}

void Shader::bind() const
{
	glUseProgram(mProgram);
	auto itr = mTextures.begin();
	for (std::size_t i = 1; i <= mTextures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glUniform1i(itr->first, i);
		itr->second->bind();
		++itr;
	}
	glActiveTexture(GL_TEXTURE0);
	if (mCurrentTexture != -1)
	{
		glUniform1i(mCurrentTexture, 0);
	}
}

bool Shader::isValid() const
{
	return glIsProgram(mProgram) == GL_TRUE;
}

void Shader::setUniform(const std::string& name, float x)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform1f(location, x);
	}
}

void Shader::setUniform(const std::string& name, float x, float y)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform2f(location, x, y);
	}
}

void Shader::setUniform(const std::string& name, float x, float y, float z)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform3f(location, x, y, z);
	}
}

void Shader::setUniform(const std::string& name, float x, float y, float z, float w)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform4f(location, x, y, z, w);
	}
}

void Shader::setUniform(const std::string& name, const glm::vec2& v)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform2fv(location, 1, &v[0]);
	}
}

void Shader::setUniform(const std::string& name, const glm::vec3& v)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform3fv(location, 1, &v[0]);
	}
}

void Shader::setUniform(const std::string& name, const glm::vec4& v)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform4fv(location, 1, &v[0]);
	}
}

void Shader::setUniform(const std::string& name, const glm::mat3& m)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		glUniformMatrix3fv(location, 1, GL_FALSE, &m[0][0]);
	}
}

void Shader::setUniform(const std::string& name, const glm::mat4& m)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
	}
}

void Shader::setUniform(const std::string& name, const Color& color)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		glUniform4f(location, (float)color.r / 255.0f, (float)color.g / 255.0f, (float)color.b / 255.0f, (float)color.a / 255.0f);
	}
}

void Shader::setUniform(const std::string& name, const Texture& texture)
{
	GLint location = getUniformLocation(name);
	if (location != -1)
	{
		auto itr = mTextures.find(location);
		if (itr == mTextures.end())
		{
			mTextures[location] = &texture;
		}
		else
		{
			itr->second = &texture;
		}
	}
}

void Shader::setUniform(const std::string& name, SpecialUniforms type)
{
	switch (type)
	{
		case CurrentTexture: mCurrentTexture = getUniformLocation(name); break;
	}
}

int Shader::getAttribLocation(const std::string& name) const
{
	return glGetAttribLocation(mProgram, name.c_str());
}

int Shader::getUniformLocation(const std::string& name)
{
	auto it = mUniforms.find(name);
	if (it != mUniforms.end())
	{
		return it->second;
	}
	else
	{
		GLint location = glGetUniformLocation(mProgram, name.c_str());
		mUniforms.insert(std::make_pair(name, location));
		if (location == -1)
		{
			fprintf(stderr, "Uniform \"%s\" not found in shader\n", name.c_str());
		}
		return location;
	}
}

bool Shader::isShaderCompiled(unsigned int shader, const std::string& shaderName) const
{
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLsizei len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(shader, len, &len, log);
		printf("%s shader compilation failed : %s\n", shaderName.c_str(), log);
		delete[] log;
		return false;
	}
	return true;
}

bool Shader::isProgramLinked() const
{
	GLint linked;
	glGetProgramiv(mProgram, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei len;
		glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &len);
		GLchar* log = new GLchar[len + 1];
		glGetProgramInfoLog(mProgram, len, &len, log);
		printf("Program linking failed: %s\n", log);
		delete[] log;
		return false;
	}
	return true;
}

} // namespace cmgl