#include <thread>
#include <iostream>
#include <cstdlib>
#include <process>

int main()
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
