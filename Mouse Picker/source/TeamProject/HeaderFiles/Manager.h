#pragma once

#include <map>
#include <string>
#include <functional>

namespace avt {

	template<class T>
	class Manager {
	private:
		std::map<std::string, T*> _elements;
	public:
		Manager() {}
		~Manager() {
			for (auto& el : _elements) {
				delete el.second;
			}
		}

		T* add(const std::string& name, T* el) {
			if (_elements.count(name)) return nullptr;

			_elements.insert(std::pair<std::string, T*>(name, el));
			return el;
		}

		T* get(const std::string& name) {
			auto it = _elements.find(name);
			if (it == _elements.end()) return nullptr;
			else return it->second;
		}

		size_t size() const {
			return _elements.size();
		}

		typename std::map<std::string, T*>::iterator begin() noexcept {
			return _elements.begin();
		}

		typename std::map<std::string, T*>::iterator end() noexcept {
			return _elements.end();
		}

		void foreach(std::function<void(std::string, T*)> func) {
			for (auto& el : _elements) {
				func(el.first, el.second);
			}
		}

		void foreach(std::function<void(T*)> func) {
			for (auto& el : _elements) {
				func(el.second);
			}
		}

		void clear() {
			for (auto& el : _elements) {
				delete el.second;
			}
			_elements.clear();
		}

		// del == true: el is deleted, returns nullptr
		// del == false: el removed but not deleted, returns pointer to el
		T* remove(const std::string& name, bool del = true) {
			auto it = _elements.find(name);
			if (it == _elements.end()) return nullptr;
		
			auto value = it->second;
			_elements.erase(it);
			if (del) {
				delete value;
				return nullptr;
			} else {
				return value;
			}
			
		}

	};

}