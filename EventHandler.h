#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include "common.h"
#include "reactor.h"

namespace reactor{

	class EventHandler{
		public:
			EventHandler(handle_t);
			virtual ~EventHandler(){}
			
			virtual void handle_read(){}
			virtual void handle_write(){}
			virtual void handle_error(){}
			virtual handle_t get_handle() const{};
		protected:
			Reactor m_reactor;	//for register handler
			handle_t m_handle;	//for set the handler
			char buf[BUFSIZE];
	};

	class RequestHandler:public EventHandler{
		public:
			RequestHandler(handle_t);
			~EventHandler();
			virtual void handle_read();
			virtual void handle_write();
			virtual void handle_error();


	};

	class ServerHandler : public EventHandler{
		public:
			ServerHandler(handle_t);
			~ServerHandler();
			virtual void handle_read();
			//virtual void handle_write();
			//virtual void handle_error();
		//private:
			//int port;
	}

	class ClientHandler:public EventHandler{
		public:
			ClientHandler(handle_t);
			~ClientHandler();
			virtual void handle_read();
			virtual void handle_write();
			virtual void handle_error();
			void get_buf(char *);
	};
}

#endif //EVENTHANDLER_H
