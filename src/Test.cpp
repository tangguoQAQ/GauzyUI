#include <iostream>

#include "Window.hpp"
#include "type/BasicTypes.hpp"
#include "comp/Component.hpp"
#include "graphic/Brush.hpp"

const gauzy::graphic::SolidColorBrush TEST_BRUSH{ gauzy::type::Color{ 0xF0F000F0 } };
const gauzy::graphic::SolidColorBrush TEST2_BRUSH{ gauzy::type::Color{ 0xFF0000F0 } };

class TestComp : public gauzy::comp::Component
{
public:
    void render(gauzy::graphic::Renderer& renderer) override
    {
        renderer.drawLine({ 0, 0 }, { 800U, 600U }, TEST_BRUSH);
        renderer.drawLine({ 0, 600U }, { 800U, 0U }, TEST2_BRUSH);
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
        e->registerSolidColorBrush(TEST2_BRUSH);
    });
    testWindow.getTopGroup().add(TestComp{});
    testWindow.show();

    return 0;
}