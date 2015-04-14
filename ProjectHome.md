The C++11 standard introduced class std::thread and a new concurrency-aware memory model.

This proposal leverage these new features in order to propose class std::process and a few associated classes, functions and concepts.

The goal of std::process is to manage operating system processes in C++ using a simple and already known interface. Class std::process is mirroring the interface of class std::thread, and we introduce a new namespace std::this\_process (reminsecent of std::this\_thread) that contain a few process management function.

With this proposal, it's possible to write code such as :

```
#include <process>
#include <cstdlib>

extern void run_as_daemon(int ac, const char *av[]);

int main(int ac, const char *av[])
{
  std::process p([=](int ac, const char *av[]) {
    run_as_daemon(ac, av);
  });
  p.detach();
}
```

or

```
#include <process>
#include <iostream>

int main(int ac, const char *av[])
{
  std::cout << "parent process is " << std::this_process::get_id() << std::endl;
  std::process p([=](int ac, const char *av[]) {
    std::cout << "before exec, I'm process " << std::this_process::get_id() << std::endl;
    std::this_process::exec(
      "/bin/bash", 
      "-c", 
      "\"echo Hello, I'm exec()'ed process $$\""
    );
  });
  p.join();
}
```

Ongoing work tries to specify interprocess communication (including pipes, named mutexes, named semaphores and so on) as well as a few utility classes and functions.