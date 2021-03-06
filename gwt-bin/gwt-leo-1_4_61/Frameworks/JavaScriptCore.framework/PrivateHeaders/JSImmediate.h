/*
 *  Copyright (C) 2003, 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.
 *  Copyright (C) 2006 Alexey Proskuryakov (ap@webkit.org)
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 *
 */

#ifndef KJS_JS_IMMEDIATE_H
#define KJS_JS_IMMEDIATE_H

#include "JSType.h"
#include <wtf/Assertions.h>
#include <wtf/AlwaysInline.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

namespace KJS {

class ExecState;
class JSObject;
class JSValue;
class UString;

/*
 * A JSValue*  is either a pointer to a cell (a heap-allocated object) or an immediate (a type-tagged 
 * IEEE floating point bit pattern masquerading as a pointer). The low two bits in a JSValue* are available 
 * for type tagging because allocator alignment guarantees they will be 00 in cell pointers.
 *
 * For example, on a 32 bit system:
 *
 * JSCell*:       XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                 00
 *               [ high 30 bits: pointer address ]  [ low 2 bits -- always 0 ]
 *
 * JSImmediate:   XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                 TT
 *             [ high 30 bits: IEEE encoded float ] [ low 2 bits -- type tag ]
 *
 * The bit "payload" (the hight 30 bits) of a non-numeric immediate is its numeric equivalent. For example, 
 * the payload of null is 0.0. This makes JSValue::toNumber() a simple bitmask for all immediates.
 *
 * Notice that the JSType value of NullType is 4, which requires 3 bits to encode. Since we only have 2 bits 
 * available for type tagging, we tag the null immediate with UndefinedType, and JSImmediate::type() has 
 * to sort them out. Null and Undefined don't otherwise get confused because the numeric value of Undefined is 
 * NaN, not 0.0.
 */

class JSImmediate {
public:
    static ALWAYS_INLINE bool isImmediate(const JSValue* v)
    {
        return getTag(v) != 0;
    }
    
    static ALWAYS_INLINE bool isNumber(const JSValue* v)
    {
        return (getTag(v) == NumberType);
    }
    
    static ALWAYS_INLINE bool isBoolean(const JSValue* v)
    {
        return (getTag(v) == BooleanType);
    }
    
    // Since we have room for only 3 unique tags, null and undefined have to share.
    static ALWAYS_INLINE bool isUndefinedOrNull(const JSValue* v)
    {
        return (getTag(v) == UndefinedType);
    }

    static JSValue* fromDouble(double d);
    static double toDouble(const JSValue*);
    static bool toBoolean(const JSValue*);
    static JSObject* toObject(const JSValue*, ExecState*);
    static UString toString(const JSValue*);
    static JSType type(const JSValue*);

    static bool getUInt32(const JSValue*, uint32_t&);
    static bool getTruncatedInt32(const JSValue*, int32_t&);
    static bool getTruncatedUInt32(const JSValue*, uint32_t&);

    // It would nice just to use fromDouble() to create these values, but that would prevent them from
    // turning into compile-time constants.
    static JSValue* trueImmediate();
    static JSValue* falseImmediate();
    static JSValue* NaNImmediate();
    static JSValue* undefinedImmediate();
    static JSValue* nullImmediate();
    
private:
    static const uintptr_t TagMask = 3; // type tags are 2 bits long
    
    static ALWAYS_INLINE JSValue* tag(uintptr_t bits, uintptr_t tag)
    {
        return reinterpret_cast<JSValue*>(bits | tag);
    }
    
    static ALWAYS_INLINE uintptr_t unTag(const JSValue* v)
    {
        return reinterpret_cast<uintptr_t>(v) & ~TagMask;
    }
    
    static ALWAYS_INLINE uintptr_t getTag(const JSValue* v)
    {
        return reinterpret_cast<uintptr_t>(v) & TagMask;
    }
    
    // NOTE: With f-strict-aliasing enabled, unions are the only safe way to do type masquerading.

    union FloatUnion {
        uint32_t asBits;
        float    asFloat;
    };

    union DoubleUnion {
        uint64_t asBits;
        double   asDouble;
    };

    // we support 32-bit platforms with sizes like this
    static const bool is32bit = 
        sizeof(float) == sizeof(uint32_t) && sizeof(double) == sizeof(uint64_t) && sizeof(uintptr_t) == sizeof(uint32_t);

    // we support 64-bit platforms with sizes like this
    static const bool is64bit =
        sizeof(float) == sizeof(uint32_t) && sizeof(double) == sizeof(uint64_t) && sizeof(uintptr_t) == sizeof(uint64_t);

    template<bool for32bit, bool for64bit> struct FPBitValues {};
};

template<> struct JSImmediate::FPBitValues<true, false> {
    static const uint32_t nanAsBits = 0x7fc00000;
    static const uint32_t oneAsBits = 0x3f800000;
    static const uint32_t zeroAsBits = 0x0;

    static ALWAYS_INLINE JSValue* fromDouble(double d)
    {
        FloatUnion floatUnion;
        floatUnion.asFloat = static_cast<float>(d);

        // check for data loss from tagging
        if ((floatUnion.asBits & TagMask) != 0)
            return 0;

        // check for data loss from conversion to float
        // The d == d check is to allow NaN - it does not
        // compare equal to itself, but we do want to allow it
        if (floatUnion.asFloat != d && d == d)
            return 0;

        return tag(floatUnion.asBits, NumberType);
    }

    static ALWAYS_INLINE float toFloat(const JSValue* v)
    {
        ASSERT(isImmediate(v));

        FloatUnion floatUnion;
        floatUnion.asBits = static_cast<uint32_t>(unTag(v));
        return floatUnion.asFloat;
    }

    static ALWAYS_INLINE double toDouble(const JSValue* v)
    {
        return toFloat(v);
    }

    static ALWAYS_INLINE bool getTruncatedInt32(const JSValue* v, int32_t& i)
    {
        float f = toFloat(v);
        if (!(f >= -2147483648.0F && f < 2147483648.0F))
            return false;
        i = static_cast<int32_t>(f);
        return isNumber(v);
    }

    static ALWAYS_INLINE bool getTruncatedUInt32(const JSValue* v, uint32_t& i)
    {
        float f = toFloat(v);
        if (!(f >= 0.0F && f < 4294967296.0F))
            return false;
        i = static_cast<uint32_t>(f);
        return isNumber(v);
    }
};

template<> struct JSImmediate::FPBitValues<false, true> {
    static const uint64_t nanAsBits = 0x7ff80000ULL << 32;
    static const uint64_t oneAsBits = 0x3ff00000ULL << 32;
    static const uint64_t zeroAsBits = 0x0;

    static ALWAYS_INLINE JSValue* fromDouble(double d)
    {
        DoubleUnion doubleUnion;
        doubleUnion.asDouble = d;

        // check for data loss from tagging
        if ((doubleUnion.asBits & TagMask) != 0)
            return 0;

        return tag(static_cast<uintptr_t>(doubleUnion.asBits), NumberType);
    }

    static ALWAYS_INLINE double toDouble(const JSValue* v)
    {
        ASSERT(isImmediate(v));

        DoubleUnion doubleUnion;
        doubleUnion.asBits = unTag(v);
        return doubleUnion.asDouble;
    }

    static ALWAYS_INLINE bool getTruncatedInt32(const JSValue* v, int32_t& i)
    {
        double d = toDouble(v);
        if (!(d >= -2147483648.0 && d < 2147483648.0))
            return false;
        i = static_cast<int32_t>(d);
        return isNumber(v);
    }

    static ALWAYS_INLINE bool getTruncatedUInt32(const JSValue* v, uint32_t& i)
    {
        double d = toDouble(v);
        if (!(d >= 0.0 && d < 4294967296.0))
            return false;
        i = static_cast<uint32_t>(d);
        return isNumber(v);
    }
};

ALWAYS_INLINE JSValue* JSImmediate::trueImmediate() { return tag(FPBitValues<is32bit, is64bit>::oneAsBits, BooleanType); }
ALWAYS_INLINE JSValue* JSImmediate::falseImmediate() { return tag(FPBitValues<is32bit, is64bit>::zeroAsBits, BooleanType); }
ALWAYS_INLINE JSValue* JSImmediate::NaNImmediate() { return tag(FPBitValues<is32bit, is64bit>::nanAsBits, NumberType); }
ALWAYS_INLINE JSValue* JSImmediate::undefinedImmediate() { return tag(FPBitValues<is32bit, is64bit>::nanAsBits, UndefinedType); }
ALWAYS_INLINE JSValue* JSImmediate::nullImmediate() { return tag(FPBitValues<is32bit, is64bit>::zeroAsBits, UndefinedType); }

ALWAYS_INLINE bool JSImmediate::toBoolean(const JSValue* v)
{
    ASSERT(isImmediate(v));

    uintptr_t bits = unTag(v);
    if ((bits << 1) == 0) // -0.0 has the sign bit set
        return false;

    return bits != FPBitValues<is32bit, is64bit>::nanAsBits;
}

ALWAYS_INLINE JSValue* JSImmediate::fromDouble(double d)
{
    return FPBitValues<is32bit, is64bit>::fromDouble(d);
}

ALWAYS_INLINE double JSImmediate::toDouble(const JSValue* v)
{
    return FPBitValues<is32bit, is64bit>::toDouble(v);
}

ALWAYS_INLINE bool JSImmediate::getUInt32(const JSValue* v, uint32_t& i)
{
    double d = toDouble(v);
    i = static_cast<uint32_t>(d);
    return isNumber(v) & (i == d);
}

ALWAYS_INLINE bool JSImmediate::getTruncatedInt32(const JSValue* v, int32_t& i)
{
    return FPBitValues<is32bit, is64bit>::getTruncatedInt32(v, i);
}

ALWAYS_INLINE bool JSImmediate::getTruncatedUInt32(const JSValue* v, uint32_t& i)
{
    return FPBitValues<is32bit, is64bit>::getTruncatedUInt32(v, i);
}

} // namespace KJS

#endif
