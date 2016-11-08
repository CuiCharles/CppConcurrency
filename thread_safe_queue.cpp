#include "thread_safe_queue.h"

threadsafe_queue::threadsafe_queue(){}

threadsafe_queue::threadsafe_queue(const threadsafe_queue &another){}

void threadsafe_queue::push(T new_value){}

bool threadsafe_queue::try_pop(T &value){return false;}

std::shared_ptr threadsafe_queue::try_pop(){return nullptr;}

void threadsafe_queue::wait_pop(T &value){}

std::shared_ptr<T> threadsafe_queue::wait_pop()(return nullptr;);

bool threadsafe_queue::empty(){ return true;}


