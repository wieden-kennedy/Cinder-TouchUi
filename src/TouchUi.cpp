#include "TouchUi.h"
#include "cinder/app/App.h"

using namespace ci;
using namespace ci::app;
using namespace std;

TouchUi::TouchUi( const WindowRef& window, int signalPriority )
{
	mEnabled			= true;
	mInterpolationSpeed	= 0.33f;
	mPan				= vec2( 0.0f );
	mPanTarget			= mPan;
	mPanMax				= vec2( numeric_limits<float>::max() );
	mPanMin				= vec2( -numeric_limits<float>::max() );
	mPanSpeed			= vec2( 0.0033f );
	mPanThreshold		= toPixels( vec2( 1.0f ) );
	mRotationSpeed		= -2.5f;
	mRotationThreshold	= 0.005f;
	mScale				= 1.0f;
	mScaleTarget		= mScale;
	mScaleMax			= numeric_limits<float>::max();
	mScaleMin			= 0.0f;
	mScaleSpeed			= 0.0067f;
	mScaleThreshold		= toPixels( 1.0f );
	mTap				= false;
	mTapDelay			= 0.05;
	mTapLocation		= vec2( numeric_limits<float>::min() );
	mTapTime			= 0.0;
	mTapThreshold		= toPixels( 15.0f );
	
	connect( window, signalPriority );
}

TouchUi::TouchUi( const TouchUi& rhs )
{
	*this = rhs;
	connect( mWindow, mSignalPriority );
}

TouchUi::~TouchUi()
{
	disconnect();
}

TouchUi& TouchUi::operator=( const TouchUi& rhs )
{
	mEnabled			= rhs.mEnabled;
	mInterpolationSpeed	= rhs.mInterpolationSpeed;
	mPan				= rhs.mPan;
	mPanSpeed			= rhs.mPanSpeed;
	mPanTarget			= rhs.mPanTarget;
	mPanThreshold		= rhs.mPanThreshold;
	mRotation			= rhs.mRotation;
	mRotationSpeed		= rhs.mRotationSpeed;
	mRotationTarget		= rhs.mRotationTarget;
	mRotationThreshold	= rhs.mRotationThreshold;
	mScale				= rhs.mScale;
	mScaleSpeed			= rhs.mScaleSpeed;
	mScaleTarget		= rhs.mScaleTarget;
	mScaleThreshold		= rhs.mScaleThreshold;
	mSignalPriority		= rhs.mSignalPriority;
	mTap				= rhs.mTap;
	mTapDelay			= rhs.mTapDelay;
	mTapLocation		= rhs.mTapLocation;
	mTapThreshold		= rhs.mTapThreshold;
	mTapTime			= rhs.mTapTime;
	mWindow				= rhs.mWindow;
	
	setPanMax( rhs.mPanMax );
	setPanMin( rhs.mPanMin );
	setScaleMax( rhs.mScaleMax );
	setScaleMin( rhs.mScaleMin );
	
	connect( mWindow, mSignalPriority );
	
	return *this;
}

void TouchUi::connect( const WindowRef& window, int signalPriority )
{
	mSignalPriority	= signalPriority;
	mWindow			= window;
	if ( window ) {
		mConnectionTouchesBegan = window->getSignalTouchesBegan().connect( signalPriority,
			[ this ]( app::TouchEvent &event ) { touchesBegan( event ); } );
		mConnectionTouchesEnded = window->getSignalTouchesEnded().connect( signalPriority,
			[ this ]( app::TouchEvent &event ) { touchesEnded( event ); } );
		mConnectionTouchesMoved = window->getSignalTouchesMoved().connect( signalPriority,
			[ this ]( app::TouchEvent &event ) { touchesMoved( event ); } );
		mConnectionUpdate = app::App::get()->getSignalUpdate().connect( signalPriority,
			[ this ]() { update(); } );
	} else {
		disconnect();
	}
}

void TouchUi::disconnect()
{
	mConnectionTouchesBegan.disconnect();
	mConnectionTouchesEnded.disconnect();
	mConnectionTouchesMoved.disconnect();
	mConnectionUpdate.disconnect();
	mWindow.reset();
}


void TouchUi::disable()
{
	mEnabled = false;
}

void TouchUi::enable( bool enable )
{
	mEnabled = enable;
}

bool TouchUi::isEnabled() const
{
	return mEnabled;
}

const vec2& TouchUi::getPan() const
{
	return mPan;
}

float TouchUi::getRotation() const
{
	return mRotation.z;
}

float TouchUi::getScale() const
{
	return mScale;
}

const vec2& TouchUi::getTapLocation() const
{
	return mTapLocation;
}

float TouchUi::getInterpolationSpeed() const
{
	return mInterpolationSpeed;
}

const vec2& TouchUi::getPanMax() const
{
	return mPanMax;
}

const vec2& TouchUi::getPanMin() const
{
	return mPanMin;
}

const vec2& TouchUi::getPanSpeed() const
{
	return mPanSpeed;
}

const vec2& TouchUi::getPanThreshold() const
{
	return mPanThreshold;
}

float TouchUi::getRotationSpeed() const
{
	return mRotationSpeed;
}

float TouchUi::getRotationThreshold() const
{
	return mRotationThreshold;
}

float TouchUi::getScaleMax() const
{
	return mScaleMax;
}

float TouchUi::getScaleMin() const
{
	return mScaleMin;
}

float TouchUi::getScaleSpeed() const
{
	return mScaleSpeed;
}

float TouchUi::getScaleThreshold() const
{
	return mScaleThreshold;
}

float TouchUi::getTapThreshold() const
{
	return mTapThreshold;
}

bool TouchUi::isTapped() const
{
	return mTap;
}

void TouchUi::setInterpolationSpeed( float v )
{
	mInterpolationSpeed = v;
}

void TouchUi::setPanMax( const vec2& v )
{
	mPanMax = v;
	if ( mPanMin.x > mPanMax.x ) {
		swap( mPanMin.x, mPanMax.x );
	}
	if ( mPanMin.y > mPanMax.y ) {
		swap( mPanMin.y, mPanMax.y );
	}
}

void TouchUi::setPanMin( const vec2& v )
{
	mPanMin = v;
	if ( mPanMin.x > mPanMax.x ) {
		swap( mPanMin.x, mPanMax.x );
	}
	if ( mPanMin.y > mPanMax.y ) {
		swap( mPanMin.y, mPanMax.y );
	}
}

void TouchUi::setPanSpeed( const vec2& v )
{
	mPanSpeed = v;
}

void TouchUi::setPanThreshold( const vec2& v )
{
	mPanThreshold = v;
}

void TouchUi::setRotationSpeed( float v )
{
	mRotationSpeed = v;
}

void TouchUi::setRotationThreshold( float v )
{
	mRotationThreshold = v;
}

void TouchUi::setScaleMax( float v )
{
	mScaleMax = v;
	if ( mScaleMin > mScaleMax ) {
		swap( mScaleMin, mScaleMax );
	}
}

void TouchUi::setScaleMin( float v )
{
	mScaleMin = v;
	if ( mScaleMin > mScaleMax ) {
		swap( mScaleMin, mScaleMax );
	}
}

void TouchUi::setScaleSpeed( float v )
{
	mScaleSpeed = v;
}

void TouchUi::setScaleThreshold( float v )
{
	mScaleThreshold = v;
}

void TouchUi::setTapDelay( double v )
{
	mTapDelay = v;
}

void TouchUi::setTapThreshold( float v )
{
	mTapThreshold = v;
}

void TouchUi::zero()
{
	mPanTarget		= vec2( 0.0f );
	mRotationTarget	= quat();
	mScaleTarget	= 1.0;
	mTapLocation	= vec2( numeric_limits<float>::min() );
	mTapTime		= 0.0;
}

void TouchUi::touchesBegan( TouchEvent& event )
{
	if ( event.getTouches().size() == 1 ) {
		mTapLocation	= toPixels( event.getTouches().begin()->getPos() );
		mTapTime		= app::App::get()->getElapsedSeconds();
	}
}

void TouchUi::touchesEnded( app::TouchEvent& event )
{
	if ( mTapTime > 0.0 && event.getTouches().size() == 1 ) {
		vec2 tap( toPixels( event.getTouches().begin()->getPos() ) );
		if ( glm::distance( tap, mTapLocation ) < mTapThreshold ) {
			mTap			= true;
			mTapLocation	= tap;
			mTapTime		= app::App::get()->getElapsedSeconds();
		}
	}
}

void TouchUi::touchesMoved( app::TouchEvent& event )
{
	const vec2 panSpeed			= mPanSpeed * vec2( pow( ( mScaleMax + mScaleMin ) - mScale, 0.0002f ) );
	static const float pi		= (float)M_PI;
	static const float twoPi	= pi * 2.0f;
	
	// End tap if multiple touches or location has moved too far
	size_t numTouches = event.getTouches().size();
	if ( numTouches > 1 ) {
		mTapLocation	= vec2( numeric_limits<float>::min() );
		mTapTime		= 0.0;
	} else {
		vec2 tap( toPixels( event.getTouches().begin()->getPos() ) );
		if ( glm::distance( tap, mTapLocation ) > mTapThreshold ) {
			mTapLocation	= vec2( numeric_limits<float>::min() );
			mTapTime		= 0.0;
		}
	}
	
	float panX		= 0.0f;
	float panY		= 0.0f;
	float scale		= 0.0f;
	float rotation	= 0.0f;

	// Pan
	if ( numTouches > 0 ) {
		const TouchEvent::Touch& touch = *event.getTouches().begin();
		const vec2 a( toPixels( touch.getPos() ) );
		const vec2 b( toPixels( touch.getPrevPos() ) );
		panX = a.x - b.x;
		panY = a.y - b.y;
	}
	
	if ( numTouches > 1 ) {
		const TouchEvent::Touch& a = *event.getTouches().begin();
		const TouchEvent::Touch& b = *( event.getTouches().begin() + 1 );
		const vec2 ap0( toPixels( a.getPos() ) );
		const vec2 ap1( toPixels( a.getPrevPos() ) );
		const vec2 bp0( toPixels( b.getPos() ) );
		const vec2 bp1( toPixels( b.getPrevPos() ) );
		
		// Scale
		float d0	= glm::distance( ap0, bp0 );
		float d1	= glm::distance( ap1, bp1 );
		scale		= d0 - d1;
		
		// Rotation
		float a0 = atan2( ap0.y - bp0.y, ap0.x - bp0.x );
		float a1 = atan2( ap1.y - bp1.y, ap1.x - bp1.x );
		rotation = a0 - a1;
		rotation = fmod( rotation + pi, twoPi );
		if ( rotation < 0.0f ) {
			rotation += twoPi;
		}
		rotation -= pi;
	}
	
	// Sort motions
	vector<Motion> motions = {
		{ MotionType_PanX,		abs( panX ) / mPanThreshold.x },
		{ MotionType_PanY,		abs( panY ) / mPanThreshold.y },
		{ MotionType_Rotation,	abs( rotation ) / mRotationThreshold },
		{ MotionType_Scale,		abs( scale ) / mScaleThreshold },
	};
	sort( motions.begin(), motions.end(), []( const Motion& a, const Motion& b ) -> bool
	{
		return a.second > b.second;
	} );
	
	// Apply only most dominate motion
	if ( motions.begin()->second > 1.0f ) {
		switch ( motions.begin()->first ) {
			case MotionType_PanX:
				mPanTarget.x	+= panX * panSpeed.x;
				break;
			case MotionType_PanY:
				mPanTarget.y	+= panY * panSpeed.y;
				break;
			case MotionType_Rotation:
				mRotationTarget = glm::rotate( mRotationTarget, rotation * mRotationSpeed, vec3( 0.0f, 0.0f, 1.0f ) );
				break;
			case MotionType_Scale:
				mScaleTarget	+= scale * mScaleSpeed;
				break;
		}
	}
}

void TouchUi::update()
{	
	mPanTarget		= glm::clamp( mPanTarget, mPanMin, mPanMax );
	mScaleTarget	= glm::clamp( mScaleTarget, mScaleMin, mScaleMax );
	
	mPan			= glm::mix( mPan, mPanTarget, mInterpolationSpeed );
	mRotation		= glm::slerp( mRotation, mRotationTarget, mInterpolationSpeed );
	mScale			= glm::mix( mScale, mScaleTarget, mInterpolationSpeed );
	
	if ( mTapTime > 0.0 && ( app::App::get()->getElapsedSeconds() - mTapTime ) > mTapDelay ) {
		mTap			= false;
		mTapLocation	= vec2( numeric_limits<float>::min() );
		mTapTime		= 0.0;
	}
}
