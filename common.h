#ifndef COMMON_H
#define COMMON_H

namespace reactor{
	
	#define MAX_EVENTS 100
	#define BUFSIZE 1024
	typedef int handle_t;
	typedef unsigned int event_t;
	
	enum{
		READEVENT	= 0x1;
		WRITEEVENT	= 0x2;
		ERROREVENT	= 0X4;
		MASKEVENT	= 0XFF;
	};

	struct handle_struct{
		handle_t handle;
		char buf[BUFSIZE];
	};

	void printerr(char *errstr){
		perror(errstr);
		exit(EXIT_FAILURE);
	}
}

#endif
