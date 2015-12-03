#pragma once

#include <unordered_map>
#include <vector>

/// <summary>
/// Defines a cache class.
/// </summary>
template<typename T> class Cache
{
public:
    /// <summary>
    /// The map type used by this cache.
    /// </summary>
    using Map = std::unordered_map<T, size_t>;

    /// <summary>
    /// The list type used by this cache.
    /// </summary>
    using List = std::vector<T>;

    /// <summary>
    /// The const iterator type used by this cache.
    /// </summary>
    using ConstIterator = typename std::vector<T>::const_iterator;

    /// <summary>
    /// The iterator type used by this cache.
    /// </summary>
    using Iterator = typename std::vector<T>::iterator;

private:
    Map  _itemsCache;
    List _items;

public:
    /// <summary>
    /// Creates a new cache.
    /// </summary>
    Cache() = default;

    /// <summary>
    /// Destroys this cache.
    /// </summary>
    ~Cache() = default;

    /// <summary>
    /// Adds the given item to this cache.
    /// </summary>
    bool Add( T& item );

    /// <summary>
    /// Gets the iterator at the beginning of this cache.
    /// </summary>
    ConstIterator Begin() const;

    /// <summary>
    /// Gets the iterator at the beginning of this cache.
    /// </summary>
    Iterator Begin();

    /// <summary>
    /// Gets the iterator at the end of this cache.
    /// </summary>
    ConstIterator End() const;

    /// <summary>
    /// Gets the iterator at the end of this cache.
    /// </summary>
    Iterator End();

    /// <summary>
    /// Removes the given item from this cache.
    /// </summary>
    bool Remove( T& item );

    /// <summary>
    /// Gets the item at the given index.
    /// </summary>
    /// <param name="index">The index.</param>
    const T& operator[]( size_t index ) const;

    /// <summary>
    /// Gets the item at the given index.
    /// </summary>
    /// <param name="index">The index.</param>
    T& operator[]( size_t index );
};

#include "Cache.inl"
