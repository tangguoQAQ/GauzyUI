#include <iostream>

#include "Window.hpp"
#include "type/BasicTypes.hpp"
#include "comp/Label.hpp"
#include "graphic/Theme.hpp"

int main()
{
    using namespace gauzy;
    using namespace gauzy::type;
    
    Window testWindow{ u8"薄云窗口测试 Gauzy Window Test", SizeU{ 800U, 600U } };
    testWindow.theme() = graphic::GAUZY_DARK_THEME;

    testWindow.getTopGroup().add(
        comp::Label{ u8"Hello, World!", Position2F{ 16.0F, 16.0F }, SizeF{ 200.0F, 32.0F } },
        comp::Label{ u8"你好，世界！", Position2F{ 16.0F, 48.0F }, SizeF{ 200.0F, 32.0F } });
    
    testWindow.show();

    return 0;
}