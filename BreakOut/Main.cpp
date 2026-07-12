#include"pch.h"
#include<Glad/glad.h>
#include<GLFW/glfw3.h>
#include"Game.h"
#include<iostream>
int main()
{
	GLFWwindow* window;
	if (!glfwInit())
		return -1;
	//----------窗口初始化设置----------
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	//----------窗口初始化设置----------

	//----------初始化窗口，创建上下文----------
	window = glfwCreateWindow(800, 600, "BreakOut", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}//安全检查
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize GLAD\n");
		return -1;
	}
	
	Game BreakOut(800,600);
	BreakOut.Init();

	float currentTime = 0.0f;
	float deltaTime = 0.0f;
	
	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - currentTime;
		currentTime = glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.0f, 0.5f, 1.0f, 1.0f);
		glfwPollEvents();
		BreakOut.ProcessInput(deltaTime);
		BreakOut.Update(deltaTime);
		BreakOut.Render();

		glfwSwapBuffers(window);
	}
	glfwTerminate();


}