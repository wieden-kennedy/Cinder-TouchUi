#include "TouchUi.h"

using namespace ci;
using namespace ci::app;
using namespace std;

TouchUi::TouchUi()
	: mWindowSize( 640, 480 ), mEnabled( true )
{
}

TouchUi::TouchUi( const WindowRef &window, int signalPriority )
	: mEnabled( true )
{
	connect( window, signalPriority );
}

TouchUi::TouchUi( const TouchUi &rhs )
	: mWindowSize( rhs.mWindowSize ), mWindow( rhs.mWindow ), 
	mSignalPriority( rhs.mSignalPriority ), mEnabled( rhs.mEnabled )
{
	connect( mWindow, mSignalPriority );
}

TouchUi::~TouchUi()
{
	disconnect();
}

TouchUi& TouchUi::operator=( const TouchUi &rhs )
{
	mWindowSize		= rhs.mWindowSize;
	mWindow			= rhs.mWindow;
	mSignalPriority = rhs.mSignalPriority;
	mEnabled		= rhs.mEnabled;
	connect( mWindow, mSignalPriority );
	return *this;
}

void TouchUi::connect( const WindowRef &window, int signalPriority )
{
	mWindow			= window;
	mSignalPriority	= signalPriority;
	if ( window ) {
		mConnectionResize = window->getSignalResize().connect( signalPriority,
			[ this ]() { setWindowSize( mWindow->getSize() ); } );
		mConnectionTouchesBegan = window->getSignalTouchesBegan().connect( signalPriority,
			[ this ]( app::TouchEvent &event ) { touchesBegan( event ); } );
		mConnectionTouchesEnded = window->getSignalTouchesEnded().connect( signalPriority,
			[ this ]( app::TouchEvent &event ) { touchesEnded( event ); } );
		mConnectionTouchesMoved = window->getSignalTouchesMoved().connect( signalPriority,
			[ this ]( app::TouchEvent &event ) { touchesMoved( event ); } );
	} else {
		disconnect();
	}
}

void TouchUi::disconnect()
{
	mConnectionResize.disconnect();
	mConnectionTouchesBegan.disconnect();
	mConnectionTouchesEnded.disconnect();
	mConnectionTouchesMoved.disconnect();
	mWindow.reset();
}

void TouchUi::touchesBegan( TouchEvent &event )
{
}

void TouchUi::touchesEnded( ci::app::TouchEvent &event )
{
}

void TouchUi::touchesMoved( ci::app::TouchEvent &event )
{
}

ivec2 TouchUi::getWindowSize() const
{
	if ( mWindow ) {
		return mWindow->getSize();
	} else {
		return mWindowSize;
	}
}