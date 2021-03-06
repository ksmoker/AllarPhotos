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

#import <WebCore/DOMSVGElement.h>
#import <WebCore/DOMSVGLangSpace.h>
#import <WebCore/DOMSVGExternalResourcesRequired.h>
#import <WebCore/DOMSVGStylable.h>
#import <WebCore/DOMSVGFitToViewBox.h>

@class DOMSVGAngle;
@class DOMSVGAnimatedAngle;
@class DOMSVGAnimatedEnumeration;
@class DOMSVGAnimatedLength;

enum {
    DOM_SVG_MARKERUNITS_UNKNOWN = 0,
    DOM_SVG_MARKERUNITS_USERSPACEONUSE = 1,
    DOM_SVG_MARKERUNITS_STROKEWIDTH = 2,
    DOM_SVG_MARKER_ORIENT_UNKNOWN = 0,
    DOM_SVG_MARKER_ORIENT_AUTO = 1,
    DOM_SVG_MARKER_ORIENT_ANGLE = 2
};

@interface DOMSVGMarkerElement : DOMSVGElement <DOMSVGLangSpace, DOMSVGExternalResourcesRequired, DOMSVGStylable, DOMSVGFitToViewBox>
@property(readonly, retain) DOMSVGAnimatedLength *refX;
@property(readonly, retain) DOMSVGAnimatedLength *refY;
@property(readonly, retain) DOMSVGAnimatedEnumeration *markerUnits;
@property(readonly, retain) DOMSVGAnimatedLength *markerWidth;
@property(readonly, retain) DOMSVGAnimatedLength *markerHeight;
@property(readonly, retain) DOMSVGAnimatedEnumeration *orientType;
@property(readonly, retain) DOMSVGAnimatedAngle *orientAngle;

- (void)setOrientToAuto;
- (void)setOrientToAngle:(DOMSVGAngle *)angle;
@end
