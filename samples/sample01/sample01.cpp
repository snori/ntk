#include <ntk/ntk.h>
#include <ntk/main.h>

using namespace ntk;

class Sample01Window : public Window {
public:
	//
	// constants
	//
	enum {
		BUTTON_1,
		BUTTON_2,
		BUTTON_3,
		QUIT_BUTTON,
	};

	//
	// methods
	//
	Sample01Window()
	:	Window(
			Rect(100, 100, 600, 400),
			application()->name() + " v" + application()->version().as_string(),
			TITLED_WINDOW)//STD_TITLED_WINDOW)
	{
		add_child(new Button(Rect(10, 10, 100, 40), "Button1", "Button1", new Message(BUTTON_1)));
		add_child(new Button(Rect(10, 50, 100, 80), "Button2", "Button2", new Message(BUTTON_2)));
		add_child(new Button(Rect(10, 90, 100, 120), "Button3", "Button3", new Message(BUTTON_3)));

		add_child(new Button(Rect(100, 200, 350, 230), "QuitButton", "*QUIT* Application", new Message(QUIT_BUTTON), View::FOLLOW_RIGHT | View::FOLLOW_BOTTOM));
	}

	bool quit_requested()
	{
		application()->quit();
		return true;
	}
	void message_received(const Message& message)
	{
		switch(message.what)
		{
		case BUTTON_1:
			message_box("MessageBox", "Button1 ‚ª‰Ÿ‚³‚ê‚Ü‚µ‚½B");
			break;

		case BUTTON_2:
			message_box("MessageBox", "Button2 ‚ª‰Ÿ‚³‚ê‚Ü‚µ‚½B");
			break;

		case BUTTON_3:
			message_box("MessageBox", "Button3 ‚ª‰Ÿ‚³‚ê‚Ü‚µ‚½B");
			break;

		case QUIT_BUTTON:
			post_message(QUIT_REQUESTED);
			break;

		default:
			Window::message_received(message);
		}
	}

};

class Sample01 : public Application {
public:
	//
	// methods
	//
	Sample01()
	:	Application("NTK Sample01", Version(1, 0, 0, 0, "20020826"))
	{
		Sample01Window* window = new Sample01Window();
		window->show();
	}

};

int
main(int argc, const char_t** argv)
{
	Sample01 app;
	app.run();

	return 0;
}
