#pragma once

#include <string>

#include "../Dependencies/stb_image.h"

namespace avt {

	class Texture {
	private:
		unsigned int _texId;
		int _width, _height, _nrChannels;

		GLenum _wrap[2] = { 0,0 };
		GLenum _filter[2] = { 0,0 };
		bool _mipmap = false;

	public:
		Texture()
			: _texId(0), _width(0), _height(0), _nrChannels(9) {}

		~Texture() {
			if (_texId) {
				glBindTexture(GL_TEXTURE_2D, 0);
				glDeleteTextures(1, &_texId);
			}
		}

		void setWrap(GLenum wrapS, GLenum wrapT) {
			_wrap[0] = wrapS;
			_wrap[1] = wrapT;
		}

		void setFilter(GLenum filterMin, GLenum filterMag) {
			_filter[0] = filterMin;
			_filter[1] = filterMag;
		}

		void useMipmap() {
			_mipmap = true;
		}

		void create(const std::string& filename) {
			if (_texId) {
				glBindTexture(GL_TEXTURE_2D, 0);
				glDeleteTextures(1, &_texId);
			}

			glGenTextures(1, &_texId);
			glBindTexture(GL_TEXTURE_2D, _texId);

			if (_wrap[0]) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap[0]);
			if (_wrap[1]) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			if (_filter[0]) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _filter[0]);
			if (_filter[1]) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _filter[1]);

			unsigned char* data = stbi_load(filename.data(), &_width, &_height, &_nrChannels, 0);
			if (data) {
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				if (_mipmap) glGenerateMipmap(GL_TEXTURE_2D);
			}
			else { // use error manager
				std::cout << "Failed to load texture" << std::endl;
			}

			stbi_image_free(data);
		}

		void bind() {
			glBindTexture(GL_TEXTURE_2D, _texId);
		}

		void unbind() {
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	};

}