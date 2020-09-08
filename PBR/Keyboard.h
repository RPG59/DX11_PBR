#pragma once

#include <queue>

#include "KeyboardEvent.h"

class Keyboard {
public:
	Keyboard();
	bool KeyIsPressed(const unsigned char keycode);
	bool KeyBufferIsEmpty();
	bool CharBufferIsEmpty();
	KeyboardEvent ReadKey();
	unsigned char ReadChar();
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char  key);
	void EnableAutoRepeatKeys();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatKeys();
	void DisableAutoRepeatChars();
	bool IsKeyAutoRepeat();
	bool IsCharAutoRepeat();
private:
	bool m_AutoRepeatKeys = false;
	bool m_AutoRepeatChars = false;
	bool m_KeyStates[256];
	std::queue<KeyboardEvent> m_KeyBuffer;
	std::queue<unsigned char> m_CharBuffer;
};
