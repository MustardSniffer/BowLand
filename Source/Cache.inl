#pragma once

// Adds the given item to this cache.
template<typename T> bool Cache<T>::Add( T& item )
{
    auto search = _itemsCache.find( item );
    if ( search != _itemsCache.end() )
    {
        return false;
    }

    _itemsCache[ item ] = _items.size();
    _items.push_back( item );

    return true;
}

// Gets the iterator at the beginning of this cache
template<typename T> typename Cache<T>::ConstIterator Cache<T>::Begin() const
{
    return _items.begin();
}

// Gets the iterator at the beginning of this cache
template<typename T> typename Cache<T>::Iterator Cache<T>::Begin()
{
    return _items.begin();
}

// Gets the iterator at the end of this cache
template<typename T> typename Cache<T>::ConstIterator Cache<T>::End() const
{
    return _items.end();
}

// Gets the iterator at the end of this cache
template<typename T> typename Cache<T>::Iterator Cache<T>::End()
{
    return _items.end();
}

// Removes the given item from this cache
template<typename T> bool Cache<T>::Remove( T& item )
{
    // Ensure we contain the item
    auto search = _itemsCache.find( item );
    if ( search == _itemsCache.end() )
    {
        return false;
    }

    // Remove the item
    size_t index = search->second;
    _items.erase( _items.begin() + index );

    // Update the indices
    _itemsCache.erase( item );
    for ( size_t index = 0; index < _items.size(); ++index )
    {
        _itemsCache[ _items[ index ] ] = index;
    }

    return true;
}

// Gets the item at the given index
template<typename T> const T& Cache<T>::operator[]( size_t index ) const
{
    return _items[ index ];
}

// Gets the item at the given index
template<typename T> T& Cache<T>::operator[]( size_t index )
{
    return _items[ index ];
}
