#include <thread>
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <iomanip>

#include <process>
#include <named_mutex>

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
		std::cout << "C this process H  : " << std::hex << std::hash<std::process::id>()(std::this_process::get_id()) << std::dec << std::endl;
		std::exit(EXIT_SUCCESS);
	});
	std::cout << "P this process    : " << std::this_process::get_id() << std::endl;
	std::cout << "P this process H  : " << std::hex << std::hash<std::process::id>()(std::this_process::get_id()) << std::dec << std::endl;
	p.join();

	std::cout << std::endl;
}

void check_named_mutex()
{
	std::cout << "======== " << __func__ << std::endl;
	
	// we'll create 3 processes that will play with a named mutex
	std::process p0([=](){
		std::named_mutex n("test-named-mutex");
		std::cout << "C0 mutex created  : " << n.name() << std::endl;
		for (int i=0; i<5; ++i) {
			std::this_thread::yield();
			n.lock();
			std::cout << "C0 mutex acquired" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			std::cout << "C0 mutex released" << std::endl;
			n.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(133));
		}
		std::exit(EXIT_SUCCESS);
	});

	std::process p1([=](){
		std::named_mutex n("test-named-mutex");
		std::cout << "C1 mutex created  : " << n.name() << std::endl;
		for (int i=0; i<5; ++i) {
			std::this_thread::yield();
			n.lock();
			std::cout << "C1 mutex acquired" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(133));
			std::cout << "C1 mutex released" << std::endl;
			n.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(85));
		}
		std::exit(EXIT_SUCCESS);
	});

	std::process p2([=](){
		std::named_mutex n("test-named-mutex");
		std::cout << "C2 mutex created  : " << n.name() << std::endl;
		for (int i=0; i<5; ++i) {
			n.lock();
			std::this_thread::yield();
			std::cout << "C2 mutex acquired" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(85));
			std::cout << "C2 mutex released" << std::endl;
			n.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
		std::exit(EXIT_SUCCESS);
	});

	p0.join();
	p1.join();
	p2.join();
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

	check_named_mutex();
}
