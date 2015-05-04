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
#include "cinder/Camera.h"
#include "cinder/gl/gl.h"

#include "TouchUI.h"

class PlaneApp : public ci::app::App
{
public:
	PlaneApp();

	void				draw() override;
	void				resize() override;
	void				update() override;
private:
	ci::CameraPersp		mCamera;
	ci::gl::VboMeshRef	mPlane;
	TouchUi				mTouchUiOne;
	TouchUi				mTouchUiTwo;
};

#include "cinder/app/RendererGl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

PlaneApp::PlaneApp()
{
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 45.0f, 0.01f, 100.0f );
	mCamera.lookAt( vec3( 0.0f, 5.0f, 5.0f ), vec3( 0.0f ) );
	
	mTouchUiOne.connect( getWindow() );
	mTouchUiOne.disableTap();
	mTouchUiOne.disableRotation();
	mTouchUiOne.disableScale();
	mTouchUiOne.setNumTouchPointsMax( 1 );
	mTouchUiOne.setPanSpeed( vec2( 0.01f ) );
	
	mTouchUiTwo.connect( getWindow() );
	mTouchUiTwo.disableTap();
	mTouchUiTwo.setNumTouchPoints( 2, 2 );
	mTouchUiOne.setPanSpeed( vec2( 0.0067f ) );
	mTouchUiTwo.setScaleMin( vec2( 0.5f ) );
	
	mPlane = gl::VboMesh::create( geom::WirePlane().subdivisions( ivec2( 128, 128 ) ) );
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
}

void PlaneApp::draw()
{
	gl::clear();
	gl::setMatrices( mCamera );
	
	gl::translate( vec3( mTouchUiTwo.getPan().x, 0.0f, mTouchUiTwo.getPan().y ) );
	gl::rotate( mTouchUiTwo.getRotation(), vec3( 0.0f, 1.0f, 0.0f ) );
	gl::scale( vec3( mTouchUiTwo.getScale().x ) );
	
	gl::draw( mPlane );
}

void PlaneApp::resize()
{
	mTouchUiOne.setMask( getWindowBounds() );
	mTouchUiTwo.setMask( getWindowBounds() );
}

void PlaneApp::update()
{
	vec3 target( -mTouchUiOne.getPan().x, mTouchUiOne.getPan().y, 0.0f );
	mCamera.lookAt( mCamera.getEyePoint(), target );
}

CINDER_APP( PlaneApp, RendererGl, []( App::Settings* settings )
{
	settings->setHighDensityDisplayEnabled( true );
	settings->setMultiTouchEnabled( true );
} )
