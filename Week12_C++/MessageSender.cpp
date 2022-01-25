#include "MessageSender.h"

std::mutex mu1;
std::mutex mu2;
std::condition_variable c;

/*
* The function print the menu
* input: none
* output: none
*/
void MessageSender::showMenu()
{
	int option = 0;
	std::string name = " ";
	while (option != EXIT)
	{
		std::cout << "1. Sign in" << std::endl;
		std::cout << "2. Sign out" << std::endl;
		std::cout << "3. Connected users" << std::endl;
		std::cout << "4. Exit" << std::endl;

		std::cin >> option;

		if (option == OPTION_1)
		{
			std::cout << "Please enter Name: " << std::endl;
			std::cin >> name;
			if (this->_users.count(name))
			{
				std::cout << "The name is taken!" << std::endl;
			}
			else
			{
				std::lock_guard <std::mutex> lock_user(mu2);
				this->_users.insert(name);
			}
		}

		else if (option == OPTION_2)
		{
			std::cout << "Please enter Name: " << std::endl;
			std::cin >> name;

			if (this->_users.count(name))
			{
				std::cout << "The name doesn't exist!" << std::endl;
			}

			else
			{
				std::lock_guard <std::mutex> lock_user(mu2);
				this->_users.erase(name);
			}
		}

		else if (option == OPTION_3)
		{
			for (std::set<std::string>::iterator it = this->_users.begin(); it != this->_users.end(); it++)
			{
				std::cout << *it << std::endl;
			}
		}
		else if (option == EXIT)
			exit(1);
	}
}

/*
* The function read the admin file "data.txt"
*/
void MessageSender::readAdminFile()
{
	std::string fileName = "data.txt";
	std::fstream file;

	std::string ans;

	while (true)
	{
		std::this_thread::sleep_for(std::chrono::seconds(60));
		file.open(fileName);
		std::unique_lock<std::mutex> locker(mu1);
		while (!file.eof())
		{
			std::getline(file, ans);
			this->_data.push_back(ans);
		}
		locker.unlock();
		c.notify_one();
		std::ofstream deleteTool;
		deleteTool.open(fileName, std::ofstream::out | std::ofstream::trunc);
		deleteTool.close();
		file.close();
	}
}

/*
* Function write the massages to the user's file.
*/
void MessageSender::writeMessagesToUsersFile()
{
	std::string fileName = "output.txt";
	std::ofstream file(fileName);
	std::string tmp;
	std::set<std::string>::iterator iter;
	while (true)
	{
		std::unique_lock<std::mutex> locker(mu1);
		c.wait(locker, [&]() {return !(this->_data.empty()); });

		std::lock_guard<std::mutex> setUsersLock(mu2);

		for (size_t i = 0; i < this->_data.size(); i++)
		{
			for (iter = this->_users.begin(); iter != this->_users.end(); ++iter)
			{
				tmp = _data[i];
				file << (*iter) << " : " << this->_data[i] << std::endl;
			};
		}
		locker.unlock();
		std::this_thread::sleep_for(std::chrono::seconds(60));
	}
	file.close();
}