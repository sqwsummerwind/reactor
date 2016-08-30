#include "EventHandler.h"
#include "common.h"
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>

namespace reactor{
	
	EventHandler::EventHandler(handle_t handle):m_handle(handle)
	{
		m_reactor = *Reactor::get_instance();
		memset(buf, 0, sizeof(buf));
	}

	handle_t EventHandler::get_handle() const{
		return m_handle;
	}

	RequestHandler::RequestHandler(handle_t handle):
	EventHandler(handle)
	{}

	void RequestHandler::handle_read(){
		int readn;
		//memset(read_buf, 0, sizeof(read_buf));
		if(readn = read(m_handle, buf, sizeof(buf))<0){
			if(errno == ECONNRESET){
				//connection reset by client
				m_reactor.unregister_handler(this);
				close(m_handle);
				delete this;
			}else{
				printerr("read error");
			}
		}else if(readn==0){
			//connection closed by client
			m_reactor.unregister_handler(this);
			close(m_handle);
			delete this;
		}else{
			
			printf("get data from,fd:%d\n%s\n", m_handle, buf);
			m_reactor.register_handler(this, WRITEEVENT);
		}
	}

	void RequestHandler::handle_write(){
		int writen;
		//get the time of system
		time_t timep;
		time(&timep);
		char *timestr = ctime(&timep);

		strcat(buf, timestr);

		writen = write(m_handle, buf, strlen(buf));
		if(writen == -1){
			printerr("write");
		}else{
			printf("send to client,fd:%d\n", m_handle);
			m_reactor.register_handler(this, READEVENT);
		}

	}

	void RequestHandler::handle_error(){
		perror("get error,client %d closed", m_handle);
		close(m_handle);
		m_reactor.unregister_handler(this);
		delete this;
	}

	ServerHandler::ServerHandler(handle_t handle):EventHandler(handle){}
	
	void ServerHandler::handle_read(){
		struct sockaddr_in cliaddr;
		handle_t clifd = accept(m_handle, &cliaddr, &sizeof(cliaddr));
		if(clifd == -1){
			perror("accept");
		}else{
			RequestHandler *req_handler = new RequestHandler(clifd);
			m_reactor.register_handler(req_handler, READEVENT);
			m_reactor.register_handler(this, READEVENT);
		}
	}

	handle_t ServerHandler::get_handle(){
		return m_handle;
	}

	ClientHandler::ClientHandler(handle_t handle):EventHandler(handle){}

	void ClientHandler::handle_read(){
		int readn;
		readn = read(m_handle, buf, sizeof(buf));
		if(readn<0){
			if(errno == ECONNRESET){
				m_reactor.unregister_handler(this);
				close(m_handle);
				delete this;
			}else{
				printerr("clientHandler handle_read");
			}
		}else if(readn == 0){
			m_reactor.unregister_handler(this);
			close(m_handle);
			delete this;
		}else{
			//judge if is the stdin
			if(m_handle != 0){
				printf("data from client:%s\n",buf);
				m_reactor.register_handler(this, WRITEEVENT);
			}else{
				//stdin
				m_reactor.register_handler(this, READEVENT);
			}

		}
	}//end handle_read

	void ClientHandler::get_buf(char *databuf){
		strcpy(databuf, buf);
	}

	void ClientHandler::handle_write(){
		
		char databuf[BUFSIZE];
		memset(databuf, 0, BUFSIZE);
		ClientHandler *stdin_handler = m_reactor.get_eventhandler(0);
		if(stdin_handler == NULL){
			perror("get event handler error");
			return;
		}

		stdin_handler->get_buf(databuf);
		if(strlen(databuf)==0){
			m_reactor.register_handler(this, READEVENT);
			return;
		}

		int writen = write(m_handle, databuf, strlen(databuf));

		if(writen == -1){
			perror("write at clienthandler handle write,fd:%d\n",m_handle);
		}

		m_reactor.register_handler(this, READEVENT);

	}
}
