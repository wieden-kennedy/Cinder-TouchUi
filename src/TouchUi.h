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
	// Creates a TouchUi which connects to \a window touch events with
	// \a signalPriority.
	TouchUi( const ci::app::WindowRef& window = nullptr, int signalPriority = 0 );
	// Creates a TouchUi from \a &rhs.
	TouchUi( const TouchUi &rhs );
	~TouchUi();

	// Copies a TouchUi from \a &rhs.
	TouchUi&				operator=( const TouchUi& rhs );

	// Connects to \a window touch events with \a signalPriority. If no
	// interactive mask exists, one will be created at the size of the window.
	void					connect( const ci::app::WindowRef& window, int signalPriority = 0 );
	// Disconnects from window's touch events.
	void					disconnect();

	// Set \a enable to true to enable touch event processing.
	// Default is true.
	void					enable( bool enable = true );
	// Disables touch event processing.
	void					disable();
	// Returns true if touch event processing is enabled.
	bool					isEnabled() const;

	// Set \a enable to true to contrain interaction to one motion
	// at a time (pan, rotation, or scale). Default is true.
	void					enableConstrainMotion( bool enable = true );
	// Disables motion contraint
	void					disableConstrainMotion();
	// Return true if
	bool					isConstrainMotionEnabled() const;

	// Set \a enable to true to enable pan motion. Default is true.
	void					enablePan( bool enable = true );
	// Disables pan motion.
	void					disablePan();
	// Returns true if pan motion is enabled.
	bool					isPanEnabled() const;
	
	// Set \a enable to true to enable rotate motion. Default is true.
	void					enableRotation( bool enable = true );
	// Disables rotate motion.
	void					disableRotation();
	// Returns true if rotate motion is enabled.
	bool					isRotationEnabled() const;
	
	// Set \a enable to true to enable scale motion. Default is true.
	void					enableScale( bool enable = true );
	// Disables scale motion.
	void					disableScale();
	// Returns true if scale motion is enabled.
	bool					isScaleEnabled() const;
	
	// Set \a enable to true to enable tap gesture. Default is true.
	void					enableTap( bool enable = true );
	// Disables tap gesture.
	void					disableTap();
	// Returns true if tap gesture is enabled.
	bool					isTapEnabled() const;

	// Returns pan position in pixels.
	const ci::vec2&			getPan() const;
	// Return rotation angle in radians.
	float					getRotation() const;
	// Returns scale.
	const ci::vec2&			getScale() const;
	
	// Returns location of tap gesture in pixels.
	const ci::vec2&			getTapPosition() const;
	// Returns location of tap gesture in pixels. Set \a clearTap to true
	// reset tap data.
	const ci::vec2			getTapPosition( bool clearTap );
	// Returns true if a tap gesture has occurred.
	bool					isTapped() const;
	// Returns true if a tap gesture has occurred. Set \a clearTap
	// to true reset tap data.
	bool					isTapped( bool clearTap );
	
	// Returns interpolation speed. This is rate at which motion updates.
	float					getInterpolationSpeed() const;
	// Returns Path2d representing interactive mask.
	const ci::Path2d&		getMask() const;
	// Return maximum number of touchpoints allowed.
	int32_t					getNumTouchPointsMax() const;
	// Return minimum number of touchpoints required.
	int32_t					getNumTouchPointsMin() const;
	
	// Returns maximum pan position allowed in pixels.
	const ci::vec2&			getPanMax() const;
	// Returns minimum pan position allowed in pixels.
	const ci::vec2&			getPanMin() const;
	// Returns pan speed in pixels.
	const ci::vec2&			getPanSpeed() const;
	// Returns pan motion's minimum movement threshold in pixels.
	const ci::vec2&			getPanThreshold() const;
	
	// Returns rotation speed in radians.
	float 					getRotationSpeed() const;
	// Returns rotation's minimum angle threshold in radians.
	float 					getRotationThreshold() const;
	
	// Returns maximum scale allowed.
	const ci::vec2&			getScaleMax() const;
	// Returns minimum scale allowed.
	const ci::vec2&			getScaleMin() const;
	// Returns scale speed.
	const ci::vec2&			getScaleSpeed() const;
	// Returns scale motion's minimum movement threshold in pixels.
	const ci::vec2& 		getScaleThreshold() const;
	
	// Returns time in seconds before a tap gesture is reset.
	double					getTapDelay() const;
	// Returns maximum offset in pixels from touch to lift to qualify a tap.
	float					getTapThreshold() const;
	
	// Returns true if scale is always symmetrical (both axes scale evenly)
	bool					isScaleSymmetryEnabled() const;

	// Disables symmetrical scale. Each axis scales independently.
	void					disableScaleSymmetry();
	// Enables symmetrical scaling. Both axes scales evenly. Default is true.
	void					enableScaleSymmetry( bool enable = true );
	
	// Sets interpolation speed to \a v. This is rate at which motion updates.
	// Values should remain between 0.0 and 1.0. Higher values are more
	// responsive. Lower values are smoother. Default is 0.33.
	void					setInterpolationSpeed( float v );
	
	// Sets interactive mask to \a path. Only touches occurring inside
	// the mask will be processed.
	void					setMask( const ci::Path2d& path );
	// Convenience method for creating an interactive mask from \a bounds.
	void					setMask( const ci::Rectf& bounds );
	// Convenience method for creating an interactive mask from circle at
	// \a center with a radius of \a radius and \a numSegments segments.
	void					setMask( const ci::vec2& center, float radius, size_t numSegments = 12 );
	
	// Limits number of touch points to between \a min and \a max. This is
	// the total number of touch points on the screen; not just within the
	// interactive mask. Default is 1 and numeric_limits<int32_t>::max().
	void					setNumTouchPoints( int32_t min, int32_t max );
	// Limits number of touch points to \a max. This is the total number of
	// touch points on the screen; not just within the interactive mask.
	// Default is numeric_limits<int32_t>::max().
	void					setNumTouchPointsMax( int32_t v );
	// Requires \a min number of touch points. This is the total number of
	// touch points on the screen; not just within the interactive mask.
	// Default is 1.
	void					setNumTouchPointsMin( int32_t v );
	
	// Sets pan position in pixels to \a v. Default is vec2( 0.0f ).
	// Set \a interpolate to false to update value immediately. Default is true.
	void					setPan( const ci::vec2& v, bool interpolate = true );
	// Sets maximum pan position in pixels to \a v. Default is
	// vec2( numeric_limits<float>::max() ).
	void					setPanMax( const ci::vec2& v );
	// Sets minimum pan position in pixels to \a v. Default is
	// vec2( -numeric_limits<float>::max() ).
	void					setPanMin( const ci::vec2& v );
	// Sets pan speed in pixels to \a v. Default is vec2( 1.0f ).
	void					setPanSpeed( const ci::vec2& v );
	// Sets minimum movement threshold for pan in pixels to \a v.
	// Default is vec2( 1.0f ).
	void					setPanThreshold( const ci::vec2& v );
	
	// Sets rotation angle in radians to \a v.  Default is 0.0f.
	// Set \a interpolate to false to update value immediately. Default is true.
	void					setRotation( float v, bool interpolate = true );
	// Sets rotation speed to \a v. This value is multiplied by
	// an interaction's rotation angle. The default is -2.5f.
	void 					setRotationSpeed( float v );
	// Sets threshold for minimum rotation angle. Default is 0.005f.
	void 					setRotationThreshold( float v );
	
	// Sets scale to \a v. Default is vec2( 1.0 ).
	// Set \a interpolate to false to update value immediately. Default is true.
	void					setScale( const ci::vec2& v, bool interpolate = true );
	// Sets maximum scale to \a v. Default is vec2( numeric_limits<float>::max() ).
	void					setScaleMax( const ci::vec2& v );
	// Sets minimum scale to \a v. Default is vec2( 0.0f ).
	void					setScaleMin( const ci::vec2& v );
	// Sets scale speed to \a v. This value is multipled by the scale distance
	// in pixels. Default is vec2( 0.0067f ).
	void					setScaleSpeed( const ci::vec2& v );
	// Sets minimum movement threshold for pan in pixels to \a v.
	// Default is vec2( 1.0f ).
	void 					setScaleThreshold( const ci::vec2& v );
	
	// Sets delay in seconds before tap gesture is reset to \a v. Default is 0.07.
	void					setTapDelay( double v );
	// Sets maximum offset in pixels from touch to lift to qualify a tap.
	// Default is 15.0f.
	void					setTapThreshold( float v );
	
	// Resets motion values.
	// Set \a pan to true to set pan to vec2( 0.0 )
	// Set \a rotation to true to set rotation to 0.0
	// Set \a scale to true to set scale to vec2( 1.0 )
	// Set \a interpolate to false to update values immediately. Default is true.
	void					zero( bool pan = true, bool rotation = true, bool scale = true, bool interpolate = true );
	
	// Returns delay in seconds before records touches are cleared.
	double					getTouchDelay() const;
	// Sets delay in seconds before records touches are cleared to \a v.
	// Default is 0.07.
	void					setTouchDelay( double v );
	// Returns touches, if any, which contributed to recent motion or gestures.
	const std::vector<ci::app::TouchEvent::Touch>& getTouches();
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
	
	std::vector<ci::app::TouchEvent::Touch>	mTouches;
	void					pushTouch( const ci::app::TouchEvent::Touch& touch );
	
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

	bool					mEnabledConstrain;
	bool					mEnabledPan;
	bool					mEnabledRotation;
	bool					mEnabledScale;
	bool					mEnabledTap;
	
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
	double					mTapDelay;
	ci::vec2				mTapPosition;
	double					mTapTime;
	float					mTapThreshold;
	double					mTouchDelay;
	double					mTouchTime;
	void					resetTap();
	
	bool					isEventValid( const ci::app::TouchEvent& event ) const;
	float					wrapAngle( float v );
};
 