#include <iostream>

#include "window.h"
#include "keyboard.h"

static void error_callback(int error, const char* description) {
    std::cerr << "Error: " << description << std::endl;
}

int main() {
    Window window(error_callback, Keyboard::key_callback);
    window.run();
    std::cout << "Window Closed" << std::endl;
}