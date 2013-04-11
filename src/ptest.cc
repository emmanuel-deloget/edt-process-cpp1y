#include <thread>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <iomanip>
#include <algorithm>

#include <sys/types.h>
#include <sys/wait.h>

#include <process>
#include <named_mutex>

int check_id()
{
	std::cout << "======== " << __func__ << std::endl;

	std::process p([=]() {
		std::cout << "C this process    : " << std::this_process::get_id() << std::endl;
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
		std::this_process::exec("/bin/bash", "-c", "echo \"E this process 3  : $$\"");
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
		for (int i=0; i<3; ++i) {
			std::this_thread::yield();
			n.lock();
			std::cout << "C0 mutex acquired" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
			std::cout << "C0 mutex released" << std::endl;
			n.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(133));
		}
	});

	std::process p1([=](){
		std::named_mutex n("test-named-mutex");
		std::cout << "C1 mutex created  : " << n.name() << std::endl;
		for (int i=0; i<2; ++i) {
			std::this_thread::yield();
			n.lock();
			std::cout << "C1 mutex acquired" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(133));
			std::cout << "C1 mutex released" << std::endl;
			n.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(85));
		}
	});

	std::process p2([=](){
		std::named_mutex n("test-named-mutex");
		std::cout << "C2 mutex created  : " << n.name() << std::endl;
		for (int i=0; i<4; ++i) {
			n.lock();
			std::this_thread::yield();
			std::cout << "C2 mutex acquired" << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(85));
			std::cout << "C2 mutex released" << std::endl;
			n.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	});

	p0.join();
	p1.join();
	p2.join();
	std::cout << std::endl;
}

void check_ret_code()
{
	std::cout << "======== " << __func__ << std::endl;

	int status; 
	std::process p([]() -> int {
		std::cout << "C this process    : " << std::this_process::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		return 1;
	});
	::pid_t the_pid = p.native_handle();
	p.detach();
	std::cout << "P this process    : " << std::this_process::get_id() << std::endl;

	::waitpid(the_pid, &status, 0);
	std::cout << "P finishing child : " << the_pid << std::endl;
	std::cout << "P status from C   : " << WEXITSTATUS(status) << std::endl;

	std::cout << std::endl;
}

void check_spawn()
{
	std::cout << "======== " << __func__ << std::endl;
	std::cout << "P this process    : " << std::this_process::get_id() << std::endl;
	std::this_process::spawn("/bin/bash", "-c", "echo \"E this process 1  : $$\"");
	std::this_process::spawn_wait("/bin/bash", "-c", "echo \"E this process 2  : $$\"");
	std::cout << std::endl;
}

bool verify_zombi(::pid_t pid)
{
	std::stringstream ss;
	ss << "/proc/" << pid << "/status";
	std::ifstream file(ss.str().c_str());
	if (!file)
		return false;
	do {
		std::string line;
		std::getline(file, line);
		if (line.length() > 6) {
			std::string ident(line.begin(), line.begin() + 6);
			if (ident == "State:") {
				std::replace(line.begin(), line.end(), '\t', ' ');
				std::stringstream tokenizer(line);
				std::string token;
				while (std::getline(tokenizer, token, ' ')) {
					if (token == "Z" || token == "Z+")
						return true;
				}
			}
		}
	} while (!file.eof());

	return false;
}

void check_detach()
{
	std::cout << "======== " << __func__ << std::endl;
	std::cout << "P this process    : " << std::this_process::get_id() << std::endl;
	std::process p([](){
		std::cout << "C this process    : " << std::this_process::get_id() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
		std::cout << "C this process    : " << "ending now" << std::endl;
	});
	::pid_t pid = p.native_handle();
	p.detach();
	std::cout << "P this process    : " << "no zombie should appear" << std::endl;
	for (int i=0; i< 5; ++i) {
		if (verify_zombi(pid)) {
			std::cout << "P this process    : " << pid << " is a zombie!" << std::endl;
			break;
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
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
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	check_exec();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	check_spawn();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	check_hash();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	check_ret_code();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	check_named_mutex();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
	check_detach();
	std::this_thread::sleep_for(std::chrono::milliseconds(200));
}
