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

#pragma once

#include "cinder/app/TouchEvent.h"
#include "cinder/app/Window.h"
#include "cinder/Quaternion.h"
#include "cinder/Path2d.h"

class TouchUi
{
public:
	TouchUi( const ci::app::WindowRef& window = nullptr, int signalPriority = 0 );
	TouchUi( const TouchUi &rhs );
	~TouchUi();

	TouchUi&				operator=( const TouchUi& rhs );

	void					connect( const ci::app::WindowRef& window, int signalPriority = 0 );
	void					disconnect();
	void					enable( bool enable = true );
	void					disable();
	bool					isEnabled() const;
	
	const ci::vec2&			getPan() const;
	float					getRotation() const;
	const ci::vec2&			getScale() const;
	const ci::vec2&			getTapPosition() const;
	const ci::vec2			getTapPosition( bool clearTap );
	bool					isTapped() const;
	bool					isTapped( bool clearTap );
	
	float					getInterpolationSpeed() const;
	const ci::Path2d&		getMask() const;
	int32_t					getNumTouchPointsMax() const;
	int32_t					getNumTouchPointsMin() const;
	const ci::vec2&			getPanMax() const;
	const ci::vec2&			getPanMin() const;
	const ci::vec2&			getPanSpeed() const;
	const ci::vec2&			getPanThreshold() const;
	float 					getRotationSpeed() const;
	float 					getRotationThreshold() const;
	const ci::vec2&			getScaleMax() const;
	const ci::vec2&			getScaleMin() const;
	const ci::vec2&			getScaleSpeed() const;
	const ci::vec2& 		getScaleThreshold() const;
	float					getTapThreshold() const;
	bool					isScaleSymmetryEnabled() const;

	void					disableScaleSymmetry();
	void					enableScaleSymmetry( bool enable = true );
	void					setInterpolationSpeed( float v );
	void					setMask( const ci::Path2d& path );
	void					setMask( const ci::Rectf& bounds );
	void					setMask( const ci::vec2& center, float radius, size_t numSegments = 12 );
	void					setNumTouchPoints( int32_t min, int32_t max );
	void					setNumTouchPointsMax( int32_t v );
	void					setNumTouchPointsMin( int32_t v );
	void					setPan( const ci::vec2& v );
	void					setPanMax( const ci::vec2& v );
	void					setPanMin( const ci::vec2& v );
	void					setPanSpeed( const ci::vec2& v );
	void					setPanThreshold( const ci::vec2& v );
	void					setRotation( float v );
	void 					setRotationSpeed( float v );
	void 					setRotationThreshold( float v );
	void					setScale( const ci::vec2& v );
	void					setScaleMax( const ci::vec2& v );
	void					setScaleMin( const ci::vec2& v );
	void					setScaleSpeed( const ci::vec2& v );
	void 					setScaleThreshold( const ci::vec2& v );
	void					setTapDelay( double v );
	void					setTapThreshold( float v );
	void					zero( bool pan = true, bool rotation = true, bool scale = true );
protected:
	enum : size_t
	{
		MotionType_PanX,
		MotionType_PanY,
		MotionType_Rotation,
		MotionType_ScaleX, 
		MotionType_ScaleY
	} typedef MotionType;
	typedef std::pair<MotionType, float> Motion;
	
	void					touchesBegan( ci::app::TouchEvent& event );
	void					touchesEnded( ci::app::TouchEvent& event );
	void					touchesMoved( ci::app::TouchEvent& event );
	void					update();

	ci::signals::Connection	mConnectionTouchesBegan;
	ci::signals::Connection	mConnectionTouchesEnded;
	ci::signals::Connection	mConnectionTouchesMoved;
	ci::signals::Connection	mConnectionUpdate;
	
	bool					mEnabled;
	int32_t					mSignalPriority;
	ci::app::WindowRef		mWindow;
	
	float					mInterpolationSpeed;
	ci::Path2d				mMask;
	int32_t					mNumTouchPointsMax;
	int32_t					mNumTouchPointsMin;
	ci::vec2				mPan;
	ci::vec2				mPanMax;
	ci::vec2				mPanMin;
	ci::vec2				mPanSpeed;
	ci::vec2				mPanTarget;
	ci::vec2				mPanThreshold;
	ci::quat				mRotation;
	float 					mRotationSpeed;
	ci::quat				mRotationTarget;
	float 					mRotationThreshold;
	ci::vec2				mScale;
	ci::vec2				mScaleMax;
	ci::vec2				mScaleMin;
	ci::vec2				mScaleSpeed;
	bool					mScaleSymmetry;
	ci::vec2				mScaleTarget;
	ci::vec2 				mScaleThreshold;
	bool					mTap;
	double					mTapDelay;
	ci::vec2				mTapPosition;
	double					mTapTime;
	float					mTapThreshold;
	void					resetTap();

	bool					isEventValid( const ci::app::TouchEvent& event ) const;
	float					wrapAngle( float v );
};
 