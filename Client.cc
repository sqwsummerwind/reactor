#include "Reactor.h"


int main(int argc, char **argv){
	int port;

	if(argc != 3){
		printf("usage: %s ip port",argv[0]);
		return 0;
	}

	struct sockaddr_in seraddr;

	port = atoi(argv[2]);
	seraddr.sin_family = AF_INET;
	seraddr.sin_port = htons(port);
	seraddr.sin_addr.s_addr = inet_addr(argv[1]);

	reactor::handle_t handle = socket(AF_INET,SOCK_STREAM,0);

	if(connect(handle,(struct sockaddr*)&seraddr,sizeof(seraddr))<0){
		perror("connect");
		return 0;
	}

	Reactor *m_reactor = Reactor::get_instance();

	ClientHandler *stdin_handler = new ClientHandler(0);
	m_reactor->register_handler(stdin_handler,reactor::READEVENT);

	ClientHanler *cli_handler = new ClientHandler(handle);
	m_reactor->register_handler(cli_handler,reactor::WRITEEVENT);
	while(1){
		m_reactor->handle_events();
	}

	return 0;

}


