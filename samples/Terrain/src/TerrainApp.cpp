#include "cinder/app/App.h"
#include "cinder/Camera.h"
#include "cinder/CameraUi.h"
#include "cinder/gl/gl.h"

#include "TouchUI.h"

class TerrainApp : public ci::app::App 
{
public:
	TerrainApp();

	void draw() override;
	void update() override;
private:
	ci::CameraPersp	mCamera;

};

#include "cinder/app/RendererGl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

TerrainApp::TerrainApp()
{
}

void TerrainApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

void TerrainApp::update()
{
}

CINDER_APP( TerrainApp, RendererGl )
