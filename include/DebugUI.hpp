#pragma once 
struct GLFWwindow;
struct DebugUI{
     DebugUI() = default;
    ~DebugUI() = default;
    void setupDebugUI(GLFWwindow* win_ptr);
    void drawDebugUI();
    void destroyDebugUI();
    void render();
private:
    void ShowOverlay(bool* p_open);
    void drawUI();
    GLFWwindow* win_ptr;
};
