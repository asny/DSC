
#pragma once

#include <vector>
#include "key.h"

namespace is_mesh
{
    
    template<typename key_type>
    class SimplexSet : public std::vector<key_type>
    {
    public:
        using std::vector<key_type>::vector;
        
        bool contains(const key_type& k) const
        {
            return std::find(this->begin(), this->end(), k) != this->end();
        }
        
        SimplexSet<key_type>& operator+=(const SimplexSet<key_type>& set)
        {
            for (auto &k : set) {
                *this += k;
            }
            return *this;
        }
        
        SimplexSet<key_type>& operator+=(const key_type& key)
        {
            if(!contains(key))
            {
                this->push_back(key);
            }
            return *this;
        }
        
        SimplexSet<key_type>& operator+=(key_type&& key)
        {
            if(!contains(key))
            {
                this->push_back(std::move(key));
            }
            return *this;
        }
        
        SimplexSet<key_type>& operator-=(const SimplexSet<key_type>& set)
        {
            for (auto &k : set) {
                *this -= k;
            }
            return *this;
        }
        
        SimplexSet<key_type>& operator-=(const key_type& key)
        {
            auto iter = std::find(this->begin(), this->end(), key);
            if(iter != this->end())
            {
                this->erase(iter);
            }
            return *this;
        }
    };
    
    template<typename key_type>
    bool operator==(const SimplexSet<key_type>& A, const SimplexSet<key_type>& B)
    {
        if(A.size() == B.size())
        {
            for (auto k : A)
            {
                if(!B.contains(k))
                {
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    
    /**
     *  Returns the union of the two sets A and B.
     */
    template<typename key_type>
    SimplexSet<key_type> operator+(const SimplexSet<key_type>& A, const SimplexSet<key_type>& B)
    {
        SimplexSet<key_type> C;
        return (C += A) += B;
    }
    
    /**
     *  Returns the union of the two sets A and B.
     */
    template<typename key_type>
    SimplexSet<key_type>&& operator+(SimplexSet<key_type>&& A, const SimplexSet<key_type>& B)
    {
        return std::move(A += B);
    }
    
    /**
     *  Returns the union of the two sets A and B.
     */
    template<typename key_type>
    SimplexSet<key_type>&& operator+(SimplexSet<key_type>&& A, SimplexSet<key_type>&& B)
    {
        return std::move(A) + B;
    }

    /**
     *  Returns the union of the two sets A and B.
     */
    template<typename key_type>
    SimplexSet<key_type>&& operator+(const SimplexSet<key_type>& A, SimplexSet<key_type>&& B)
    {
        return std::move(B) + A;
    }
    
    /**
     *  Returns set A without the elements in set B.
     */
    template<typename key_type>
    SimplexSet<key_type> operator-(const SimplexSet<key_type>& A, const SimplexSet<key_type>& B)
    {
        SimplexSet<key_type> C;
        return (C += A) -= B;
    }
    
    /**
     *  Returns set A without the elements in set B.
     */
    template<typename key_type>
    SimplexSet<key_type>&& operator-(SimplexSet<key_type>&& A, const SimplexSet<key_type>& B)
    {
        return std::move(A -= B);
    }
    
    /**
     *  Returns set A without the element key.
     */
    template<typename key_type>
    SimplexSet<key_type> operator-(const SimplexSet<key_type>& A, const key_type& key)
    {
        SimplexSet<key_type> B = {key};
        return A - B;
    }
    
    /**
     *  Returns set A without the element key.
     */
    template<typename key_type>
    SimplexSet<key_type>&& operator-(SimplexSet<key_type>&& A, const key_type& key)
    {
        return std::move(A -= key);
    }
    
    /**
     *  Returns the intersection of sets A and B.
     */
    template<typename key_type>
    SimplexSet<key_type> operator&(const SimplexSet<key_type>& A, const SimplexSet<key_type>& B)
    {
        SimplexSet<key_type> C = A;
        return C - (A - B);
    }
    
    /**
     *  Returns the intersection of sets A and B.
     */
    template<typename key_type>
    SimplexSet<key_type>&& operator&(SimplexSet<key_type>&& A, const SimplexSet<key_type>& B)
    {
        return std::move(std::move(A) - (A - B));
    }
    
    inline void simplex_set_test()
    {
        std::cout << "Testing simplex set class: ";
        SimplexSet<int> A = {1,3,9,4};
        SimplexSet<int> B = {1,7,5,3,10};
        
        SimplexSet<int> U = {1,3,9,4,7,5,10};
        assert((A+B) == U);
        
        SimplexSet<int> C = {9,4};
        assert((A-B) == C);
        
        SimplexSet<int> I = {1,3};
        assert((A&B) == I);
        
        A -= 3;
        A += 9;
        A += 11;
        SimplexSet<int> E = {1,9,4,11};
        assert(A == E);
        
        std::cout << "PASSED" << std::endl;
    }
}
