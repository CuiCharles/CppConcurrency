#include <deque>
#include <mutex>
#include <thread>
#include <future>
#include <utility>
#include <iostream>

std::mutex m;
std::deque<std::packaged_task<void()> > tasks;

bool is_shut_down = false;

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
		std::packaged_task<void()> task;
		{
			std::lock_guard<std::mutex> lk(m);
			if(tasks.empty())
				continue;
			task = std::move(tasks.front());
			tasks.pop_front();
		}
		task();
	}
}



template<typename Func>
std::future<void> post_task_for_gui_thread(Func f)
{
	std::packaged_task<void()> task(f);
	std::future<void> res = task.get_future();
	std::lock_guard<std::mutex> lk(m);
	tasks.push_back(std::move(task));
	return res;
}



int main()
{

	std::thread gui_bg_thread(gui_thread);

	post_task_for_gui_thread([]{std::cout << "func1" << std::endl;});
	post_task_for_gui_thread([]{std::cout << "func2" << std::endl;});
	post_task_for_gui_thread([]{std::cout << "func3" << std::endl;});
	getchar();
	is_shut_down = true;
	gui_bg_thread.join();

	return 0;


}