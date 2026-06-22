#include "Window.hpp"
#include "type/BasicTypes.hpp"

int main()
{
   gauzy::Window testWindow{ u8"薄云窗口测试 Gauzy Window Test", gauzy::type::SizeU{ 800U, 600U } };
   testWindow.show();

   return 0; 
}