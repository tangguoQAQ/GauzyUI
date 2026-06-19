#include <a>

class MyComp : public gauzy::comp::Panel
{
public:
    MyComp(gauzy::UiString title) : Panel()
    {
        Panel::add(
            Label{ title },
            Button{ title + " Button" }
        )
    }
};

class TestApp : public gauzy::Application
{
public:
    TestApp() {}

    virtual gauzy::comp::Window buildWindow() override
    {
        using namespace gauzy::comp;

        return Window mainWindow{ "GauzyUI Test", vec2d{ 800, 600 },
            Label{ "Hello, GauzyUI!" },
            Panel{
                Label{ "Panel Label" },
                Button{ "Panel Button" },
                MyComp{ "Custom 1" }
            },
            Label{ bind( "count: {}", &count_ ) },
            Button{ "OK", event::Clicked{ [&] () { count_++ } } },
            Button{ "Foo", event::Clicked{ FooButtonClicked } }
        };
    }

private:
    gauzy::State<int> count_{ 0 };
};

gauzy::Application gauzyMain()
{
    return TestApp{};
}
