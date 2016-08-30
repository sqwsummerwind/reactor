#ifndef REACTOR_H
#define REACTOR_H
#include "common.h"
#include "EventHandler.h"

using namespace std;

namespace reactor{
	
	class Reactor{
		public:
			~Reactor(){
				delete m_demultiplexer;
			}
			void register_handler(EventHandler *handler, event_t evt);
			void unregister_handler(EventHandler *handler);
			void handle_events();
			EventHandler *get_eventhandler(handle_t);
			static Reactor *get_instance();
			
		private:
			//for delete the instance of reactor(m_reactor)
			class GC{
				//GC(){}
				~GC(){
					delete m_reactor;
				}

			}
			static GC gc;

			Reactor(){
				m_demultiplexer = new EpollDemultiplexer();
			}

			map<handle_t, EventHandler *> m_handlers;
			static Reactor *m_reactor;
			EventDemultiplexer *m_demultiplexer;

	}
};

#endif
