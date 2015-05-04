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

class _TBOX_PREFIX_App : public ci::app::App
{
public:
	_TBOX_PREFIX_App();
	
	void				draw() override;
	void				resize() override;
	void				update() override;
private:
	ci::CameraPersp		mCamera;
	ci::gl::VboMeshRef	mCube;
	TouchUi				mTouchUi;
};

#include "cinder/app/RendererGl.h"
#include "cinder/Log.h"

using namespace ci;
using namespace ci::app;
using namespace std;

_TBOX_PREFIX_App::_TBOX_PREFIX_App()
{
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 45.0f, 0.01f, 100.0f );
	mCamera.lookAt( vec3( 0.0f, 0.0f, 5.0f ), vec3( 0.0f ) );
	
	mTouchUi.connect( getWindow() );
	mTouchUi.setScaleMin( vec2( 0.5f ) );
	
	mCube = gl::VboMesh::create( geom::WireCube() );
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
}

void _TBOX_PREFIX_App::draw()
{
	gl::clear();
	gl::setMatrices( mCamera );
	
	gl::translate( mTouchUi.getPan() * vec2( 1.0f, -1.0f ) );
	gl::rotate( mTouchUi.getRotation(), vec3( 1.0f ) );
	gl::scale( vec3( mTouchUi.getScale().x ) );
	
	gl::draw( mCube );
}

void _TBOX_PREFIX_App::resize()
{
	mTouchUi.setMask( getWindowBounds() );
}

void CubeApp::update()
{
	if ( mTouchUi.isTapped() ) {
		CI_LOG_V( mTouchUi.getTapPosition( true ) );
	}
}

CINDER_APP( _TBOX_PREFIX_App, RendererGl, []( App::Settings* settings )
{
	settings->setHighDensityDisplayEnabled( true );
	settings->setMultiTouchEnabled( true );
} )





#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"

#include <Box2D/Box2D.h>

using namespace ci;
using namespace ci::app;
using namespace std;

const float BOX_SIZE = 10;

class _TBOX_PREFIX_App : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
	
	void addBox( const vec2 &pos );
	
	b2World				*mWorld;
	vector<b2Body*>		mBoxes;
};

void _TBOX_PREFIX_App::setup()
{
	b2Vec2 gravity( 0.0f, 10.0f );
	mWorld = new b2World( gravity );

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set( 0.0f, getWindowHeight() );
	b2Body* groundBody = mWorld->CreateBody( &groundBodyDef );

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox( getWindowWidth(), 10.0f );

	// Add the ground fixture to the ground body.
	groundBody->CreateFixture( &groundBox, 0.0f );
	
}

void _TBOX_PREFIX_App::addBox( const vec2 &pos )
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set( pos.x, pos.y );

	b2Body *body = mWorld->CreateBody( &bodyDef );

	b2PolygonShape dynamicBox;
	dynamicBox.SetAsBox( BOX_SIZE, BOX_SIZE );

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.5f; // bounce

	body->CreateFixture( &fixtureDef );
	mBoxes.push_back( body );
}

void _TBOX_PREFIX_App::mouseDown( MouseEvent event )
{
	addBox( event.getPos() );
}

void _TBOX_PREFIX_App::update()
{
	for( int i = 0; i < 10; ++i )
		mWorld->Step( 1 / 30.0f, 10, 10 );
}

void _TBOX_PREFIX_App::draw()
{
	gl::clear();
	
	gl::color( 1, 0.5f, 0.25f );
	for( const auto &box : mBoxes ) {
		gl::pushModelMatrix();
		gl::translate( box->GetPosition().x, box->GetPosition().y );
		gl::rotate( box->GetAngle() );

		gl::drawSolidRect( Rectf( -BOX_SIZE, -BOX_SIZE, BOX_SIZE, BOX_SIZE ) );

		gl::popModelMatrix();
	}
}

CINDER_APP( _TBOX_PREFIX_App, RendererGl )