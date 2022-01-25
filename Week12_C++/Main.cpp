#include<thread>
#include <iostream>
#include "MessageSender.h"

int main()
{
	MessageSender t;

	std::thread reader(&MessageSender::readAdminFile, &t);
	std::thread sender(&MessageSender::writeMessagesToUsersFile, &t);

	t.showMenu();

	reader.detach();
	sender.detach();

	return 0;
}