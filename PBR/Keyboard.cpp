#include "Keyboard.h"

Keyboard::Keyboard() {
	for (int i = 0; i < 256; ++i) {
		m_KeyStates[i] = false;
	}
}

bool Keyboard::KeyIsPressed(const unsigned char keycode) {
	return m_KeyStates[keycode];
}

bool Keyboard::KeyBufferIsEmpty() {
	return m_KeyBuffer.empty();
}

bool Keyboard::CharBufferIsEmpty() {
	return m_CharBuffer.empty();
}

KeyboardEvent Keyboard::ReadKey() {
	if(m_KeyBuffer.empty()) {
		return KeyboardEvent();
	}
	else {
		KeyboardEvent e = m_KeyBuffer.front();
		m_KeyBuffer.pop();
		return e;
	}
}

unsigned char Keyboard::ReadChar() {
	if (m_CharBuffer.empty()) {
		return 0u;
	}
	else {
		unsigned char e = m_CharBuffer.front();
		m_CharBuffer.pop();
		return e;
	}
}

void Keyboard::OnKeyPressed(const unsigned char key) {
	m_KeyStates[key] = true;
	m_KeyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void Keyboard::OnChar(const unsigned char key) {
	m_CharBuffer.push(key);
}

void Keyboard::OnKeyReleased(const unsigned char key) {
	m_KeyStates[key] = true;
	m_KeyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void Keyboard::EnableAutoRepeatChars() { m_AutoRepeatChars = true; }
void Keyboard::EnableAutoRepeatKeys() { m_AutoRepeatKeys = true; }
void Keyboard::DisableAutoRepeatChars() { m_AutoRepeatChars = false; }
void Keyboard::DisableAutoRepeatKeys() { m_AutoRepeatKeys = false; }
bool Keyboard::IsCharAutoRepeat() { return m_AutoRepeatChars; }
bool Keyboard::IsKeyAutoRepeat() { return m_AutoRepeatKeys; }








