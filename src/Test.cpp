#include "Window.hpp"

int main() {
   gauzy::comp::Window testWindow{ u8"薄云窗口测试 Gauzy Window Test", gauzy::type::Vec2d{ 800, 600 } };
   testWindow.show();

   return 0; 
}