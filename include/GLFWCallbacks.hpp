#pragma once
struct GLFWwindow;
void glfw_ResizeCallback(GLFWwindow* win_ptr, int width, int height);
void glfw_MoveCallback(GLFWwindow* win_ptr, int xpos, int ypos);
void glfw_ErrorCallback(int error, const char* description);
void glfw_MouseMoveCallback(GLFWwindow* window, double x, double y);
