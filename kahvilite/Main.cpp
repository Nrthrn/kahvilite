#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include "Clicker.h"

namespace m {
	std::string command = "";
	std::string tempCPS = "";
}

int main()
{
	system("title kahvi lite v0.1");

	std::thread t(startClicker);
	t.detach();

	while (true)
	{
		std::string ascii = R"(
				 __  __   ____   _   _ __  __ _     _     _  _____  ____ 
				|  |/  / / () \ | |_| |\ \/ /| |   | |__ | ||_   _|| ===|
				|__|\__\/__/\__\|_| |_| \__/ |_|   |____||_|  |_|  |____|
		)";

		std::cout << ascii;

		std::string commands = R"(


				clicker - toggle clicker
				cps     - change CPS
)";
		std::cout << commands;
		std::cout << "\n\n";


		std::cout << "				lite>";
		std::cin >> m::command;

		//std::stoi(str);
		if (m::command == "clicker")
		{
			clicker::enabled = !clicker::enabled;
			std::string state = clicker::enabled ? "ON" : "OFF";
		}
		else if (m::command == "cps")
		{
			std::cout << "				New CPS: ";
			std::cin >> m::tempCPS;
			std::stringstream ss(m::tempCPS);
			int newCPS;
			ss >> newCPS;
			clicker::cps = newCPS;
		}

		system("cls");
	}
	return 0;
}