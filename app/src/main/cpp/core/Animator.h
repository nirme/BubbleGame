#pragma once

#include <algorithm>
#include <cmath>
#include <cassert>
#include <vector>

#include "ControllerValues.h"



namespace core
{
	class Animator
	{
	public:
		class Listener
		{
		public:
			virtual void progress(float _value) = 0;
			virtual ~Listener() {};
		};

		enum ANIMATION_MODE
		{
			AM_ONCE = 0,
			AM_REPEAT = 1,
			AM_LOOP = 2,
		};

	protected:
		ANIMATION_MODE mode;

		float upperBound;
		float currentValue;
		float t;

		float interpolatedValue;

		typedef std::vector<Listener*> ListenerList;
		ListenerList listeners;

		virtual float _progressImpl(float _t) = 0;
		virtual void _resetImpl() {};

	public:

		Animator(ANIMATION_MODE _mode = AM_ONCE, float _upperBound = 1.0f);
		virtual ~Animator();

		void progress(float _value);
		void registerListener(Listener *_listener);
		void unregisterListener(Listener *_listener);
		float getValue() const;
		void setMode(ANIMATION_MODE _mode);
		void setUpperBound(float _upperBound);
		void reset();
	};

}