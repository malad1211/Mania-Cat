#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

int key[4] = { 0x5A,   0x58 , VK_OEM_COMMA, VK_OEM_PERIOD };
bool last_keystroke[4] = { FALSE , FALSE , FALSE , FALSE };
bool keystroke[4] = { FALSE , FALSE , FALSE , FALSE };
bool display = FALSE;
HHOOK keyboardHook;
sf::Texture texture[9];
sf::Sprite Cat, LeftPaw, RightPaw;
sf::RenderWindow window(sf::VideoMode(1189, 669), "Mania Cat");

// Get window handle
sf::WindowHandle handle = window.getSystemHandle();

void Keypress(bool UP, int i) {
	bool changed = FALSE;
	if (UP == FALSE) keystroke[i] = TRUE;
	else if (UP == TRUE) keystroke[i] = FALSE;

	if (last_keystroke[i] != keystroke[i]) {
		changed = TRUE;
		last_keystroke[i] = keystroke[i];
	}

	if (changed) {
		display = TRUE;
		//std::this_thread::sleep_for(std::chrono::milliseconds(50));

		int temp = (keystroke[0] * 10) + keystroke[1];
		switch (temp) {
		case 10:
			LeftPaw.setTexture(texture[1]);
			break;
		case 11:
			LeftPaw.setTexture(texture[2]);
			break;
		case 01:
			LeftPaw.setTexture(texture[3]);
			break;
		case 0:
			LeftPaw.setTexture(texture[7]);
		}

		temp = (keystroke[3] * 10) + keystroke[2];
		temp = 20;
		switch (temp) {
		case 10:
			RightPaw.setTexture(texture[4]);
			break;
		case 11:
			RightPaw.setTexture(texture[5]);
			break;
		case 01:
			RightPaw.setTexture(texture[6]);
			break;
		case 0:
			RightPaw.setTexture(texture[8]);
		}
		window.clear();
		window.draw(Cat);
		window.draw(LeftPaw);
		window.draw(RightPaw);
		window.display();
	}
}

void DrawHand(int state) {
	switch (state) {
	case 10:
		LeftPaw.setTexture(texture[1]);
		break;
	case 11:
		LeftPaw.setTexture(texture[2]);
		break;
	case 01:
		LeftPaw.setTexture(texture[3]);
		break;
	case 00:
		LeftPaw.setTexture(texture[7]);
		break;
	case 32:
		RightPaw.setTexture(texture[4]);
		break;
	case 33:
		RightPaw.setTexture(texture[5]);
		break;
	case 23:
		RightPaw.setTexture(texture[6]);
		break;
	case 22:
		RightPaw.setTexture(texture[8]);
		break;
	}
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode,
	WPARAM wParam,
	LPARAM lParam)

{
	if (nCode == HC_ACTION)
	{
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
		switch (wParam)
		{
		case WM_KEYDOWN:
			for (int i = 0; i < 4; i++) {
				if (p->vkCode == key[i]) {
					bool changed = FALSE;
					keystroke[i] = TRUE;
					if (last_keystroke[i] != keystroke[i]) {
						changed = TRUE;
						last_keystroke[i] = TRUE;
					}
					if (changed)
					{
						int state;
						if (i < 2)
							state = (keystroke[0] * 10) + keystroke[1];
						else
							state = ((keystroke[3] + 2) * 10) + keystroke[2] + 2;
						DrawHand(state);
					}
					break;
				}
			}
			break;
		case WM_KEYUP:
			for (int i = 0; i < 4; i++) {
				if (p->vkCode == key[i]) {
					bool changed = FALSE;
					keystroke[i] = FALSE;
					if (last_keystroke[i] != keystroke[i]) {
						changed = TRUE;
						last_keystroke[i] = FALSE;
					}
					if (changed)
					{
						int state;
						if (i < 2)
							state = (keystroke[0] * 10) + keystroke[1];
						else
							state = ((keystroke[3] + 2) * 10) + keystroke[2] + 2;
						DrawHand(state);
					}
					break;
				}
			}
			break;
		}
	}
	return(CallNextHookEx(NULL, nCode, wParam, lParam));
}


void KeyboardListener() {
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL,
		LowLevelKeyboardProc,
		NULL,
		NULL);

	MSG message{ 0 };
	//Waits for user input
	while (GetMessage(&message, NULL, 0, 0) != 0)
	{
	}
	// Unhook
	UnhookWindowsHookEx(keyboardHook);
	//std::cout << "dead\n";
}


int main()
{
	std::ifstream config;
	std::string string;


	// lazy config lol
	config.open("config.txt");
	if (!config.fail())
	{
		int c = 0;
		while (std::getline(config, string)) {
			for (std::string::size_type i = 0; i < string.size(); i++)
			{
				if (string[i] == '=') {
					key[c] = VkKeyScan(string[i+1]);
					c++;
				}
			}
		}
		config.close();
	}

	if (!texture[0].loadFromFile("base.png"))
		return EXIT_FAILURE;
	if (!texture[1].loadFromFile("base_1000.png"))
		return EXIT_FAILURE;
	if (!texture[2].loadFromFile("base_1100.png"))
		return EXIT_FAILURE;
	if (!texture[3].loadFromFile("base_0100.png"))
		return EXIT_FAILURE;
	if (!texture[4].loadFromFile("base_0001.png"))
		return EXIT_FAILURE;
	if (!texture[5].loadFromFile("base_0011.png"))
		return EXIT_FAILURE;
	if (!texture[6].loadFromFile("base_0010.png"))
		return EXIT_FAILURE;
	if (!texture[7].loadFromFile("base_left.png"))
		return EXIT_FAILURE;
	if (!texture[8].loadFromFile("base_right.png"))
		return EXIT_FAILURE;
	Cat.setTexture(texture[0]);
	LeftPaw.setTexture(texture[7]);
	RightPaw.setTexture(texture[8]);
	window.clear();
	window.draw(Cat);
	window.draw(LeftPaw);
	window.draw(RightPaw);
	window.display();
	window.setFramerateLimit(60);


	// Set window to topmost
	// Uncomment the code and rebuild if you want the window to stay on top
	//SetWindowPos(handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	// Install the low-level keyboard hook
	/*keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL,
		LowLevelKeyboardProc,
		NULL,
		NULL);*/

	std::thread hook(KeyboardListener);
	hook.detach();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				UnhookWindowsHookEx(keyboardHook);
				window.close();
				break;
			case (sf::Event::Resized || sf::Event::GainedFocus || sf::Event::LostFocus):

				break;
			}
		}
		window.clear();
		window.draw(Cat);
		window.draw(LeftPaw);
		window.draw(RightPaw);
		window.display();
		sf::sleep(sf::milliseconds(15));
	}
	return EXIT_SUCCESS;
}
