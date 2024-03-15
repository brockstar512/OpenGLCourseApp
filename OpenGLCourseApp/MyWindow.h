#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <cmath>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
class MyWindow
{

public:
	MyWindow();
	MyWindow(GLint windowWidth, GLint windowHeight);
	int Init();
	GLfloat GetBufferWidth() { return bufferWidth; }
	GLfloat GetBufferHeight() { return bufferHeight; }
	bool getShouldClose() { return glfwWindowShouldClose(window); }
	void SwapBuffers() { glfwSwapBuffers(window); }
	bool* GetKeys() { return keys; }
	GLfloat GetXChange();
	GLfloat GetYChange();
	~MyWindow();
private:
	GLFWwindow* window;
	GLint width = 800;
	GLint height = 600;
	GLint bufferWidth, bufferHeight;
	bool keys[1024];
	void CreateCallbacks();
	static void HandleKeys(GLFWwindow* window, int key, int code, int action,int mode);
	static void HandleMouse(GLFWwindow* window, double xPos, double yPos);
	//anytime the last coordinates of the mouse was
	GLfloat lastX;
	GLfloat lastY;
	//the change of the last and current coordinates
	GLfloat xChange = 0.0f;
	GLfloat yChange = 0.0f;
	//have a delay before you first initialize game
	bool mouseFirstMove;

};

