#include "Animator.h"

namespace core
{

	Animator::Animator(ANIMATION_MODE _mode, float _upperBound) :
			mode(_mode),
			upperBound(_upperBound)
	{
		reset();
	};


	Animator::~Animator()
	{};


	void Animator::progress(float _value)
	{
		currentValue += _value;

		switch (mode)
		{
			case AM_ONCE:
			{
				t = (currentValue < upperBound) ? (currentValue / upperBound) : 1.0f;
				break;
			}
			case AM_REPEAT:
			{
				t = std::fmod(currentValue, upperBound) / upperBound;
				break;
			}
			case AM_LOOP:
			{
				float tmp = currentValue / upperBound;
				t = ((int)(tmp) % 2 == 0) ? std::fmod(tmp, 1.0f) : (1.0f - std::fmod(tmp, 1.0f));
				break;
			}
		}

		interpolatedValue = _progressImpl(t);
		interpolatedValue = std::clamp(interpolatedValue, 0.0f, 1.0f);

		for (unsigned int i = 0, iEnd = listeners.size(); i < iEnd; ++i)
		{
			listeners[i]->progress(interpolatedValue);
		};
	};


	void Animator::registerListener(Listener *_listener)
	{
		assert(std::find(listeners.begin(), listeners.end(), _listener) == listeners.end() && "listener already registered");
		listeners.push_back(_listener);
	};


	void Animator::unregisterListener(Listener *_listener)
	{
		ListenerList::iterator it = std::find(listeners.begin(), listeners.end(), _listener);
		assert(it != listeners.end() && "listener not registered");
		std::swap(*it, listeners.back());
		listeners.pop_back();
	};


	float Animator::getValue() const
	{
		return t;
	};


	void Animator::setMode(ANIMATION_MODE _mode)
	{
		mode = _mode;
	};


	void Animator::setUpperBound(float _upperBound)
	{
		upperBound = _upperBound;
	};


	void Animator::reset()
	{
		currentValue = 0.0f;
		t = 0.0f;
		interpolatedValue = 0.0f;
		_resetImpl();
	};

}