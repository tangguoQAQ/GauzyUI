#pragma once

#include "comp/Component.hpp"

namespace gauzy::comp
{
    /**
     * @brief 标签类，用于显示静态文本，文字背景透明（不绘制背景矩形）。
     */
    class Label : public Component
    {
    public:
        Label(const Label&) = default;
        Label(Label&&) noexcept = default;
        Label& operator=(const Label&) = default;
        Label& operator=(Label&&) noexcept = default;
        ~Label() override = default;

        Label(std::string text, type::Position2F position, type::SizeF size) noexcept;

        [[nodiscard]] const std::string& getTitle() const noexcept;
        void setTitle(const std::string& newTitle) noexcept;

        void render(graphic::Renderer& renderer) override;
    private:
        std::string title;
    };
}