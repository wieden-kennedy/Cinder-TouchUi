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

#include "TouchUi.h"
#include "cinder/app/App.h"

using namespace ci;
using namespace ci::app;
using namespace std;

TouchUi::TouchUi( const WindowRef& window, int signalPriority )
{
	mEnabled			= true;
	mInterpolationSpeed	= 0.33f;
	mNumTouchPointsMax	= numeric_limits<int32_t>::max();
	mNumTouchPointsMin	= 1;
	mPan				= vec2( 0.0f );
	mPanMax				= vec2( numeric_limits<float>::max() );
	mPanMin				= vec2( -numeric_limits<float>::max() );
	mPanSpeed			= vec2( 0.0067f );
	mPanTarget			= mPan;
	mPanThreshold		= toPixels( vec2( 1.0f ) );
	mRotationSpeed		= -2.5f;
	mRotationThreshold	= 0.005f;
	mScale				= vec2( 1.0f );
	mScaleMax			= vec2( numeric_limits<float>::max() );
	mScaleMin			= vec2( 0.0f );
	mScaleSpeed			= vec2( 0.0067f );
	mScaleSymmetry		= true;
	mScaleTarget		= mScale;
	mScaleThreshold		= toPixels( vec2( 1.0f ) );
	mTap				= false;
	mTapDelay			= 0.07;
	mTapPosition		= vec2( numeric_limits<float>::min() );
	mTapTime			= 0.0;
	mTapThreshold		= toPixels( 15.0f );

	if ( window != nullptr ) {
		setMask( Area( ivec2( 0 ), window->getSize() ) );
	}
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
	mMask				= rhs.mMask;
	mNumTouchPointsMax	= rhs.mNumTouchPointsMax;
	mNumTouchPointsMin	= rhs.mNumTouchPointsMin;
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
	mScaleSymmetry		= rhs.mScaleSymmetry;
	mScaleTarget		= rhs.mScaleTarget;
	mScaleThreshold		= rhs.mScaleThreshold;
	mSignalPriority		= rhs.mSignalPriority;
	mTap				= rhs.mTap;
	mTapDelay			= rhs.mTapDelay;
	mTapPosition		= rhs.mTapPosition;
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
		if ( mMask.empty() ) {
			setMask( Area( ivec2( 0 ), window->getSize() ) );
		}
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

const vec2& TouchUi::getScale() const
{
	return mScale;
}

const vec2& TouchUi::getTapPosition() const
{
	return mTapPosition;
}

const vec2 TouchUi::getTapPosition( bool clearTapPosition )
{
	vec2 tapPosition = mTapPosition;
	if ( clearTapPosition ) {
		resetTap();
	}
	return tapPosition;
}

bool TouchUi::isTapped() const
{
	return mTap;
}

bool TouchUi::isTapped( bool clearTap )
{
	bool tap = mTap;
	if ( clearTap ) {
		resetTap();
	}
	return tap;
}

float TouchUi::getInterpolationSpeed() const
{
	return mInterpolationSpeed;
}

const Path2d& TouchUi::getMask() const
{
	return mMask;
}

int32_t TouchUi::getNumTouchPointsMax() const
{
	return mNumTouchPointsMax;
}

int32_t TouchUi::getNumTouchPointsMin() const
{
	return mNumTouchPointsMin;
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

const vec2& TouchUi::getScaleMax() const
{
	return mScaleMax;
}

const vec2& TouchUi::getScaleMin() const
{
	return mScaleMin;
}

const vec2& TouchUi::getScaleSpeed() const
{
	return mScaleSpeed;
}

const vec2& TouchUi::getScaleThreshold() const
{
	return mScaleThreshold;
}

float TouchUi::getTapThreshold() const
{
	return mTapThreshold;
}

bool TouchUi::isScaleSymmetryEnabled() const
{
	return mScaleSymmetry;
}

void TouchUi::disableScaleSymmetry()
{
	mScaleSymmetry = false;
}

void TouchUi::enableScaleSymmetry( bool enable )
{
	mScaleSymmetry = enable;
}

void TouchUi::setInterpolationSpeed( float v )
{
	mInterpolationSpeed = v;
}

void TouchUi::setMask( const Path2d& path )
{
	mMask = path;
}

void TouchUi::setMask( const Rectf& bounds )
{
	mMask.clear();
	mMask.moveTo( toPixels( bounds.getUpperLeft() ) );
	mMask.lineTo( toPixels( bounds.getUpperRight() ) );
	mMask.lineTo( toPixels( bounds.getLowerRight() ) );
	mMask.lineTo( toPixels( bounds.getLowerLeft() ) );
	mMask.close();
}

void TouchUi::setMask( const vec2& center, float radius, size_t numSegments )
{
	mMask.clear();
	float t			= 0.0f;
	const float d	= ( 1.0f / (float)numSegments ) * (float)M_PI * 2.0f;
	for ( size_t i = 0; i < numSegments; ++i, t += d ) {
		vec2 v = toPixels( center + vec2( glm::cos( t ), glm::sin( t ) ) * radius );
		if ( i == 0 ) {
			mMask.moveTo( v );
		} else {
			mMask.lineTo( v );
		}
	}
	mMask.close();
}

void TouchUi::setNumTouchPoints( int32_t min, int32_t max )
{
	mNumTouchPointsMax = max;
	mNumTouchPointsMin = min;
}

void TouchUi::setNumTouchPointsMax( int32_t v )
{
	mNumTouchPointsMax = v;
}

void TouchUi::setNumTouchPointsMin( int32_t v )
{
	mNumTouchPointsMin = v;
}

void TouchUi::setPan( const vec2& v )
{
	mPanTarget = v;
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

void TouchUi::setRotation( float v )
{
	mRotationTarget.z = wrapAngle( v );
}

void TouchUi::setRotationSpeed( float v )
{
	mRotationSpeed = v;
}

void TouchUi::setRotationThreshold( float v )
{
	mRotationThreshold = v;
}

void TouchUi::setScale( const vec2& v )
{
	mScaleTarget = v;
}

void TouchUi::setScaleMax( const vec2& v )
{
	mScaleMax = v;
	if ( mScaleMin.x > mScaleMax.x ) {
		swap( mScaleMin.x, mScaleMax.x );
	}
	if ( mScaleMin.y > mScaleMax.y ) {
		swap( mScaleMin.y, mScaleMax.y );
	}
}

void TouchUi::setScaleMin( const vec2& v )
{
	mScaleMin = v;
	if ( mScaleMin.x > mScaleMax.x ) {
		swap( mScaleMin.x, mScaleMax.x );
	}
	if ( mScaleMin.y > mScaleMax.y ) {
		swap( mScaleMin.y, mScaleMax.y );
	}
}

void TouchUi::setScaleSpeed( const vec2& v )
{
	mScaleSpeed = v;
}

void TouchUi::setScaleThreshold( const vec2& v )
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

void TouchUi::zero( bool pan, bool rotation, bool scale )
{
	if ( pan ) {
		setPan( vec2( 0.0f ) );
	}
	if ( rotation ) {
		setRotation( 0.0f );
	}
	if ( scale ) {
		setScale( vec2( 1.0f ) );
	}
	resetTap();
}

void TouchUi::touchesBegan( TouchEvent& event )
{
	if ( !isEventValid( event ) ) {
		return;
	}

	if ( event.getTouches().size() == 1 ) {
		const vec2 tap = toPixels( event.getTouches().begin()->getPos() );
		if ( mMask.contains( tap ) ) {
			mTapPosition	= tap;
			mTapTime		= app::App::get()->getElapsedSeconds();
		}
	}
}

void TouchUi::touchesEnded( app::TouchEvent& event )
{
	if ( !isEventValid( event ) ) {
		return;
	}

	if ( mTapTime > 0.0 && event.getTouches().size() == 1 ) {
		const vec2 tap( toPixels( event.getTouches().begin()->getPos() ) );
		if ( mMask.contains( tap ) && glm::distance( tap, mTapPosition ) < mTapThreshold ) {
			mTap			= true;
			mTapPosition	= tap;
			mTapTime		= app::App::get()->getElapsedSeconds();
		}
	}
}

void TouchUi::touchesMoved( app::TouchEvent& event )
{
	if ( !isEventValid( event ) ) {
		return;
	}

	// End tap if multiple touches or location has moved too far
	const size_t numTouches = event.getTouches().size(); 
	if ( numTouches > 1 ) {
		resetTap();
	} else {
		const vec2 tap( toPixels( event.getTouches().begin()->getPos() ) );
		if ( !mMask.contains( tap ) || glm::distance( tap, mTapPosition ) > mTapThreshold ) {
			resetTap();
		}
	}
	
	const vec2 panSpeed( 
		mPanSpeed.x * pow( ( mScaleMax.x + mScaleMin.x ) - mScale.x, 0.0002f ), 
		mPanSpeed.y * pow( ( mScaleMax.y + mScaleMin.y ) - mScale.y, 0.0002f ) );

	float panX		= 0.0f;
	float panY		= 0.0f;
	float scaleX	= 0.0f;
	float scaleY	= 0.0f;
	float rotation	= 0.0f;

	// Pan
	if ( numTouches > 0 ) {
		const TouchEvent::Touch& touch = *event.getTouches().begin();
		const vec2 a( toPixels( touch.getPos() ) );
		const vec2 b( toPixels( touch.getPrevPos() ) );
		if ( mMask.contains( b ) ) {
			panX = a.x - b.x;
			panY = a.y - b.y;
		}
	}
	
	// Multi-touch
	if ( numTouches > 1 && mNumTouchPointsMax > 1 ) {
		const TouchEvent::Touch& a = *event.getTouches().begin();
		const TouchEvent::Touch& b = *( event.getTouches().begin() + 1 );
		const vec2 ap0( toPixels( a.getPos() ) );
		const vec2 ap1( toPixels( a.getPrevPos() ) );
		const vec2 bp0( toPixels( b.getPos() ) );
		const vec2 bp1( toPixels( b.getPrevPos() ) );
		if ( mMask.contains( bp0 ) && mMask.contains( bp1 ) ) {

			// Scale
			float dx0 = glm::distance( ap0.x, bp0.x );
			float dx1 = glm::distance( ap1.x, bp1.x );
			scaleX = dx0 - dx1;

			float dy0 = glm::distance( ap0.y, bp0.y );
			float dy1 = glm::distance( ap1.y, bp1.y );
			scaleY = dy0 - dy1;

			// Rotation
			float a0 = atan2( ap0.y - bp0.y, ap0.x - bp0.x );
			float a1 = atan2( ap1.y - bp1.y, ap1.x - bp1.x );
			rotation = wrapAngle( a0 - a1 );
		}
	}
	
	// Sort motions
	vector<Motion> motions = {
		{ MotionType_PanX,		abs( panX ) / mPanThreshold.x },
		{ MotionType_PanY,		abs( panY ) / mPanThreshold.y },
		{ MotionType_Rotation,	abs( rotation ) / mRotationThreshold },
		{ MotionType_ScaleX,	abs( scaleX ) / mScaleThreshold.x },
		{ MotionType_ScaleY,	abs( scaleY ) / mScaleThreshold.y },
	};
	sort( motions.begin(), motions.end(), []( const Motion& a, const Motion& b ) -> bool
	{
		return a.second > b.second;
	} );
	
	// Apply only most dominate motion
	if ( motions.begin()->second > 1.0f ) {
		switch ( motions.begin()->first ) {
			case MotionType_PanX:
				mPanTarget.x		+= panX * panSpeed.x;
				break;
			case MotionType_PanY:
				mPanTarget.y		+= panY * panSpeed.y;
				break;
			case MotionType_Rotation:
				mRotationTarget.z	+= rotation * mRotationSpeed;
				break;
			default:
				if ( mScaleSymmetry ) {
					mScaleTarget	+= vec2( ( scaleX * mScaleSpeed.x + scaleY * mScaleSpeed.y ) * 0.5f );
				} else {
					mScaleTarget	+= vec2( scaleX * mScaleSpeed.x, scaleY * mScaleSpeed.y );
				}
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
		resetTap();
	}
}

void TouchUi::resetTap()
{
	mTap			= false;
	mTapPosition	= vec2( numeric_limits<float>::min() );
	mTapTime		= 0.0;
}

bool TouchUi::isEventValid( const app::TouchEvent& event ) const
{
	if ( !mEnabled ) {
		return false;
	}
	const int32_t numTouches = (int32_t)event.getTouches().size();
	if ( numTouches < mNumTouchPointsMin || numTouches > mNumTouchPointsMax ) {
		return false;
	}
	return true;
}

float TouchUi::wrapAngle( float v )
{
	static const float pi		= (float)M_PI;
	static const float twoPi	= pi * 2.0f;

	v = fmod( v + pi, twoPi );
	if ( v < 0.0f ) {
		v += twoPi;
	}
	v -= pi;
	return v;
}
