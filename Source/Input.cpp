#include "Input.hpp"

using namespace DirectX;

std::unique_ptr<Keyboard>   Input::_keyboard;
Keyboard::State             Input::_lastKeys;
Keyboard::State             Input::_currKeys;
std::unique_ptr<Mouse>      Input::_mouse;
Mouse::State                Input::_lastMouse;
Mouse::State                Input::_currMouse;

// Check if mouse button is down
bool Input::IsButtonDown( MouseButton button )
{
    switch ( button )
    {
        case MouseButton::Left:   return _currMouse.leftButton;
        case MouseButton::Middle: return _currMouse.middleButton;
        case MouseButton::Right:  return _currMouse.rightButton;
    }
    return false;
}

// Check if mouse button is up
bool Input::IsButtonUp( MouseButton button )
{
    switch ( button )
    {
        case MouseButton::Left:   return !_currMouse.leftButton;
        case MouseButton::Middle: return !_currMouse.middleButton;
        case MouseButton::Right:  return !_currMouse.rightButton;
    }
    return false;
}

// Check if key is down
bool Input::IsKeyDown( Key key )
{
    return _currKeys.IsKeyDown( static_cast<Keyboard::Keys>( key ) );
}

// Check if key is up
bool Input::IsKeyUp( Key key )
{
    return _currKeys.IsKeyUp( static_cast<Keyboard::Keys>( key ) );
}

// Get mouse position
XMFLOAT2 Input::GetMousePosition()
{
    return XMFLOAT2(
        static_cast<float>( _currMouse.x ),
        static_cast<float>( _currMouse.y )
    );
}

// Get delta mouse position
XMFLOAT2 Input::GetDeltaMousePosition()
{
    return XMFLOAT2(
        static_cast<float>( _currMouse.x - _lastMouse.x ),
        static_cast<float>( _currMouse.y - _lastMouse.y )
    );
}

// Initializes the input methods
bool Input::Initialize( HWND window )
{
    _keyboard = std::make_unique<Keyboard>();
    if ( !_keyboard )
    {
        return false;
    }

    _mouse = std::make_unique<Mouse>();
    if ( !_mouse )
    {
        return false;
    }

    //_mouse->SetMode( Mouse::MODE_RELATIVE ); // Memory exception??
    _mouse->SetWindow( window );

    return true;
}

// Processes an input
void Input::ProcessMessage( UINT msg, WPARAM wParam, LPARAM lParam )
{
    _mouse->ProcessMessage( msg, wParam, lParam );
    _keyboard->ProcessMessage( msg, wParam, lParam );
}

// Update input values
void Input::Update()
{
    _lastKeys = _currKeys;
    _currKeys = _keyboard->GetState();

    _lastMouse = _currMouse;
    _currMouse = _mouse->GetState();
}

// Check if mouse button was pressed
bool Input::WasButtonPressed( MouseButton button )
{
    switch ( button )
    {
        case MouseButton::Left:   return _currMouse.leftButton   && !_lastMouse.leftButton;
        case MouseButton::Middle: return _currMouse.middleButton && !_lastMouse.middleButton;
        case MouseButton::Right:  return _currMouse.rightButton  && !_lastMouse.rightButton;
    }
    return false;
}

// Check if mouse button was released
bool Input::WasButtonReleased( MouseButton button )
{
    switch ( button )
    {
        case MouseButton::Left:   return !_currMouse.leftButton   && _lastMouse.leftButton;
        case MouseButton::Middle: return !_currMouse.middleButton && _lastMouse.middleButton;
        case MouseButton::Right:  return !_currMouse.rightButton  && _lastMouse.rightButton;
    }
    return false;
}

// Check if key was pressed
bool Input::WasKeyPressed( Key key )
{
    return _currKeys.IsKeyDown( static_cast<Keyboard::Keys>( key ) )
        && _lastKeys.IsKeyUp  ( static_cast<Keyboard::Keys>( key ) );
}

// Check if key was released
bool Input::WasKeyReleased( Key key )
{
    return _currKeys.IsKeyUp  ( static_cast<Keyboard::Keys>( key ) )
        && _lastKeys.IsKeyDown( static_cast<Keyboard::Keys>( key ) );
}
