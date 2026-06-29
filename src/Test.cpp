#include <iostream>

#include "Window.hpp"
#include "type/BasicTypes.hpp"
#include "comp/Component.hpp"
#include "graphic/Brush.hpp"

const gauzy::graphic::SolidColorBrush TEST_BRUSH{ gauzy::type::Color{ 0xFF000000 } };

class TestComp : public gauzy::comp::Component
{
public:
    void render(gauzy::graphic::Renderer& renderer) override
    {
        renderer.drawLine({ 0, 0 }, { 800U, 600U }, TEST_BRUSH);
        std::cout << "TestComp render\n";
    }
};

int main()
{
    gauzy::Window testWindow{ u8"薄云窗口测试 Gauzy Window Test", gauzy::type::SizeU{ 800U, 600U } };
    testWindow.onRendererRegistry.subscribe([] (auto& e)
    {
        std::cout << "Renderer registry\n";
        e->registerSolidColorBrush(TEST_BRUSH);
    });
    testWindow.getTopGroup().add(TestComp{});
    testWindow.show();

    return 0;
}