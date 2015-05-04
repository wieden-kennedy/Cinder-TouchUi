/*
*
* Copyright (c) 2015, Wieden+Kennedy
* Stephen Schieberl
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or
* without modification, are permitted provided that the following
* conditions are met:
*
* Redistributions of source code must retain the above copyright
* notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in
* the documentation and/or other materials provided with the
* distribution.
*
* Neither the name of the Ban the Rewind nor the names of its
* contributors may be used to endorse or promote products
* derived from this software without specific prior written
* permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include "cinder/app/App.h"

#include "TouchUI.h"

class CircleApp : public ci::app::App
{
public:
	CircleApp();

	void				draw() override;
	void				update() override;
private:
	std::list<TouchUi>	mTouchUis;
};

#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

CircleApp::CircleApp()
{
	const size_t numCircles = 5;
	float t					= 0.0f;
	const float d			= ( 1.0f / (float)numCircles ) * (float)M_PI * 2.0f;
	const vec2 c			= getWindowCenter();
	static const float r	= 150.0f;
	for ( size_t i = 0; i < numCircles; ++i, t += d ) {
		TouchUi touchUi( getWindow() );
		touchUi.disableRotation();
		touchUi.disableScale();
		touchUi.setInterpolationSpeed( 0.8f );
		touchUi.setPanSpeed( vec2( 0.5f ) );

		vec2 v = c + vec2( glm::cos( t ), glm::sin( t ) ) * r;
		touchUi.setMask( v, r * 0.5f, 32 );
		touchUi.setPan( v );

		mTouchUis.push_back( touchUi );
	}
}

void CircleApp::draw()
{
	gl::clear();
	gl::setMatricesWindow( getWindowSize() );
	
	for ( const TouchUi& touchUi : mTouchUis ) {
		gl::draw( touchUi.getMask() );
	}
}

void CircleApp::update()
{
	static const float pi = (float)M_PI;
	size_t i = 0;
	for ( TouchUi& a : mTouchUis ) {
		const Path2d& mask			= a.getMask();
		const float r				= mask.calcBoundingBox().getWidth() * 0.5f;
		const size_t numSegments	= mask.getNumSegments();
		vec2 ap						= a.getPan();
		size_t j = 0;
		for ( TouchUi& b : mTouchUis ) {
			if ( i != j ) {
				vec2 bp = b.getPan();
				float d = glm::distance( ap, bp );
				if ( d < r * 2.0f ) {
					float a = atan2( ap.y - bp.y, ap.x - bp.x );
					a = a < pi ? a + pi : a - pi;
					vec2 v	= vec2( glm::cos( a ), glm::sin( a ) ) * r * 2.0f * 0.05f;
					ap		-= v;
					bp		+= v;
					b.setPan( bp );
				}
			}
			++j;
		}
		a.setMask( ap, r, numSegments );
		++i;
	}
}

CINDER_APP( CircleApp, RendererGl, []( App::Settings* settings )
{
	settings->setHighDensityDisplayEnabled( true );
	settings->setMultiTouchEnabled( true );
} )
