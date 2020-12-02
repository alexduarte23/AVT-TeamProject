#include "../HeaderFiles/Shader.h"

namespace avt {

	void Shader::parseShader(const std::string& filename, GLchar** output) {
		std::ifstream fileStream(filename);
		std::string shader;
		std::string line;

		while (std::getline(fileStream, line)) {
			shader.append(line + '\n');
		}
		if (fileStream.eof()) {
			*output = new GLchar[shader.length() + 1];
			strcpy_s(*output, shader.length() + 1, shader.c_str());
		}
		else {
			std::cout << "Error reading file " << filename << std::endl;
		}

	}

	void Shader::addShader(GLenum shader_type, const std::string& filename) {
		GLchar* shaderChar;
		parseShader(filename, &shaderChar);

		unsigned int shaderId = glCreateShader(shader_type);
		glShaderSource(shaderId, 1, &shaderChar, 0);
		glCompileShader(shaderId);

		_shaders.insert({ shader_type, {shaderId} });
		delete[] shaderChar;
	}

	void Shader::create() {
		_program = glCreateProgram();

		for (auto& el : _shaders) { // Shader Attach
			glAttachShader(_program, el.second.i);
		}

		for (auto& el : _attributes) { // Attribute Bind
			glBindAttribLocation(_program, el.second.i, el.first.c_str());
		}

		glLinkProgram(_program);

		for (auto& el : _uniforms) { // Uniforms
			el.second.i = glGetUniformLocation(_program, el.first.c_str());
		}

		for (auto& el : _ubos) { // UBOS
			el.second.i = glGetUniformBlockIndex(_program, el.first.c_str());
			glUniformBlockBinding(_program, el.second.i, el.second.binding_point);
		}

		for (auto& el : _shaders) { // Shader Delete
			glDetachShader(_program, el.second.i);
			glDeleteShader(el.second.i);
		}
	}

}