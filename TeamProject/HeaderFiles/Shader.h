#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <map>
#include <string>
#include <fstream>
#include <iostream>


namespace avt {

	const std::string MODEL_MATRIX = "ModelMatrix";
	const std::string NORMAL_MATRIX = "NormalMatrix";
	const std::string VIEW_MATRIX = "ViewMatrix";
	const std::string PROJECTION_MATRIX = "ProjectionMatrix";

	const std::string VERTEX_ATTRIBUTE = "inVertex";
	const std::string NORMAL_ATTRIBUTE = "inNormal";
	const std::string TEXCOORD_ATTRIBUTE = "inTexcoord";
	const std::string TANGENT_ATTRIBUTE = "inTangent";
	const std::string BITANGENT_ATTRIBUTE = "inBitangent";

	class Shader {
	public:
		GLuint _program;

		struct ShaderInfo {
			GLuint i;
		};
		std::map<GLenum, ShaderInfo> _shaders;

		struct AttributeInfo {
			GLuint i;
		};
		std::map<std::string, AttributeInfo> _attributes;

		struct UniformInfo {
			GLint i;
		};
		std::map<std::string, UniformInfo> _uniforms;

		struct UboInfo {
			GLuint block_index;
			GLuint binding_point;
		};
		std::map<std::string, UboInfo> _ubos;

		void parseShader(const std::string& filename, GLchar** output);

		Shader() : _program(0) {}
		~Shader() {}

		void addShader(GLenum shader_type, const std::string& filename);

		bool isAttribute(const std::string& name) {
			return _attributes.count(name);
		}

		void addUniform(const std::string& name) {
			UniformInfo uniformInfo;
			_uniforms.insert(std::pair<std::string, UniformInfo>(name, uniformInfo));
		}

		bool isUniform(const std::string& name) {
			return _uniforms.count(name);
		}

		GLuint getUniform(const std::string& name) {
			return _uniforms.find(name)->second.i;
		}

		void addAttribute(const std::string& name, const GLuint index) {
			_attributes.insert({ name, {index} });
		}

		void addUbo(const std::string& name, const GLuint binding_point) {
			_ubos.insert({ name, {0, binding_point} }); // block index will be determined later
		}

		bool isUniformBlock(const std::string& name) {
			return _ubos.count(name);
		}

		void create();

		void bind() {
			glUseProgram(_program);
		}

		void unbind() {
			glUseProgram(0);
		}

	};
}
