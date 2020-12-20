#pragma once
#include "Shader.h"
#include "Camera.h"
#include "Engine.h"

namespace avt {
	class MousePicker
	{
	private:

		Vector3 _currentRay;

		Vector3 calculateMouseRay(Camera* camera, double xcursor, double ycursor){
			float mouseX = (float)xcursor;
			float mouseY= (float)ycursor;

			Vector2 normalizedCoords = getNormalizedDeviceCoords(mouseX, mouseY);
			Vector4 clipCoords = Vector4(normalizedCoords.x(), normalizedCoords.y(), -1.0f, 1.0f);
			Vector4 eyeCoords = toEyeCoords(clipCoords, camera->projMatrix());
			Vector3 worldRay = toWorldCoords(eyeCoords, camera->viewMatrix());
			std::cout <<  worldRay;
			return worldRay;
		}

		Vector2 getNormalizedDeviceCoords(float mouseX, float mouseY) {
			float x = (2.0f * mouseX) / WIDTH - 1.0f;
			float y = 1.0f - (2.0f * mouseY) / HEIGHT;
			return Vector2(x, y); //-y ?
		}

		Vector4 toEyeCoords(Vector4 clipCoords, Mat4 projMatrix) {
			Mat4 invertedProjMatrix = projMatrix.inverted();
			Vector4 eyeCoords = invertedProjMatrix * clipCoords;
			return Vector4(eyeCoords.x(), eyeCoords.y(), -1.0f, 0.0f);
		}

		Vector3 toWorldCoords(Vector4 eyeCoords, Mat4 viewMatrix) {
			Mat4 invertedViewMatrix = viewMatrix.inverted();
			Vector4 rayWorld = invertedViewMatrix * eyeCoords;
			return rayWorld.to3D().normalized();
		}

	public:  
	
		void create(Camera* camera) {

		}

		Vector3 getCurrentRay() {
			return _currentRay;
		}

		void update(Camera* camera, double xcursor, double ycursor) {
			//update view matrix
			_currentRay = calculateMouseRay(camera, xcursor, ycursor);
		}

	};
}

