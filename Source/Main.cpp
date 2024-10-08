// In loving memory of Riley.
// ~2008-2/8/2024
// R.I.P.


char versionString[128];
bool bitness;

// Make it so on laptops, it will request the dGPU if possible, without this, you have to force it to use the dGPU
extern "C"
{
	__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}

extern "C"
{
	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <time.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include "Input.h"
#include "Renderer.h"
#include "Shader.h"
#include "SingleplayerHandler.h"
#include "Texture.h"
#include "Window.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);


// TODO: Replace with JSON file loading or something
int windowWidth = 640;
int windowHeight = 480;
const char* windowTitle = "KiwiCubed Engine";

int main() {

	// Initialize GLFW
	if (!glfwInit())
	{
		std::cerr << "[Initialization / Error] Failed to initialize GLFW" << std::endl;
		return -1;
	}
	else {
		std::cout << "[Initialization / Info] Successfully initialized GLFW" << std::endl;
	}

	// Create a window
	Window globalWindow = Window(windowWidth, windowHeight, windowTitle);
	strcpy_s(versionString, windowTitle);
	strcat_s(versionString, "v0.0.2pre-alpha");
	globalWindow.SetTitle(versionString);
	globalWindow.Setup();

	glfwSetWindowUserPointer(globalWindow.GetWindowInstance(), &globalWindow);
	glfwSetFramebufferSizeCallback(globalWindow.GetWindowInstance(), framebuffer_size_callback);

	// Initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "[Initialization / Error] Failed to initialize GLAD" << std::endl << std::endl;
		return -1;
	}
	else {
		std::cout << "[Initialization / Info] Successfully initialized GLAD" << std::endl << std::endl;
	}

	// Setup ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplGlfw_InitForOpenGL(globalWindow.GetWindowInstance(), true);
	ImGui_ImplOpenGL3_Init("#version 430");

	if (sizeof(void*) == 8) {
		bitness = 1;
	}
	else if (sizeof(void*) == 4) {
		bitness = 0;
	}
	else {
		std::cerr << "[Initialization / Error]: Could not find machine bitness" << std::endl;
		return -1;
	}
	std::cout << "[Initialization / Info] Machine bitness: " << (bitness == 1 ? "64" : "32") << std::endl;
	std::cout << "[Initialization / Info] Using OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "[Initialization / Info] Using graphics device: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "[Initialization / Info] Using resolution: " << globalWindow.GetWidth() << " x " << globalWindow.GetHeight() << std::endl << std::endl;


	// Set things up before main game loop
	GLCall(glViewport(0, 0, globalWindow.GetWidth(), globalWindow.GetHeight()));
	GLCall(glEnable(GL_DEPTH_TEST));

	// Create a singleplayer world
	SingleplayerHandler singleplayerHandler(globalWindow);
	singleplayerHandler.Setup();
	singleplayerHandler.StartSingleplayerWorld();

	// Create a debug shader
	Shader shaderProgram("Resources/Shaders/Vertex.vert", "Resources/Shaders/Fragment.frag");

	// Create a debug texture
	Texture textureAtlas("Resources/Textures/Blocks/tex_coords_test_img.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

	// Start generating a chunk in the singleplayer world
	singleplayerHandler.singleplayerWorld.GenerateWorld();

	// Bind stuff
	textureAtlas.Bind();
	shaderProgram.Bind();

	Renderer renderer = Renderer();

	int frames = 0;

	// Main game loop
	while (!glfwWindowShouldClose(globalWindow.GetWindowInstance())) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//ImGui::ShowDemoWindow();

		// Make background ~pink~
		renderer.ClearScreen(0.98f, 0.88f, 1.0f);

		// Do rendering stuff
		globalWindow.QueryInputs();
		if (singleplayerHandler.isLoadedIntoSingleplayerWorld) {
			singleplayerHandler.singleplayerWorld.player.Update(&globalWindow, shaderProgram, "windowViewMatrix");
		}

		singleplayerHandler.singleplayerWorld.Render(shaderProgram);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(globalWindow.GetWindowInstance());
		++frames;
	}

	// Clean up once the program has exited
	std::cout << "[Cleanup / Info] Cleaned up, exiting program" << std::endl;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(globalWindow.GetWindowInstance());
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	if (width == 0 || height == 0) {
		return;
	}
	glViewport(0, 0, width, height);
	Window* globalWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (globalWindow) {
		globalWindow->UpdateWindowSize(width, height);
	}
}