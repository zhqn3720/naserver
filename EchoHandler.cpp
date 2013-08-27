#include "EchoHandler.hpp"
#include "Dispatcher.hpp"
#include <unistd.h>
#include <iostream>
using namespace tanic;

EchoHandler::EchoHandler(Dispatcher * dispatcher, int handle)
              :m_dispatcher(dispatcher)
{
	//std::cout << "register echohandler\n";
	m_handle = handle;
	m_dispatcher->registerEvents(this,POLLIN | POLLRDNORM);
}

void EchoHandler::handleEvent()
{
	 if (m_reventType & (POLLIN | POLLPRI | POLLRDHUP)) {
		 handleRead();
	 }
	 if ((m_reventType & POLLHUP) && !(m_reventType & POLLIN)) {
		 handleClose();
	 }
 }
 
 void EchoHandler::handleRead()
 {
	 char buf[100];
	 ssize_t n = read(m_handle, buf, sizeof(buf));
	 if (n > 0) {
		 std::cout << " received "<<n <<" bytes\n";
	 } else if (n == 0) {
		 handleClose();
	 } else {
		 handleError();
	 }
}

void EchoHandler::handleClose()
{
	;
}
