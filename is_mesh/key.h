//
//  Deformabel Simplicial Complex (DSC) method
//  Copyright (C) 2013  Technical University of Denmark
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  See licence.txt for a copy of the GNU General Public License.

#pragma once

#include <iostream>

namespace is_mesh
{
    // Note: Non-polymorphic
    class Key
    {
    protected:
        unsigned int key;
        
        Key() noexcept : Key(static_cast<unsigned int>(-1))
        {
            
        }
        
        Key(unsigned int _key) noexcept : key(_key)
        {
            
        }
        
    public:
        bool is_valid() const noexcept
        {
            return key != static_cast<unsigned int>(-1);
        }
        
        //conversion to int
        operator unsigned int() const noexcept
        {
            return key;
        }
        
        friend inline bool operator==(Key    const & a, Key    const & b)   { return a.key == b.key; }
        friend inline bool operator==(Key          & a, Key          & b)   { return a.key == b.key; }
        friend inline bool operator==(unsigned int const & k, Key    const & b)   { return   k   == b.key; }
        friend inline bool operator==(Key    const & a, unsigned int const & k)   { return a.key ==   k;   }
        friend inline bool operator!=(Key    const & a, Key    const & b)   { return a.key != b.key; }
        friend inline bool operator!=(Key          & a, Key          & b)   { return a.key != b.key; }
        friend inline bool operator!=(unsigned int const & k, Key    const & b)   { return   k   != b.key; }
        friend inline bool operator!=(Key    const & a, unsigned int const & k)   { return a.key !=   k;   }
        friend inline bool operator< (Key    const & a, Key    const & b)   { return a.key <  b.key; }
        friend inline bool operator< (Key          & a, Key          & b)   { return a.key <  b.key; }
        friend inline bool operator< (unsigned int const & k, Key    const & b)   { return   k   <  b.key; }
        friend inline bool operator< (Key    const & a, unsigned int const & k)   { return a.key <    k;   }

        friend std::ostream& operator<< (std::ostream & os, Key const & a) { return (os << a.key); }
        friend std::istream& operator>> (std::istream & is, Key       & a) { return (is >> a.key); }
    };
    
    class NodeKey : public Key
    {
    public:
        NodeKey() noexcept : Key() {}
        NodeKey(unsigned int k) noexcept : Key(k) {}
    };
    
    class EdgeKey : public Key
    {
    public:
        EdgeKey() noexcept : Key() {}
        EdgeKey(unsigned int k) noexcept : Key(k) {}
    };
    
    class FaceKey : public Key
    {
    public:
        FaceKey() noexcept  : Key() {}
        FaceKey(unsigned int k) noexcept : Key(k) {}
    };
    
    class TetrahedronKey : public Key
    {
    public:
        TetrahedronKey() noexcept : Key() {}
        TetrahedronKey(unsigned int k) noexcept : Key(k) {}
    };
    
}
