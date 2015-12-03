#pragma once

// Create a new, empty COM pointer
template<typename T> ComPtr<T>::ComPtr()
    : _object( nullptr )
{
}

// Copy another COM pointer
template<typename T> ComPtr<T>::ComPtr( const ComPtr<T>& other )
    : _object( other._object )
{
    AddRef();
}

// Take over the responsibilities of another COM pointer
template<typename T> ComPtr<T>::ComPtr( ComPtr<T>&& other )
    : _object( other._object )
{
    other._object = nullptr;
}

// Destroy this COM pointer
template<typename T> ComPtr<T>::~ComPtr()
{
    Release();
}

// Get the underlying COM pointer
template<typename T> const T* ComPtr<T>::Get() const
{
    return _object;
}

// Get the address of the underlying COM pointer
template<typename T> const T** ComPtr<T>::GetAddress() const
{
    return &_object;
}

// Add a reference to this COM pointer
template<typename T> void ComPtr<T>::AddRef()
{
    if ( _object )
    {
        _object->AddRef();
    }
}

// Get the underlying COM pointer
template<typename T> T* ComPtr<T>::Get()
{
    return _object;
}

// Get the address of the underlying COM pointer
template<typename T> T** ComPtr<T>::GetAddress()
{
    return &_object;
}

// Release a reference to this COM pointer
template<typename T> void ComPtr<T>::Release()
{
    ReleaseMacro( _object );
}

// Resets this COM pointer
template<typename T> void ComPtr<T>::Reset()
{
    ReleaseMacro( _object );
}

// De-reference this COM pointer
template<typename T> const T* ComPtr<T>::operator->() const
{
    return _object;
}

// De-reference this COM pointer
template<typename T> const T* ComPtr<T>::operator*() const
{
    return _object;
}

// Implicitly convert to a Boolean
template<typename T> ComPtr<T>::operator bool() const
{
    return ( _object != nullptr );
}

// De-reference this COM pointer
template<typename T> T* ComPtr<T>::operator->()
{
    return _object;
}

// De-reference this COM pointer
template<typename T> T* ComPtr<T>::operator*()
{
    return _object;
}

// Copy another COM pointer
template<typename T> ComPtr<T>& ComPtr<T>::operator=( const ComPtr<T>& other )
{
    // If we're the same pointer, don't do anything
    if ( _object == other._object )
    {
        return *this;
    }

    Release();
    _object = other._object;
    AddRef();

    return *this;
}

// Take over the responsibilities of another COM pointer
template<typename T> ComPtr<T>& ComPtr<T>::operator=( ComPtr<T>&& other )
{
    // If we're the same pointer, don't do anything
    if ( _object == other._object )
    {
        return *this;
    }

    Release();
    _object = other._object;
    other._object = nullptr;

    return *this;
}
