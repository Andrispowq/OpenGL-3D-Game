#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include "Window.h"
#include "engine/core/math/Math.h"

#define InputInstance Input::GetInstance()

class Input
{
public:
	static Input& GetInstance();

	bool Init(const Window& window) const;
	bool Update();

	bool IsKeyPushed(const int& key) const { return std::find(pushedKeys.begin(), pushedKeys.end(), key) != pushedKeys.end(); }
	bool IsKeyHeld(const int& key) const { return std::find(keysHolding.begin(), keysHolding.end(), key) != keysHolding.end(); }
	bool IsKeyReleased(const int& key) const { return std::find(releasedKeys.begin(), releasedKeys.end(), key) != releasedKeys.end(); }

	bool IsButtonPushed(const int& button) const { return std::find(pushedButtons.begin(), pushedButtons.end(), button) != pushedButtons.end(); }
	bool IsButtonHeld(const int& button) const { return std::find(buttonsHolding.begin(), buttonsHolding.end(), button) != buttonsHolding.end(); }
	bool IsButtonReleased(const int& button) const { return std::find(releasedButtons.begin(), releasedButtons.end(), button) != releasedButtons.end(); }

	bool IsPause() const { return pause; }
	void SetPause(bool pause) { Input::pause = pause; }

	Vector2f GetCursorPosition() const { return cursorPosition; }
	void SetCursorPosition(const Vector2f& cursorPosition) { Input::cursorPosition = cursorPosition; }

	Vector2f GetLockedCursorPosition() const { return lockedCursorPosition; }
	void SetLockedCursorPosition(const Vector2f& lockedCursorPosition) { Input::lockedCursorPosition = lockedCursorPosition; }

	Vector2f GetJoystickPosition() const { return joystickPosition; }
	void SetJoystickPosition(const Vector2f& joystickPosition) { Input::joystickPosition = joystickPosition; }

	float GetScrollOffset() const { return scrollOffset; }
	void SetScrollOffset(float scrollOffset) { Input::scrollOffset = scrollOffset; }

	inline std::vector<int> GetPushedKeys() const { return pushedKeys; }
	inline std::vector<int> GetKeysHolding() const { return keysHolding; }
	inline std::vector<int> GetReleasedKeys() const { return releasedKeys; }

	inline std::vector<int> GetPushedButtons() const { return pushedButtons; }
	inline std::vector<int> GetButtonsHolding() const { return buttonsHolding; }
	inline std::vector<int> GetReleasedButtons() const { return releasedButtons; }

	inline void SetPushedKeys(const std::vector<int>& pushedKeys) { this->pushedKeys = pushedKeys; }
	inline void SetKeysHolding(const std::vector<int>& keysHolding) { this->keysHolding = keysHolding; }
	inline void SetReleasedKeys(const std::vector<int>& releasedKeys) { this->releasedKeys = releasedKeys; }

	inline void SetPushedButtons(const std::vector<int>& pushedButtons) { this->pushedButtons = pushedButtons; }
	inline void SetButtonsHolding(const std::vector<int>& buttonsHolding) { this->buttonsHolding = buttonsHolding; }
	inline void SetReleasedButtons(const std::vector<int>& releasedButtons) { this->releasedButtons = releasedButtons; }
private:
	Input() {};
	virtual ~Input() {}
private:
	static Input instance;

	std::vector<int> pushedKeys;
	std::vector<int> keysHolding;
	std::vector<int> releasedKeys;

	std::vector<int> pushedButtons;
	std::vector<int> buttonsHolding;
	std::vector<int> releasedButtons;

	Vector2f cursorPosition;
	Vector2f lockedCursorPosition;
	Vector2f joystickPosition;
	float scrollOffset;
	
	bool pause;
};

#endif