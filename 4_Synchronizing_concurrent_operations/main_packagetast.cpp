#include <deque>
#include <mutex>
#include <thread>
#include <future>
#include <utility>
#include <iostream>

std::mutex m;
std::deque<std::packaged_task<int(int)> > tasks;

bool is_shut_down = false;

int x = 6;

bool gui_shutdown_message_recieved()
{
	return is_shut_down;
}
void get_and_process_gui_message()
{
	//std::cout << "get_and_process_gui_message" << std::endl;
}

void gui_thread()
{
	while(!gui_shutdown_message_recieved())
	{
		get_and_process_gui_message();
		std::packaged_task<int(int)> task;
		{
			std::lock_guard<std::mutex> lk(m);
			if(tasks.empty())
				continue;
			task = std::move(tasks.front());
			tasks.pop_front();
		}
		task(x++);
	}
}



template<typename Func>
std::future<int> post_task_for_gui_thread(Func f)
{
	std::packaged_task<int(int)> task(f);
	std::future<int> res = task.get_future();
	std::lock_guard<std::mutex> lk(m);
	tasks.push_back(std::move(task));
	return res;
}



int main()
{

	std::thread gui_bg_thread(gui_thread);

	std::future<int> f1 = post_task_for_gui_thread([](int a) -> int{std::cout << "func1" << std::endl; return a;});
	std::future<int> f2 = post_task_for_gui_thread([](int a) -> int{std::cout << "func2" << std::endl; return a;});
	std::future<int> f3 = post_task_for_gui_thread([](int a) -> int{std::cout << "func3" << std::endl; return a;});

	std::cout << "future1" << f1.get() << std::endl;
	std::cout << "future2" << f2.get() << std::endl;
	std::cout << "future3" << f3.get() << std::endl;

	getchar();
	is_shut_down = true;
	gui_bg_thread.join();

	return 0;


}