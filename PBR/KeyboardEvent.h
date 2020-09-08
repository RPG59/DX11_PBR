#pragma once

class KeyboardEvent {
public:
	enum EventType
	{
		Press,
		Release,
		Invalid
	};
	KeyboardEvent();
	KeyboardEvent(EventType, const unsigned char);
	~KeyboardEvent();
	bool IsPress() const;
	bool IsRelease() const;
	bool IsValid() const;
	unsigned char GetKeyCode() const;

private:
	EventType m_Type;
	unsigned char m_Key;
};
