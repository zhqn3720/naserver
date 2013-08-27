#ifndef _TANIC_EVENTHANDLER_HPP
#define _TANIC_EVENTHANDLER_HPP

namespace tanic
{
	class EventHandler
	{
	public:
		virtual ~EventHandler() {}
		//回调接口：handleEvent()
		virtual void handleEvent() = 0;
		virtual int getHandle() = 0;
		virtual void setReventType(int) = 0;
		virtual void setIndex(int) = 0;
		virtual int index() = 0;
	};
}
#endif
