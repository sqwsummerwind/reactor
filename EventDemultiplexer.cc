#include "common.h"
#include "EventDemultiplexer.h"
#include <sys/epoll.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector.h>

namespace reactor{

	EpollDemultiplexer::EpollDemultiplexer():m_fd_num(0)
	{
		m_epoll_fd = epoll_create1(0);
		if(m_epoll_fd == -1){
			printerr("epoll_create error:");
			//exit(EXIT_FAILURE);
		}
	}

	EpollDemultiplexer::~EpollDemultiplexer(){
		close(m_epoll_fd);
	}

	//call epoll_ctl to add the handle
	void EpollDemultiplexer::request_event(handle_t handle, event_t et){
		struct epoll_event ev;	//the object linked to the file descriptor
		ev.data.fd = handle;
		
		if(et & READEVENT){
			//the associated file is available for read
			ev.events |= EPOLLIN;	
		}
		if(et & WRITEEVENT){
			//the associated file is available for write
			ev.events |= EPOLLOUT;
		}
		//after the event is pulled out with epoll_wait
		//the associated file descriptor is disabled 
		//and must call epoll_ctl with EPOLL_CTL_MOD
		ev |= EPOLLONESHOT;

		if(epoll_ctl(m_epoll_fd, EPOLL_CTL_MOD, handle, &ev) == -1){
			//handle is the first time set to epoll
			if(errno == ENOENT){
				if(epoll_ctl(e_epoll_fd, EPOLL_CTL_ADD, handle, &ev) == -1){
					printerr("epoll_ctl(add)");
					//exit(EXIT_FAILURE);
				}
				m_fd_num++;
			}else{
				printerr("epoll_ctl(mod)");
				//exit(EXIT_FAILURE);
			}
		}

	}//end request_event

	//call epoll_ctl to delete handle
	void EpollDemultiplexer::unrequest_event(handle_t handle){
		
		if(epoll_ctl(m_epoll_fd, EPOLL_CTL_DEL, handle, NULL) == -1){
			printerr("epoll_ctl(del)");
		}
		m_fd_num--;
	}

	//call epoll_wait to wait for the handles available
	void EpollDemultiplexer::wait_event(map<handle_t, EventHandler*> *handlers){
		//store the information return, includes handle ready for operation
		struct epoll_event ep_events[m_fd_num];

		memset(events, 0, sizeof(ep_events));

		int nfdr = epoll_wait(m_epoll_fd, ep_events, m_fd_num, -1);

		if(nfdr == -1){
			printerr("epoll_wait");
		}else if(nfdr == 0){
			return;
		}

		for(int index = 0; index < nfdr; index++){
			
			handle_t handle = ep_events[index].data.fd;
			event_t ev = ep_events[index].events;
			
			if(handlers->find(handle) == handlers->end()){
				printerr("handle not in map");
			}

			//error happened
			if(ev & EPOLLERR || ev & EPOLLHUP){
				(*handlers)[handle]->handle_error(); 
			}else{
			
				//read ready
				if(ev & EPOLLIN){
					(*handlers)[handle]->handle_read();
				}
				//write ready
				if(ev&EPOLLOUT){
					(*handlers)[handle]->handle_write();
				}

			}

		}//end for
		return;
	}//end wait_event
}
