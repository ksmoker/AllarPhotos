// -*- mode: c++; c-basic-offset: 4 -*-
/*
 * This file is part of the KDE libraries
 *
 * Copyright (C) 2005, 2006 Apple Computer, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef WTF_HashMap_h
#define WTF_HashMap_h

#include "HashTable.h"
#include "Vector.h"

namespace WTF {

    template<typename PairType> struct PairFirstExtractor;

    template<typename KeyArg, typename MappedArg, typename HashArg = typename DefaultHash<KeyArg>::Hash,
        typename KeyTraitsArg = HashTraits<KeyArg>, typename MappedTraitsArg = HashTraits<MappedArg> >
    class HashMap {
    private:
        typedef KeyTraitsArg KeyTraits;
        typedef MappedTraitsArg MappedTraits;
        typedef PairBaseHashTraits<KeyTraits, MappedTraits> ValueTraits;

    public:
        typedef typename KeyTraits::TraitType KeyType;
        typedef typename MappedTraits::TraitType MappedType;
        typedef typename ValueTraits::TraitType ValueType;

    private:
        typedef HashArg HashFunctions;

        typedef typename HashKeyStorageTraits<HashFunctions, KeyTraits>::Hash StorageHashFunctions;

        typedef typename HashKeyStorageTraits<HashFunctions, KeyTraits>::Traits KeyStorageTraits;
        typedef typename MappedTraits::StorageTraits MappedStorageTraits;
        typedef PairHashTraits<KeyStorageTraits, MappedStorageTraits> ValueStorageTraits;

        typedef typename KeyStorageTraits::TraitType KeyStorageType;
        typedef typename MappedStorageTraits::TraitType MappedStorageType;
        typedef typename ValueStorageTraits::TraitType ValueStorageType;

        typedef HashTable<KeyStorageType, ValueStorageType, PairFirstExtractor<ValueStorageType>,
            StorageHashFunctions, ValueStorageTraits, KeyStorageTraits> HashTableType;

    public:
        typedef HashTableIteratorAdapter<HashTableType, ValueType> iterator;
        typedef HashTableConstIteratorAdapter<HashTableType, ValueType> const_iterator;

        HashMap();
        HashMap(const HashMap&);
        HashMap& operator=(const HashMap&);
        ~HashMap();

        void swap(HashMap&);

        int size() const;
        int capacity() const;
        bool isEmpty() const;

        // iterators iterate over pairs of keys and values
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

        iterator find(const KeyType&);
        const_iterator find(const KeyType&) const;
        bool contains(const KeyType&) const;
        MappedType get(const KeyType&) const;

        // replaces value but not key if key is already present
        // return value is a pair of the iterator to the key location, 
        // and a boolean that's true if a new value was actually added
        pair<iterator, bool> set(const KeyType&, const MappedType&); 

        // does nothing if key is already present
        // return value is a pair of the iterator to the key location, 
        // and a boolean that's true if a new value was actually added
        pair<iterator, bool> add(const KeyType&, const MappedType&); 

        void remove(const KeyType&);
        void remove(iterator it);
        void clear();

    private:
        pair<iterator, bool> inlineAdd(const KeyType&, const MappedType&);
        void refAll();
        void derefAll();

        HashTableType m_impl;
    };

    template<typename PairType> struct PairFirstExtractor {
        static const typename PairType::first_type& extract(const PairType& p) { return p.first; }
    };

    template<bool canReplaceDeletedKey, typename ValueType, typename ValueTraits, typename ValueStorageTraits, typename HashFunctions>
    struct HashMapTranslator;

    template<typename ValueType, typename ValueTraits, typename ValueStorageTraits, typename HashFunctions>
    struct HashMapTranslator<true, ValueType, ValueTraits, ValueStorageTraits, HashFunctions> {
        typedef typename ValueType::first_type KeyType;
        typedef typename ValueType::second_type MappedType;
        typedef typename ValueStorageTraits::TraitType ValueStorageType;
        typedef typename ValueStorageTraits::FirstTraits KeyStorageTraits;
        typedef typename KeyStorageTraits::TraitType KeyStorageType;
        typedef typename ValueStorageTraits::SecondTraits MappedStorageTraits;
        typedef typename MappedStorageTraits::TraitType MappedStorageType;
        typedef typename ValueTraits::FirstTraits KeyTraits;
        typedef typename ValueTraits::SecondTraits MappedTraits;

        static unsigned hash(const KeyType& key) { return HashFunctions::hash(key); }
        static bool equal(const KeyStorageType& a, const KeyType& b) { return HashFunctions::equal(*(KeyType*)&a, b); }
        static void translate(ValueStorageType& location, const KeyType& key, const MappedType& mapped)
        {
            Assigner<KeyTraits::needsRef, KeyType, KeyStorageType, KeyTraits>::assign(key, location.first);
            Assigner<MappedTraits::needsRef, MappedType, MappedStorageType, MappedTraits>::assign(mapped, location.second);
        }
    };

    template<typename ValueType, typename ValueTraits, typename ValueStorageTraits, typename HashFunctions>
    struct HashMapTranslator<false, ValueType, ValueTraits, ValueStorageTraits, HashFunctions> {
        typedef typename ValueType::first_type KeyType;
        typedef typename ValueType::second_type MappedType;
        typedef typename ValueStorageTraits::TraitType ValueStorageType;
        typedef typename ValueStorageTraits::FirstTraits KeyStorageTraits;
        typedef typename KeyStorageTraits::TraitType KeyStorageType;
        typedef typename ValueStorageTraits::SecondTraits MappedStorageTraits;
        typedef typename MappedStorageTraits::TraitType MappedStorageType;
        typedef typename ValueTraits::FirstTraits KeyTraits;
        typedef typename ValueTraits::SecondTraits MappedTraits;
        
        static unsigned hash(const KeyType& key) { return HashFunctions::hash(key); }
        static bool equal(const KeyStorageType& a, const KeyType& b) { return HashFunctions::equal(*(KeyType*)&a, b); }
        static void translate(ValueStorageType& location, const KeyType& key, const MappedType& mapped)
        {
            if (location.first == KeyStorageTraits::deletedValue())
                location.first = KeyStorageTraits::emptyValue();
            Assigner<KeyTraits::needsRef, KeyType, KeyStorageType, KeyTraits>::assign(key, location.first);
            Assigner<MappedTraits::needsRef, MappedType, MappedStorageType, MappedTraits>::assign(mapped, location.second);
        }
    };

    template<typename T, typename U, typename V, typename W, typename X>
    inline void HashMap<T, U, V, W, X>::refAll()
    {
        HashTableRefCounter<HashTableType, ValueTraits>::refAll(m_impl);
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline void HashMap<T, U, V, W, X>::derefAll()
    {
        HashTableRefCounter<HashTableType, ValueTraits>::derefAll(m_impl);
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline HashMap<T, U, V, W, X>::HashMap()
    {
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline HashMap<T, U, V, W, X>::HashMap(const HashMap& other)
        : m_impl(other.m_impl)
    {
        refAll();
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline HashMap<T, U, V, W, X>& HashMap<T, U, V, W, X>::operator=(const HashMap& other)
    {
        HashMap tmp(other);
        swap(tmp); 
        return *this;
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline void HashMap<T, U, V, W, X>::swap(HashMap& other)
    {
        m_impl.swap(other.m_impl); 
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline HashMap<T, U, V, W, X>::~HashMap()
    {
        derefAll();
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline int HashMap<T, U, V, W, X>::size() const
    {
        return m_impl.size(); 
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline int HashMap<T, U, V, W, X>::capacity() const
    { 
        return m_impl.capacity(); 
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline bool HashMap<T, U, V, W, X>::isEmpty() const
    {
        return m_impl.isEmpty();
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline typename HashMap<T, U, V, W, X>::iterator HashMap<T, U, V, W, X>::begin()
    {
        return m_impl.begin();
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline typename HashMap<T, U, V, W, X>::iterator HashMap<T, U, V, W, X>::end()
    {
        return m_impl.end();
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline typename HashMap<T, U, V, W, X>::const_iterator HashMap<T, U, V, W, X>::begin() const
    {
        return m_impl.begin();
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline typename HashMap<T, U, V, W, X>::const_iterator HashMap<T, U, V, W, X>::end() const
    {
        return m_impl.end();
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline typename HashMap<T, U, V, W, X>::iterator HashMap<T, U, V, W, X>::find(const KeyType& key)
    {
        return m_impl.find(*(const KeyStorageType*)&key);
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline typename HashMap<T, U, V, W, X>::const_iterator HashMap<T, U, V, W, X>::find(const KeyType& key) const
    {
        return m_impl.find(*(const KeyStorageType*)&key);
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline bool HashMap<T, U, V, W, X>::contains(const KeyType& key) const
    {
        return m_impl.contains(*(const KeyStorageType*)&key);
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline pair<typename HashMap<T, U, V, W, X>::iterator, bool>
    HashMap<T, U, V, W, X>::inlineAdd(const KeyType& key, const MappedType& mapped) 
    {
        const bool canReplaceDeletedKey = !KeyTraits::needsDestruction || KeyStorageTraits::needsDestruction;
        typedef HashMapTranslator<canReplaceDeletedKey, ValueType, ValueTraits, ValueStorageTraits, HashFunctions> TranslatorType;
        return m_impl.template add<KeyType, MappedType, TranslatorType>(key, mapped);
    }

    template<typename T, typename U, typename V, typename W, typename X>
    pair<typename HashMap<T, U, V, W, X>::iterator, bool>
    HashMap<T, U, V, W, X>::set(const KeyType& key, const MappedType& mapped) 
    {
        pair<iterator, bool> result = inlineAdd(key, mapped);
        if (!result.second)
            // add call above didn't change anything, so set the mapped value
            result.first->second = mapped;
        return result;
    }

    template<typename T, typename U, typename V, typename W, typename X>
    pair<typename HashMap<T, U, V, W, X>::iterator, bool>
    HashMap<T, U, V, W, X>::add(const KeyType& key, const MappedType& mapped)
    {
        return inlineAdd(key, mapped);
    }

    template<typename T, typename U, typename V, typename W, typename MappedTraits>
    typename HashMap<T, U, V, W, MappedTraits>::MappedType
    HashMap<T, U, V, W, MappedTraits>::get(const KeyType& key) const
    {
        if (m_impl.isEmpty())
            return MappedTraits::emptyValue();
        ValueStorageType* entry = const_cast<HashTableType&>(m_impl).lookup(*(const KeyStorageType*)&key);
        if (!entry)
            return MappedTraits::emptyValue();
        return ((ValueType *)entry)->second;
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline void HashMap<T, U, V, W, X>::remove(iterator it)
    {
        if (it.m_impl == m_impl.end())
            return;
        RefCounter<ValueTraits, ValueStorageTraits>::deref(*it.m_impl);
        m_impl.remove(it.m_impl);
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline void HashMap<T, U, V, W, X>::remove(const KeyType& key)
    {
        remove(find(key));
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline void HashMap<T, U, V, W, X>::clear()
    {
        derefAll();
        m_impl.clear();
    }

    template<typename T, typename U, typename V, typename W, typename X>
    bool operator==(const HashMap<T, U, V, W, X>& a, const HashMap<T, U, V, W, X>& b)
    {
        if (a.size() != b.size())
            return false;

        typedef typename HashMap<T, U, V, W, X>::const_iterator const_iterator;

        const_iterator end = a.end();
        const_iterator notFound = b.end();
        for (const_iterator it = a.begin(); it != end; ++it) {
            const_iterator bPos = b.find(it->first);
            if (bPos == notFound || it->second != bPos->second)
                return false;
        }

        return true;
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline bool operator!=(const HashMap<T, U, V, W, X>& a, const HashMap<T, U, V, W, X>& b)
    {
        return !(a == b);
    }

    template<typename MappedType, typename HashTableType>
    void deleteAllPairSeconds(HashTableType& collection)
    {
        typedef typename HashTableType::const_iterator iterator;
        iterator end = collection.end();
        for (iterator it = collection.begin(); it != end; ++it)
            delete *(MappedType*)&it->second;
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline void deleteAllValues(const HashMap<T, U, V, W, X>& collection)
    {
        deleteAllPairSeconds<typename HashMap<T, U, V, W, X>::MappedType>(collection);
    }

    template<typename KeyType, typename HashTableType>
    void deleteAllPairFirsts(HashTableType& collection)
    {
        typedef typename HashTableType::const_iterator iterator;
        iterator end = collection.end();
        for (iterator it = collection.begin(); it != end; ++it)
            delete *(KeyType*)&it->first;
    }

    template<typename T, typename U, typename V, typename W, typename X>
    inline void deleteAllKeys(const HashMap<T, U, V, W, X>& collection)
    {
        deleteAllPairFirsts<typename HashMap<T, U, V, W, X>::KeyType>(collection);
    }
    
    template<typename T, typename U, typename V, typename W, typename X>
    inline void copyValuesToVector(const HashMap<T, U, V, W, X>& collection, Vector<U>& vector)
    {
        typedef typename HashMap<T, U, V, W, X>::const_iterator iterator;
        
        vector.resize(collection.size());
        
        iterator it = collection.begin();
        iterator end = collection.end();
        for (unsigned i = 0; it != end; ++it, ++i)
            vector[i] = (*it).second;
    }   

} // namespace WTF

using WTF::HashMap;

#endif /* WTF_HashMap_h */
