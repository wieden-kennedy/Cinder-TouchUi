#pragma once

#include "cinder/Vector.h"
#include "cinder/app/TouchEvent.h"
#include "cinder/app/Window.h"

class TouchUi
{
public:
	TouchUi();
	TouchUi( const ci::app::WindowRef &window = nullptr, int signalPriority = 0 );
	TouchUi( const TouchUi &rhs );
	~TouchUi();

	TouchUi&				operator=( const TouchUi &rhs );

	void					connect( const ci::app::WindowRef &window, int signalPriority = 0 );
	void					disconnect();
	void					enable( bool enable = true ) { mEnabled = enable; }
	void					disable() { mEnabled = false; }
	bool					isEnabled() const { return mEnabled; }

	void					setWindowSize( const ci::ivec2 &windowSizePixels ) { mWindowSize = windowSizePixels; }
protected:
	void					touchesBegan( ci::app::TouchEvent &event );
	void					touchesEnded( ci::app::TouchEvent &event );
	void					touchesMoved( ci::app::TouchEvent &event );

	ci::ivec2				getWindowSize() const;

	bool					mEnabled;
	ci::ivec2				mWindowSize;
	ci::app::WindowRef		mWindow;

	int32_t					mSignalPriority;
	
	ci::signals::Connection	mConnectionResize;
	ci::signals::Connection	mConnectionTouchesBegan;
	ci::signals::Connection	mConnectionTouchesEnded;
	ci::signals::Connection	mConnectionTouchesMoved;
};