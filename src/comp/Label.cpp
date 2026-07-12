#include "comp/Label.hpp"

#include "graphic/Renderer.hpp"

namespace gauzy::comp
{
    Label::Label(std::string text, type::Position2F position, type::SizeF size) noexcept :
        Component(position, size),
        title(std::move(text))
    {
    }

    const std::string& Label::getTitle() const noexcept
    {
        return title;
    }

    void Label::setTitle(const std::string& newTitle) noexcept
    {
        title = newTitle;
    }

    void Label::render(graphic::Renderer& renderer)
    {
        renderer.drawText(getPosition(), title,
            renderer.theme().foregroundBrush, renderer.theme().bodyTextFormat);
    }
} // namespace gauzy::comp
