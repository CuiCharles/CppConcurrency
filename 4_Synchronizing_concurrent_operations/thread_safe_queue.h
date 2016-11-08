 #ifndef __THREAD_SAFE_QUEUE__
 #define __THREAD_SAFE_QUEUE__

#include <memory>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <iostream>


template<typename T>
class threadsafe_queue
{
private:
	mutable std::mutex mut;
	std::queue<T> data_queue;
	std::condition_variable data_cond;

public:
	threadsafe_queue& operator=(const threadsafe_queue&) = delete;

	threadsafe_queue()
	{

	}

	threadsafe_queue(const threadsafe_queue &other)
	{
		std::lock_guard<std::mutex> lk(other.mut);

		data_queue = other.data_queue;
	}

	

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lk(mut);
		
		data_queue.push(new_value);
		std::cout << "push data " << new_value << std::endl;
		data_cond.notify_one();

	}


	bool try_pop(T& value)
	{ 
		std::lock_guard<std::mutex> lk(mut);
		if(data_queue.empty())
			return false;
		value = data_queue.front();
		data_queue.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(mut);
		if(data_queue.empty())
			return std::shared_ptr<T> ();
		std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
		data_queue.pop();
		return res;
	}

	void wait_pop(T& value)
	{
		std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        value=data_queue.front();
        data_queue.pop();
	}

	std::shared_ptr<T> wait_pop()
	{

		std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk,[this]{return !data_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
	}

	bool empty() const
	{ 
		std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
	}


};

#endif