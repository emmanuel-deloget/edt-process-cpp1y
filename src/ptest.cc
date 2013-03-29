#include <thread>
#include <iostream>
#include <cstdlib>
#include <process>

int check_id()
{
	std::cout << "======== " << __func__ << std::endl;

	std::process p([=]() {
		std::cout << "C this process    : " << std::this_process::get_id() << std::endl;
		std::exit(EXIT_SUCCESS);
	});
	std::cout << "P this process    : " << std::this_process::get_id() << std::endl;
	std::cout << "P before join()   : " << p.get_id() << std::endl;
	p.join();
	std::cout << "P after join()    : " << p.get_id() << std::endl;
	std::cout << "P this process    : " << std::this_process::get_id() << std::endl;

	std::cout << std::endl;
}

int check_exec()
{
	std::cout << "======== " << __func__ << std::endl;

	std::cout << "P this process    : " << std::this_process::get_id() << std::endl;
	std::process p([=]() {
	std::cout << "C this process    : " << std::this_process::get_id() << std::endl;
		std::this_process::exec("/bin/bash", "-c", "echo \"E this process    : $$\"");
	});
	p.join();

	std::cout << std::endl;
}

void check_hash()
{
	std::cout << "======== " << __func__ << std::endl;

	std::process p([=]() {
		std::cout << "C this process    : " << std::this_process::get_id() << std::endl;
		std::cout << "C this process H  : " << std::hex << std::hash<std::process::id>()(std::this_process::get_id()) << std::endl;
		std::exit(EXIT_SUCCESS);
	});
	std::cout << "P this process    : " << std::this_process::get_id() << std::endl;
	std::cout << "P this process H  : " << std::hex << std::hash<std::process::id>()(std::this_process::get_id()) << std::endl;
	p.join();

	std::cout << std::endl;
}

int main()
{
	std::cout << "======== LEGEND" << std::endl;
	std::cout << "  P == parent" << std::endl;
	std::cout << "  C == child" << std::endl;
	std::cout << "  E == executed" << std::endl;
	std::cout << std::endl;

	check_id();
	check_exec();
	check_hash();
}
