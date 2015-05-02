#include "cinder/app/App.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"
#include "cinder/gl/gl.h"

#include "TouchUI.h"

class CubeApp : public ci::app::App
{
public:
	CubeApp();

	void			draw() override;
	void			update() override;
private:
	ci::CameraPersp	mCamera;
	TouchUi			mTouchUi;
};

#include "cinder/Log.h"
#include "cinder/app/RendererGl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

CubeApp::CubeApp()
{
	mCamera = CameraPersp( getWindowWidth(), getWindowHeight(), 45.0f, 0.01f, 100.0f );
	mCamera.lookAt( vec3( 0.0f, 0.0f, 5.0f ), vec3( 0.0f ) );
	
	mTouchUi.connect( getWindow() );
	mTouchUi.setScaleMin( 0.5f );
	
	gl::enableDepthRead();
	gl::enableDepthWrite();
}

void CubeApp::draw()
{
	gl::clear();
	gl::setMatrices( mCamera );
	
	gl::translate( mTouchUi.getPan() * vec2( 1.0f, -1.0f ) );
	gl::rotate( mTouchUi.getRotation(), vec3( 1.0f ) );
	gl::scale( vec3( mTouchUi.getScale() ) );
	
	gl::drawColorCube( vec3( 0.0f ), vec3( 1.0f ) );
}

void CubeApp::update()
{
	if ( mTouchUi.isTapped() ) {
		CI_LOG_V( mTouchUi.getTapLocation() );
	}
}

CINDER_APP( CubeApp, RendererGl, []( App::Settings* settings )
{
	settings->setHighDensityDisplayEnabled( true );
	settings->setMultiTouchEnabled( true );
} )
