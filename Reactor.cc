#include "Reactor.h"
#include <stdio.h>

using namespace std;

namespace reactor{
	
	Reactor *Reactor::m_reactor = NULL;
	Reactor::GC Reactor::gc;

	Reactor *Reactor::get_instance(){
		if(m_reactor == NULL){
			//add locl here
			if(m_reactor == NULL){
				m_reactor = new Reactor();
			}
		}

		return m_reactor;
	}

	void Reactor::register_handler(EventHandler *handler, event_t evt){
		struct handle_struct handle = handler->get_handle();
		//first register
		if(m_handlers.find(handle)==m_handlers.end()){
			m_handlers[handle] = handler;
		}
		m_demultiplexer->request_event(handle, evt);
	}
	
	void Reactor::unregister_handler(EventHandler *handler){
		handle_t handle = handler->get_handle();
		if(m_handlers.find(handle) != m_handlers.end()){
			m_handlers.erase(handle);
			//delete handler;
			m_demultiplexer->unrequest_event(handle);
		}
		printf("handler already unregister\n");
	}

	void Reactor::handle_events(){
		m_demultiplexer->wait_event(&m_handlers);
	}

	EventHandler *Reactor::get_eventhandler(handle_t handle){
		if(m_handlers.find(handle)!=m_handlers.end()){
			return m_handlers[handle];
		}else{
			return NULL;
		}
	}


}

