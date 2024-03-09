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
	GLfloat GetBuggerHeight() { return bufferHeight; }
	bool getShouldClose() { return glfwWindowShouldClose(window); }
	void SwapBuffers() { glfwSwapBuffers(window); }
	~MyWindow();
private:
	GLFWwindow* window;
	GLint width = 800;
	GLint height = 600;
	GLint bufferWidth, bufferHeight;

};

