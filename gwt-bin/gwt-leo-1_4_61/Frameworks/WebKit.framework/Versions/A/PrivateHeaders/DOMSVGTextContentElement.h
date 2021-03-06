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

#import <WebKit/DOMSVGElement.h>
#import <WebKit/DOMSVGTests.h>
#import <WebKit/DOMSVGLangSpace.h>
#import <WebKit/DOMSVGExternalResourcesRequired.h>
#import <WebKit/DOMSVGStylable.h>

@class DOMSVGAnimatedEnumeration;
@class DOMSVGAnimatedLength;
@class DOMSVGPoint;
@class DOMSVGRect;

enum {
    DOM_LENGTHADJUST_UNKNOWN = 0,
    DOM_LENGTHADJUST_SPACING = 1,
    DOM_LENGTHADJUST_SPACINGANDGLYPHS = 2
};

@interface DOMSVGTextContentElement : DOMSVGElement <DOMSVGTests, DOMSVGLangSpace, DOMSVGExternalResourcesRequired, DOMSVGStylable>
@property(readonly, retain) DOMSVGAnimatedLength *textLength;
@property(readonly, retain) DOMSVGAnimatedEnumeration *lengthAdjust;

- (int)getNumberOfChars;
- (float)getComputedTextLength;
- (float)getSubStringLength:(unsigned)offset length:(unsigned)length;
- (DOMSVGPoint *)getStartPositionOfChar:(unsigned)offset;
- (DOMSVGPoint *)getEndPositionOfChar:(unsigned)offset;
- (DOMSVGRect *)getExtentOfChar:(unsigned)offset;
- (float)getRotationOfChar:(unsigned)offset;
- (int)getCharNumAtPosition:(DOMSVGPoint *)point;
- (void)selectSubString:(unsigned)offset length:(unsigned)length;
@end
