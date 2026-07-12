#include <iostream>

#include "Window.hpp"
#include "type/BasicTypes.hpp"
#include "comp/Component.hpp"
#include "graphic/Brush.hpp"

const gauzy::graphic::SolidColorBrush TEST_BRUSH{ gauzy::type::Color{ 0xF0F000F0 } };

const gauzy::graphic::TextFormat TEST_TEXT_FORMAT{ "微软雅黑" };

class TestComp : public gauzy::comp::Component
{
public:
    void render(gauzy::graphic::Renderer& renderer) override
    {
        static const gauzy::graphic::SolidColorBrush TEST2_BRUSH{ gauzy::type::Color{ 0xFF0000F0 } };

        renderer.drawLine({ 0U, 0U }, { 800U, 600U }, TEST_BRUSH);
        renderer.drawLine({ 0U, 600U }, { 800U, 0U }, TEST2_BRUSH);
        renderer.drawRect({ 100U, 100U }, { 200U, 150U }, TEST2_BRUSH);
        renderer.fillRect({ 300U, 100U }, { 200U, 150U }, TEST_BRUSH);

        renderer.drawText({ 0U, 0U }, "Hello, World!",
            TEST_BRUSH, TEST_TEXT_FORMAT);
    }
};

int main()
{
    gauzy::Window testWindow{ u8"薄云窗口测试 Gauzy Window Test", gauzy::type::SizeU{ 800U, 600U } };
    testWindow.onRendererRegistry.subscribe([] (auto& e)
    {
        std::cout << "Renderer registry\n" << std::flush;
    });
    testWindow.getTopGroup().add(TestComp{});
    testWindow.show();

    return 0;
}