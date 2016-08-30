#ifndef EVENTDEMULTIPLEXER_H
#define EVENTDEMULTIPLEXER_H

#include "EventHandler.h"
#include "common.h"
using namespace std;

namespace reactor{
	class EventDemultiplexer{
		public:
			virtual ~EventDemultiplexer(){};
			virtual void wait_event(map<handle_t, EventHandler *> *handlers);
			virtual void request_event(handle_t handle, event_t et);
			virtual void unrequest_event(handle_t handle);
			
	};

	class EpollDemultiplexer{
		public:
			EpollDemultiplexer();
			~EpollDemultiplexer();
			virtual void wait_event(map<handle_t, EventHandler *> *handlers);
			virtual void request_event(handle_t handle, event_t et);
			virtual void unrequest_event(handle_t handle);
		
		private:
			int m_epoll_fd;
			int m_fd_num;
	};
}


#endif 
