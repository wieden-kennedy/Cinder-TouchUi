#pragma once

#include "cinder/Vector.h"
#include "cinder/app/TouchEvent.h"
#include "cinder/app/Window.h"

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
	float					getScale() const;
	const ci::vec2&			getTapLocation() const;
	
	float					getInterpolationSpeed() const;
	const ci::vec2&			getPanMax() const;
	const ci::vec2&			getPanMin() const;
	const ci::vec2&			getPanSpeed() const;
	const ci::vec2&			getPanThreshold() const;
	float 					getRotationSpeed() const;
	float 					getRotationThreshold() const;
	float					getScaleMax() const;
	float					getScaleMin() const;
	float					getScaleSpeed() const;
	float 					getScaleThreshold() const;
	float					getTapThreshold() const;
	bool					isTapped() const;

	void					setInterpolationSpeed( float v );
	void					setPanMax( const ci::vec2& v );
	void					setPanMin( const ci::vec2& v );
	void					setPanSpeed( const ci::vec2& v );
	void					setPanThreshold( const ci::vec2& v );
	void 					setRotationSpeed( float v );
	void 					setRotationThreshold( float v );
	void					setScaleMax( float v );
	void					setScaleMin( float v );
	void					setScaleSpeed( float v );
	void 					setScaleThreshold( float v );
	void					setTapDelay( double v );
	void					setTapThreshold( float v );
	void					zero();
protected:
	enum : size_t
	{
		MotionType_PanX,
		MotionType_PanY,
		MotionType_Rotation,
		MotionType_Scale
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
	float					mScale;
	float					mScaleMax;
	float					mScaleMin;
	float					mScaleSpeed;
	float					mScaleTarget;
	float 					mScaleThreshold;
	bool					mTap;
	double					mTapDelay;
	ci::vec2				mTapLocation;
	double					mTapTime;
	float					mTapThreshold;
};
 