#pragma once

#include "DirectX.hpp"

/// <summary>
/// Defines a smart COM pointer wrapper.
/// </summary>
template<typename T> class ComPtr
{
    template<typename U> friend class ComPtr;

private:
    T* _object;

    /// <summary>
    /// Adds a reference to this COM pointer wrapper.
    /// </summary>
    void AddRef();

    /// <summary>
    /// Releases this COM pointer wrapper.
    /// </summary>
    void Release();

public:
    /// <summary>
    /// Creates a new, empty COM pointer.
    /// </summary>
    ComPtr();

    /// <summary>
    /// Copies the given COM pointer.
    /// </summary>
    /// <param name="other">The COM pointer to copy.</param>
    ComPtr( const ComPtr<T>& other );

    /// <summary>
    /// Takes over the responsibilities of the given COM pointer.
    /// </summary>
    /// <param name="other">The COM pointer.</param>
    ComPtr( ComPtr<T>&& other );

    /// <summary>
    /// Destroys this COM pointer.
    /// </summary>
    ~ComPtr();

    /// <summary>
    /// Gets the underlying wrapped COM pointer.
    /// </summary>
    const T* Get() const;

    /// <summary>
    /// Gets the address of the underlying wrapped COM pointer.
    /// </summary>
    const T** GetAddress() const;

    /// <summary>
    /// Gets the underlying wrapped COM pointer.
    /// </summary>
    T* Get();

    /// <summary>
    /// Gets the address of the underlying wrapped COM pointer.
    /// </summary>
    T** GetAddress();

    /// <summary>
    /// Resets this COM pointer.
    /// </summary>
    void Reset();

    /// <summary>
    /// De-references this COM pointer.
    /// </summary>
    const T* operator->() const;

    /// <summary>
    /// De-references this COM pointer.
    /// </summary>
    const T* operator*() const;

    /// <summary>
    /// Implicitly converts this pointer into a Boolean value.
    /// </summary>
    operator bool() const;

    /// <summary>
    /// De-references this COM pointer.
    /// </summary>
    T* operator->();

    /// <summary>
    /// De-references this COM pointer.
    /// </summary>
    T* operator*();

    /// <summary>
    /// Copies the data from the given COM pointer.
    /// </summary>
    /// <param name="other">The other pointer.</param>
    ComPtr<T>& operator=( const ComPtr<T>& other );

    /// <summary>
    /// Takes over the responsibilities of the given COM pointer.
    /// </summary>
    /// <param name="other">The pointer.</param>
    ComPtr<T>& operator=( ComPtr<T>&& other );
};

#include "ComPtr.inl"
