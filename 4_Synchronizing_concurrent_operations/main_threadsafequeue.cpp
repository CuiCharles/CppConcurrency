#include "thread_safe_queue.h"
#include <iostream>
#include <thread>


std::vector<int>  data_vec;
int tip_index = 0;
threadsafe_queue<int> data_queue;



bool more_data_to_prepare()
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


void process_data(int data)
{
	std::cout << data << std::endl;
}




void data_preparation_thread()
{
    while(more_data_to_prepare())
    {
  		const int data=prepare_data();
        data_queue.push(data);
    }
}
void data_processing_thread()
{

	
	std::this_thread::sleep_for(std::chrono::milliseconds(100));

    while(true)
    {
  		int data;
		data_queue.wait_pop(data);
		process_data(data);
		if(is_last_chunk(data))
			break;
  	} 
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