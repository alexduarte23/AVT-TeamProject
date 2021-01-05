#pragma once

#include <vector>
#include "avt_math.h"
#include "SceneNodeCallback.h"
#include "Light.h"

#include "Mesh.h"

namespace avt {
	//class Mesh;

	class SceneNode {
	private:
		Mesh* _mesh;
		SceneNode* _parent;
		std::vector<SceneNode*> _nodes;
		//Mat4 _matrix;

		Shader* _shader;

		Vector3 _translation, _scale;
		Quaternion _rot;

		SceneNodeCallback* _callback;

		//mouse picking
		unsigned int _stencilIndex = 0; //0 = not selectable
		bool _selected = false;
		//

	public:
		SceneNode(Mesh* mesh = nullptr)
			: _callback(nullptr), _parent(nullptr), _mesh(mesh), _translation(0, 0, 0), _scale(1.f, 1.f, 1.f), _rot({ 1.f,0,0 }, 0) /*, _matrix(Mat4::identity())*/,
			_shader(nullptr) {}

		virtual ~SceneNode() {
			for (auto node : _nodes) {
				delete node;
			}
		}

		SceneNode* createNode(Mesh* mesh = nullptr) {
			SceneNode* node = new SceneNode(mesh);
			_nodes.push_back(node);
			node->setParent(this);
			return node;
		}

		SceneNode* addNode(SceneNode* node) {
			_nodes.push_back(node);
			node->setParent(this);
			return node;
		}

		void setMesh(Mesh* mesh) {
			_mesh = mesh;
		}

		Mesh* getMesh() {
			return _mesh;
		}

		const std::vector<SceneNode*>& children() const {
			return _nodes;
		}

		void setShader(Shader* shader) {
			_shader = shader;
		}

		Shader* getShader() {
			return _shader;
		}

		virtual void draw(const Mat4& worldMatrix, Light* light) {
			if (_shader) draw(_shader, worldMatrix, light);
		}

		virtual void draw(Shader* shader, const Mat4& worldMatrix, Light* light) {
			auto newWorldMat = worldMatrix * getTransform();
			Shader* curr_shader = _shader ? _shader : shader;

			if (_mesh) {
				curr_shader->bind();
				_mesh->va().bind();

				glEnable(GL_STENCIL_TEST);
				glStencilFunc(GL_ALWAYS, getStencilIndex(), 0xFF);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

				glUniform3f(curr_shader->getUniform("LightPosition"), light->getPosition().x(), light->getPosition().y(), light->getPosition().z());
				glUniform3f(curr_shader->getUniform("LightColor"), light->getColor().x(), light->getColor().y(), light->getColor().z());

				beforeDraw();
				glUniformMatrix4fv(curr_shader->getUniform(MODEL_MATRIX), 1, GL_FALSE, newWorldMat.data());
				glDrawArrays(GL_TRIANGLES, 0, _mesh->vb().size());
				afterDraw();

				glDisable(GL_STENCIL_TEST);

				_mesh->va().unbind();
				curr_shader->unbind();
			}

			for (auto childNode : _nodes) {
				childNode->draw(curr_shader, newWorldMat, light);
			}
		}

		/*void setMatrix(const Mat4& matrix) {
			_matrix = matrix;
		}

		const Mat4& getMatrix() const {
			return _matrix;
		}
		
		void applyMatrix(const Mat4& matrix) {
			_matrix *= matrix;
		}*/

		Mat4 getTransform() const {
			return Mat4::scale(_scale)
				* _rot.toMat()
				* Mat4::translation(_translation);
		}

		void setTranslation(const Vector3& v) {
			_translation = v;
		}

		const Vector3& getTranslation() const {
			return _translation;
		}

		void translate(const Vector3& v) {
			_translation += v;
		}

		void setScale(const Vector3& v) {
			_scale = v;
		}

		const Vector3& getScale() const {
			return _scale;
		}

		void scale(const Vector3& v) {
			_scale *= v;
		}

		void rotateX(float rad) {
			_rot *= Quaternion({ 1.f,0,0 }, rad);
		}

		void rotateY(float rad) {
			_rot *= Quaternion({ 0,1.f,0 }, rad);
		}

		void rotateZ(float rad) {
			_rot *= Quaternion({ 0,0,1.f }, rad);
		}

		void setRotation(const Quaternion& q) {
			_rot = q;
		}

		const Quaternion& getRotation() const {
			return _rot;
		}

		void rotate(const Quaternion& q) {
			_rot *= q;
		}

		SceneNode* getParent() {
			return _parent;
		}

		void setParent(SceneNode* node) {
			_parent = node;
		}

		void setCallback(SceneNodeCallback* callback) {
			_callback = callback;
		}

		SceneNodeCallback* getCallback() {
			return _callback;
		}

		void beforeDraw() {
			if (_callback) _callback->beforeDraw();
		}

		void afterDraw() {
			if (_callback) _callback->afterDraw();
		}

		avt::Vector4 pos() {
			return _parent->getTransform()* getTransform() * avt::Vector4(0.0f, 0.0f, 0.0f, 1.f);
		}

		void setStencilIndex(unsigned int index) { //mouse picking
			_stencilIndex = index;
		}

		unsigned int getStencilIndex() { //mouse picking
			return _stencilIndex;
		}

		void selected(bool b) { //mouse picking
			_selected = b;
		}

		//return if node has been selected by mouse picker
		bool isSelected() { //mouse picking
			return _selected;
		}

	};

}