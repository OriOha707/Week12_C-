#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>

#define OPTION_1 1
#define OPTION_2 2
#define OPTION_3 3
#define EXIT 4

class MessageSender
{
public:
	void showMenu();
	void readAdminFile();
	void writeMessagesToUsersFile();

private:
	std::vector<std::string> _data;
	std::set<std::string> _users;
};