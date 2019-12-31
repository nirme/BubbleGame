#include "Controller.h"


namespace core
{

	class FrameTimeControllerValue : public ControllerValue
	{
	protected:
		float frameTime;
		float elapsedTime;

		float timeSpeed;

	public:
		FrameTimeControllerValue(float _timeSpeed = 1.0f) :
			ControllerValue(),
			timeSpeed(_timeSpeed),
			elapsedTime(0.0f),
			frameTime(0.0f)
		{};


		float getElapsedTime()
		{
			return elapsedTime;
		};

		float getTimeSpeed()
		{
			return timeSpeed;
		};

		void setTimeSpeed(float _timeSpeed)
		{
			timeSpeed = _timeSpeed;
		};


		virtual float get() const
		{
			return frameTime;
		};

		virtual void set(float _value)
		{
			frameTime = timeSpeed * _value;
			elapsedTime += frameTime;
		};
	};

	typedef std::shared_ptr<FrameTimeControllerValue> SharedFrameTimeControllerValuePtr;

}
