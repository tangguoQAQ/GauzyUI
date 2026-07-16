#include <iostream>

#include "Window.hpp"
#include "type/BasicTypes.hpp"
#include "comp/Label.hpp"
#include "comp/ComponentGroup.hpp"
#include "comp/Interactive.hpp"
#include "graphic/Theme.hpp"
#include "event/ComponentEventContext.hpp"

using namespace gauzy;

class TestInteractive : public comp::Component, public comp::Interactive
{
public:
    using comp::Component::Component;

    void render(graphic::Renderer& renderer) override
    {
        renderer.drawRect(getPosition(), getSize(), renderer.theme().foregroundBrush);
    }
};

int main()
{
    using namespace gauzy::type;

    Window testWindow{ u8"薄云窗口测试 Gauzy Window Test", SizeU{ 800U, 600U } };
    testWindow.theme() = graphic::GAUZY_DARK_THEME;

    testWindow.topGroup().add(
        comp::Label{ u8"Hello, World!", Position2F{ 16.0F, 16.0F }, SizeF{ 200.0F, 32.0F } },
        comp::Label{ u8"你好，世界！", Position2F{ 16.0F, 48.0F }, SizeF{ 200.0F, 32.0F } });
    
    TestInteractive testComp{ Position2F{ 300.0F, 200.0F }, SizeF{ 150.0F, 50.0F } };
    testComp.onClicked.subscribe([](event::ComponentEventContext& context)
    {
        std::cout << "TestInteractive clicked" << std::endl;
    });
    testWindow.topGroup().add(testComp);

    testWindow.show();

    return 0;
}