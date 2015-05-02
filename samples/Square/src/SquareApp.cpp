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

class SquareApp : public ci::app::App
{
public:
	SquareApp();

	void		draw() override;
	void		update() override;
private:
	ci::Rectf	mRect;
	TouchUi		mTouchUi;
};

#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

SquareApp::SquareApp()
{
	vec2 c	= getWindowCenter();
	vec2 v0 = c + vec2( -100.0f );
	vec2 v1 = c + vec2( 100.0f );
	mRect = Rectf( v0, v1 );

	mTouchUi.connect( getWindow() );
	mTouchUi.setScaleMin( 0.5f );
	mTouchUi.setMask( mRect );
	mTouchUi.setPanSpeed( vec2( 1.333f ) );
	mTouchUi.setScaleSpeed( mTouchUi.getScaleSpeed() * 3.0f );
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
}

void SquareApp::draw()
{
	gl::clear();
	gl::setMatricesWindow( getWindowSize() );
	
	gl::draw( mTouchUi.getMask() );
}

void SquareApp::update()
{
	mat4 m( 1.0f );
	m = glm::translate( m, vec3( mTouchUi.getPan(), 0.0f ) );
	m = glm::translate( m, vec3( mRect.getCenter(), 0.0f ) );
	m = glm::rotate( m, mTouchUi.getRotation(), vec3( 0.0f, 0.0f, -1.0f ) );
	m = glm::scale( m, vec3( mTouchUi.getScale() ) );
	m = glm::translate( m, -vec3( mRect.getCenter(), 0.0f ) );

	vec2 v0 = vec2( m * vec4( mRect.getUpperLeft(),		0.0f, 1.0f ) );
	vec2 v1 = vec2( m * vec4( mRect.getUpperRight(),	0.0f, 1.0f ) );
	vec2 v2 = vec2( m * vec4( mRect.getLowerRight(),	0.0f, 1.0f ) );
	vec2 v3 = vec2( m * vec4( mRect.getLowerLeft(),		0.0f, 1.0f ) );
	
	Path2d path;
	path.moveTo( v0 );
	path.lineTo( v1 );
	path.lineTo( v2 );
	path.lineTo( v3 );
	path.close();

	mTouchUi.setMask( path );
}

CINDER_APP( SquareApp, RendererGl, []( App::Settings* settings )
{
	settings->setHighDensityDisplayEnabled( true );
	settings->setMultiTouchEnabled( true );
} )
