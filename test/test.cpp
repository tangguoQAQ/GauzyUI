
int main()
{
	// ===== Create a window =====
	// No initial parameters.
	auto main_window = gauzy::window{};
	main_window.set_title("GauzyUI Test");
	main_window.set_size(800, 600);

	// With initial parameters. Just one line.
	auto main_window = gauzy::window{ "GauzyUI Test", vec2d{ 800, 600 } };

	// ===== Add a event handler =====
	// Add a event handler for the window.
	main_window.add_event_handler([](gauzy::event::paint e)
		{
			renderer.render();
		});

	// ===== Handle the renderer =====
	// When a window is created, a renderer is created internally.
	// Window is just the bearer (or container) of the renderer. Yes, this is DirectUi.
	// A window's renderer dicides how its client area is rendered -- its looks.

	// Get the renderer.
	auto renderer = main_window.get_renderer();

	renderer.set_background_color(gauzy::color::white);

	return main_window.show();
}