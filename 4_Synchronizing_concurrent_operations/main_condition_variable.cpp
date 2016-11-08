#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <vector>
#include <condition_variable>
#include "thread_safe_queue.h"

/*
	std::condition_variable and std::condition_variable_any are both declared in <condition_variable> 
	library header.They both need to work with an std::mutex in order to provide appropriate 
	synchronization.

	

*/

std::mutex mut;
std::queue<int> data_queue;
std::condition_variable data_cond;
std::vector<int>  data_vec;
int tip_index = 0;


bool more_data_prepared()
{
	if(tip_index < 100)
	{
		return true;
	}
	else 
	{
		return false;
	}
}


int prepare_data()
{
	return data_vec[tip_index++];
}

bool is_last_chunk(int data)
{
	if(data == 100)
		return true;
	else 
		return false;
}

void process_data(int);

//productor
void data_preparation_thread()
{
	while(more_data_prepared())
	{
		const int data = prepare_data();
		std::lock_guard<std::mutex> lk(mut);
		data_queue.push(data);
		std::cout << "push data " << data << std::endl;
		data_cond.notify_one();
	}
}


//consumer
void data_processing_thread()
{
	while(true)
	{
		std::unique_lock<std::mutex> lk(mut);
		//first lock, then check. If the condition is not satisfied,unlocking the mutex and waiting(blocked).
		//otherwise, continue the locking status and go on.
		data_cond.wait(lk, []{ return !data_queue.empty();});
		int data = data_queue.front();
		data_queue.pop();
		lk.unlock();
		process_data(data);
		if(is_last_chunk(data))
		{
			break;
		}
	}
}

void process_data(int data)
{
	std::cout << data << std::endl;
}


int main()
{

	for(int i = 0;i < 100; ++i)
	{
		data_vec.push_back(i + 1);
	}

	std::thread preparetion_thread(data_preparation_thread);
	std::thread processing_thread(data_processing_thread);
	preparetion_thread.join();
	processing_thread.join();

	return 0;
}