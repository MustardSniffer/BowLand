#pragma once

#include "DirectX.hpp"
#include <DirectXTK\Keyboard.h>
#include <DirectXTK\Mouse.h>

/// <summary>
/// An enumeration of keyboard keys.
/// </summary>
enum class Key
{
    None                = DirectX::Keyboard::None,
    Back                = DirectX::Keyboard::Back,
    Tab                 = DirectX::Keyboard::Tab,
    Enter               = DirectX::Keyboard::Enter,
    Pause               = DirectX::Keyboard::Pause,
    CapsLock            = DirectX::Keyboard::CapsLock,
    Escape              = DirectX::Keyboard::Escape,
    Space               = DirectX::Keyboard::Space,
    PageUp              = DirectX::Keyboard::PageUp,
    PageDown            = DirectX::Keyboard::PageDown,
    End                 = DirectX::Keyboard::End,
    Home                = DirectX::Keyboard::Home,
    Left                = DirectX::Keyboard::Left,
    Up                  = DirectX::Keyboard::Up,
    Right               = DirectX::Keyboard::Right,
    Down                = DirectX::Keyboard::Down,
    Select              = DirectX::Keyboard::Select,
    PrintScreen         = DirectX::Keyboard::PrintScreen,
    Insert              = DirectX::Keyboard::Insert,
    Delete              = DirectX::Keyboard::Delete,
    D0                  = DirectX::Keyboard::D0,
    D1                  = DirectX::Keyboard::D1,
    D2                  = DirectX::Keyboard::D2,
    D3                  = DirectX::Keyboard::D3,
    D4                  = DirectX::Keyboard::D4,
    D5                  = DirectX::Keyboard::D5,
    D6                  = DirectX::Keyboard::D6,
    D7                  = DirectX::Keyboard::D7,
    D8                  = DirectX::Keyboard::D8,
    D9                  = DirectX::Keyboard::D9,
    A                   = DirectX::Keyboard::A,
    B                   = DirectX::Keyboard::B,
    C                   = DirectX::Keyboard::C,
    D                   = DirectX::Keyboard::D,
    E                   = DirectX::Keyboard::E,
    F                   = DirectX::Keyboard::F,
    G                   = DirectX::Keyboard::G,
    H                   = DirectX::Keyboard::H,
    I                   = DirectX::Keyboard::I,
    J                   = DirectX::Keyboard::J,
    K                   = DirectX::Keyboard::K,
    L                   = DirectX::Keyboard::L,
    M                   = DirectX::Keyboard::M,
    N                   = DirectX::Keyboard::N,
    O                   = DirectX::Keyboard::O,
    P                   = DirectX::Keyboard::P,
    Q                   = DirectX::Keyboard::Q,
    R                   = DirectX::Keyboard::R,
    S                   = DirectX::Keyboard::S,
    T                   = DirectX::Keyboard::T,
    U                   = DirectX::Keyboard::U,
    V                   = DirectX::Keyboard::V,
    W                   = DirectX::Keyboard::W,
    X                   = DirectX::Keyboard::X,
    Y                   = DirectX::Keyboard::Y,
    Z                   = DirectX::Keyboard::Z,
    LeftWindows         = DirectX::Keyboard::LeftWindows,
    RightWindows        = DirectX::Keyboard::RightWindows,
    NumPad0             = DirectX::Keyboard::NumPad0,
    NumPad1             = DirectX::Keyboard::NumPad1,
    NumPad2             = DirectX::Keyboard::NumPad2,
    NumPad3             = DirectX::Keyboard::NumPad3,
    NumPad4             = DirectX::Keyboard::NumPad4,
    NumPad5             = DirectX::Keyboard::NumPad5,
    NumPad6             = DirectX::Keyboard::NumPad6,
    NumPad7             = DirectX::Keyboard::NumPad7,
    NumPad8             = DirectX::Keyboard::NumPad8,
    NumPad9             = DirectX::Keyboard::NumPad9,
    Multiply            = DirectX::Keyboard::Multiply,
    Add                 = DirectX::Keyboard::Add,
    Separator           = DirectX::Keyboard::Separator,
    Subtract            = DirectX::Keyboard::Subtract,
    Decimal             = DirectX::Keyboard::Decimal,
    Divide              = DirectX::Keyboard::Divide,
    F1                  = DirectX::Keyboard::F1,
    F2                  = DirectX::Keyboard::F2,
    F3                  = DirectX::Keyboard::F3,
    F4                  = DirectX::Keyboard::F4,
    F5                  = DirectX::Keyboard::F5,
    F6                  = DirectX::Keyboard::F6,
    F7                  = DirectX::Keyboard::F7,
    F8                  = DirectX::Keyboard::F8,
    F9                  = DirectX::Keyboard::F9,
    F10                 = DirectX::Keyboard::F10,
    F11                 = DirectX::Keyboard::F11,
    F12                 = DirectX::Keyboard::F12,
    F13                 = DirectX::Keyboard::F13,
    F14                 = DirectX::Keyboard::F14,
    F15                 = DirectX::Keyboard::F15,
    F16                 = DirectX::Keyboard::F16,
    F17                 = DirectX::Keyboard::F17,
    F18                 = DirectX::Keyboard::F18,
    F19                 = DirectX::Keyboard::F19,
    F20                 = DirectX::Keyboard::F20,
    F21                 = DirectX::Keyboard::F21,
    F22                 = DirectX::Keyboard::F22,
    F23                 = DirectX::Keyboard::F23,
    F24                 = DirectX::Keyboard::F24,
    LeftShift           = DirectX::Keyboard::LeftShift,
    RightShift          = DirectX::Keyboard::RightShift,
    LeftControl         = DirectX::Keyboard::LeftControl,
    RightControl        = DirectX::Keyboard::RightControl,
    LeftAlt             = DirectX::Keyboard::LeftAlt,
    RightAlt            = DirectX::Keyboard::RightAlt,
    BrowserBack         = DirectX::Keyboard::BrowserBack,
    BrowserForward      = DirectX::Keyboard::BrowserForward,
    BrowserRefresh      = DirectX::Keyboard::BrowserRefresh,
    BrowserStop         = DirectX::Keyboard::BrowserStop,
    BrowserSearch       = DirectX::Keyboard::BrowserSearch,
    BrowserFavorites    = DirectX::Keyboard::BrowserFavorites,
    BrowserHome         = DirectX::Keyboard::BrowserHome,
    VolumeMute          = DirectX::Keyboard::VolumeMute,
    VolumeDown          = DirectX::Keyboard::VolumeDown,
    VolumeUp            = DirectX::Keyboard::VolumeUp,
    MediaNextTrack      = DirectX::Keyboard::MediaNextTrack,
    MediaPreviousTrack  = DirectX::Keyboard::MediaPreviousTrack,
    MediaStop           = DirectX::Keyboard::MediaStop,
    MediaPlayPause      = DirectX::Keyboard::MediaPlayPause,
    OemSemicolon        = DirectX::Keyboard::OemSemicolon,
    OemPlus             = DirectX::Keyboard::OemPlus,
    OemComma            = DirectX::Keyboard::OemComma,
    OemMinus            = DirectX::Keyboard::OemMinus,
    OemPeriod           = DirectX::Keyboard::OemPeriod,
    OemQuestion         = DirectX::Keyboard::OemQuestion,
    OemTilde            = DirectX::Keyboard::OemTilde,
    OemOpenBrackets     = DirectX::Keyboard::OemOpenBrackets,
    OemPipe             = DirectX::Keyboard::OemPipe,
    OemCloseBrackets    = DirectX::Keyboard::OemCloseBrackets,
    OemQuotes           = DirectX::Keyboard::OemQuotes,
    Oem8                = DirectX::Keyboard::Oem8,
    OemBackslash        = DirectX::Keyboard::OemBackslash,
};

/// <summary>
/// An enumeration of mouse buttons.
/// </summary>
enum class MouseButton
{
    Left,
    Middle,
    Right
};

/// <summary>
/// Defines a static input class, similar to Unity's.
/// </summary>
class Input
{
    friend class DirectXGameCore;

    static std::unique_ptr<DirectX::Keyboard> _keyboard;
    static DirectX::Keyboard::State _lastKeys;
    static DirectX::Keyboard::State _currKeys;
    static std::unique_ptr<DirectX::Mouse> _mouse;
    static DirectX::Mouse::State _lastMouse;
    static DirectX::Mouse::State _currMouse;

private:
    /// <summary>
    /// Initializes the input data.
    /// </summary>
    static bool Initialize( HWND window );

    // Process an input message
    static void ProcessMessage( UINT msg, WPARAM wParam, LPARAM lParam );

    /// <summary>
    /// Updates all input information.
    /// </summary>
    static void Update();

    // Hide all instance methods
    Input() = delete;
    ~Input() = delete;
    Input( const Input& ) = delete;
    Input( Input&& ) = delete;
    Input& operator=( const Input& ) = delete;
    Input& operator=( Input&& ) = delete;

public:
    /// <summary>
    /// Checks to see if they given mouse button is down.
    /// </summary>
    /// <param name="button">The mouse button.</param>
    static bool IsButtonDown( MouseButton button );

    /// <summary>
    /// Checks to see if they given mouse button is up.
    /// </summary>
    /// <param name="button">The mouse button.</param>
    static bool IsButtonUp( MouseButton button );

    /// <summary>
    /// Checks to see if they given key is down.
    /// </summary>
    /// <param name="key">The key.</param>
    static bool IsKeyDown( Key key );

    /// <summary>
    /// Checks to see if they given key is up.
    /// </summary>
    /// <param name="key">The key.</param>
    static bool IsKeyUp( Key key );

    /// <summary>
    /// Gets the mouse's current position.
    /// </summary>
    static DirectX::XMFLOAT2 GetMousePosition();

    /// <summary>
    /// Gets the mouse's change in position since the last frame.
    /// </summary>
    static DirectX::XMFLOAT2 GetDeltaMousePosition();

    /// <summary>
    /// Checks to see if they given mouse button was pressed.
    /// </summary>
    /// <param name="button">The mouse button.</param>
    static bool WasButtonPressed( MouseButton button );

    /// <summary>
    /// Checks to see if they given mouse button was released.
    /// </summary>
    /// <param name="button">The mouse button.</param>
    static bool WasButtonReleased( MouseButton button );

    /// <summary>
    /// Checks to see if they given key was pressed.
    /// </summary>
    /// <param name="key">The key.</param>
    static bool WasKeyPressed( Key key );

    /// <summary>
    /// Checks to see if they given key was pressed.
    /// </summary>
    /// <param name="key">The key.</param>
    static bool WasKeyReleased( Key key );
};
