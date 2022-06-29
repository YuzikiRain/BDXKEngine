#include "Input.h"

Window* Input::window{};
float Input::mouseScrollDelta;
Vector2 Input::mousePosition;
bool Input::lastMouseButtonState[3];
bool Input::mouseButtonState[3];
bool Input::lastKeyboardState[256];
bool Input::keyboardState[256];

bool Input::GetMouseButtonDown(int mouseButtonIndex)
{
	if (lastMouseButtonState[mouseButtonIndex] == false &&
		mouseButtonState[mouseButtonIndex] == true)
		return true;
	return false;
}
bool Input::GetMouseButton(int mouseButtonIndex)
{
	if (lastMouseButtonState[mouseButtonIndex] == true &&
		mouseButtonState[mouseButtonIndex] == true)
		return true;
	return false;
}
bool Input::GetMouseButtonUp(int mouseButtonIndex)
{
	if (lastMouseButtonState[mouseButtonIndex] == true &&
		mouseButtonState[mouseButtonIndex] == false)
		return true;
	return false;
}

bool Input::GetKeyDown(KeyCode keyCode)
{
	if (lastKeyboardState[(int)keyCode] == false &&
		keyboardState[(int)keyCode] == true)
		return true;
	return false;
}
bool Input::GetKey(KeyCode keyCode)
{
	if (lastKeyboardState[(int)keyCode] == true &&
		keyboardState[(int)keyCode] == true)
		return true;
	return false;
}
bool Input::GetKeyUp(KeyCode keyCode)
{
	if (lastKeyboardState[(int)keyCode] == true &&
		keyboardState[(int)keyCode] == false)
		return true;
	return false;
}

Input* Input::Initialize(Window* window)
{
	Input::window = window;
	mousePosition = window->GetCursorLocalPosition();
	window->AddMessageListener(OnWindowMessage);
	return new Input{};
}

void Input::FlushState()
{
	mouseScrollDelta *= 0.7f;

	for (int i = 0; i < 3; i++)
	{
		lastMouseButtonState[i] = mouseButtonState[i];
	}
	for (int i = 0; i < 256; i++)
	{
		lastKeyboardState[i] = keyboardState[i];
	}
}
void Input::OnWindowMessage(Window* window, UINT messageSign, WPARAM wparameter, LPARAM lparameter)
{
	switch (messageSign)
	{
	case WM_PAINT:
	{
		Input::FlushState();
		break;
	}
#pragma region 鼠标事件
	case WM_MOUSEMOVE:
	{
		Input::mousePosition.x = (float)(lparameter << 48 >> 48);
		Input::mousePosition.y = (float)(lparameter >> 16);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		Input::mouseScrollDelta += GET_WHEEL_DELTA_WPARAM(wparameter) / 1000.0f;
		break;
	}
	case WM_LBUTTONDOWN:
	{
		Input::mouseButtonState[0] = true;
		break;
	}
	case WM_LBUTTONUP:
	{
		Input::mouseButtonState[0] = false;
		break;
	}
	case WM_RBUTTONDOWN:
	{
		Input::mouseButtonState[1] = true;
		break;
	}
	case WM_RBUTTONUP:
	{
		Input::mouseButtonState[1] = false;
		break;
	}
	case WM_MBUTTONDOWN:
	{
		Input::mouseButtonState[2] = true;
		break;
	}
	case WM_MBUTTONUP:
	{
		Input::mouseButtonState[2] = false;
		break;
	}
#pragma endregion
#pragma region 键盘事件
	case WM_KEYDOWN:
	{
		Input::keyboardState[wparameter] = true;
		break;
	}
	case WM_KEYUP:
	{
		Input::keyboardState[wparameter] = false;
		break;
	}
#pragma endregion
	}
}
