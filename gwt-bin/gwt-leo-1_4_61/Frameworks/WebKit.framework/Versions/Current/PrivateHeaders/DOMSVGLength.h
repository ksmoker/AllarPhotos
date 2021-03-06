/*
 * Copyright (C) 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.
 * Copyright (C) 2006 Samuel Weinig <sam.weinig@gmail.com>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#import <WebKit/DOMObject.h>

@class NSString;

enum {
    DOM_SVG_LENGTHTYPE_UNKNOWN = 0,
    DOM_SVG_LENGTHTYPE_NUMBER = 1,
    DOM_SVG_LENGTHTYPE_PERCENTAGE = 2,
    DOM_SVG_LENGTHTYPE_EMS = 3,
    DOM_SVG_LENGTHTYPE_EXS = 4,
    DOM_SVG_LENGTHTYPE_PX = 5,
    DOM_SVG_LENGTHTYPE_CM = 6,
    DOM_SVG_LENGTHTYPE_MM = 7,
    DOM_SVG_LENGTHTYPE_IN = 8,
    DOM_SVG_LENGTHTYPE_PT = 9,
    DOM_SVG_LENGTHTYPE_PC = 10
};

@interface DOMSVGLength : DOMObject
@property(readonly) unsigned short unitType;
@property float value;
@property float valueInSpecifiedUnits;
@property(copy) NSString *valueAsString;

- (void)newValueSpecifiedUnits:(unsigned short)unitType valueInSpecifiedUnits:(float)valueInSpecifiedUnits;
- (void)convertToSpecifiedUnits:(unsigned short)unitType;
@end
