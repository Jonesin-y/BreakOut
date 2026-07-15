#include"pch.h"
#include<Glad/glad.h>
#include<GLFW/glfw3.h>
#include"Game.h"
#include<iostream>
#include<Windows.h>

Game BreakOut(1400, 1200);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mod)
{
	//printf("key=%d action=%d\n", key, action);
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key >= 0 && key <= 1024)
	{
		if (action == GLFW_PRESS)
			BreakOut.Keys[key] = true;
		else if (action == GLFW_RELEASE)
			BreakOut.Keys[key] = false;
	}
}

int main()
{
	SetConsoleOutputCP(CP_UTF8);  // 设置控制台编码为UTF-8，解决中文乱码
	GLFWwindow* window;
	std::cout << "创建窗口" << std::endl;
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
	window = glfwCreateWindow(1400, 1200, "BreakOut", NULL, NULL);
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
	glfwSetKeyCallback(window,key_callback);
	BreakOut.Init();

	float currentTime = 0.0f;
	float deltaTime = 0.0f;
	glEnable(GL_BLEND);
	while (!glfwWindowShouldClose(window))
	{
		deltaTime = glfwGetTime() - currentTime;
		currentTime = glfwGetTime();
		glClear(GL_COLOR_BUFFER_BIT);
		glfwPollEvents();
		BreakOut.ProcessInput(deltaTime);
		BreakOut.Update(deltaTime);
		BreakOut.Render();
		

		glfwSwapBuffers(window);
	}
	glfwTerminate();


}

