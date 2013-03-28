#include <thread>
#include <iostream>
#include <cstdlib>
#include <process>

int check_id()
{
	std::process p([=]() {
		std::cout << "- this process    : " << std::this_process::get_id() << std::endl;
		std::exit(EXIT_SUCCESS);
	});
	std::cout << "+ this process    : " << std::this_process::get_id() << std::endl;
	std::cout << "+ before join()   : " << p.get_id() << std::endl;
	p.join();
	std::cout << "+ after join()    : " << p.get_id() << std::endl;
	std::cout << "+ this process    : " << std::this_process::get_id() << std::endl;
}

int check_exec()
{
	std::cout << "I'm the parent, pid=" << std::this_process::get_id() << std::endl;
	std::process p([=]() {
		std::cout << "I'm the child, pid=" << std::this_process::get_id() << std::endl;
		std::this_process::exec("/bin/bash", "-c", "echo \"I'm the exec()uted child, pid=$$\"");
	});
	p.join();
}

int main()
{
	check_id();
	check_exec();
}
