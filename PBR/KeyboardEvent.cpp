#include "KeyboardEvent.h"

KeyboardEvent::KeyboardEvent() : m_Type(EventType::Invalid), m_Key(0u) {}
KeyboardEvent::KeyboardEvent(EventType type, const unsigned char key) : m_Type(type), m_Key(key) {}

KeyboardEvent::~KeyboardEvent() {

}

bool KeyboardEvent::IsPress() const {
	return m_Type == EventType::Press;
}

bool KeyboardEvent::IsRelease() const {

	return m_Type == EventType::Release;
}

bool KeyboardEvent::IsValid() const {
	return m_Type != EventType::Invalid;
}

unsigned char KeyboardEvent::GetKeyCode() const {
	return m_Key;
}

