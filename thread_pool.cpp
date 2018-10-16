#include "thread_pool.h"

#include <iostream>

Thread_Pool::Thread_Pool() { create_workers(std::thread::hardware_concurrency() / 2); }
Thread_Pool::Thread_Pool(int num_threads) { create_workers(num_threads); }
Thread_Pool::~Thread_Pool() { kill_workers(); }

void Thread_Pool::add_work(std::function<void()> job) {
    {
        std::unique_lock<std::mutex> lock(m_queue_lock);
        m_job_queue.push(job);
    }

    if (m_is_enabled) {
        m_wait_cond.notify_one();
    }
}

void Thread_Pool::enable() {
    if (m_is_enabled) { return; } // skip if enabled
    
    m_is_enabled = true;
    m_wait_cond.notify_all(); // expensive call...

    /*
    // It's possible that jobs could be added to the queue between when we 
    // check the queue size and when we notify. This can result in there 
    // being jobs in the queue where no threads are awake to grab them.
    size_t num_jobs;
    {
        std::unique_lock<std::mutex> lock(m_queue_lock);
        num_jobs = m_job_queue.size();
    }

    switch (num_jobs) {
        case 0: 
            break; // don't notify if there is no work
        case 1: 
            m_wait_cond.notify_one(); 
            break;
        default: 
            m_wait_cond.notify_all(); 
            break;
    }
    */
}

void Thread_Pool::disable() { 
    m_is_enabled = false;
}

bool Thread_Pool::is_done() { 
    std::unique_lock<std::mutex> lock(m_queue_lock);
    return (m_num_waiting == m_workers.size()) && m_job_queue.empty();
}

void Thread_Pool::kill_workers() {
    m_stop_workers = true;
    m_wait_cond.notify_all();

    for (auto& worker : m_workers) {
        worker.join();
    }
}

void Thread_Pool::create_workers(unsigned num_threads) {
    m_workers.reserve(static_cast<size_t>(num_threads));

    for (unsigned n = 0; n < num_threads; ++n) {
        m_workers.push_back(std::thread(&Thread_Pool::do_work, this));
    }
}

void Thread_Pool::do_work() {
    while (true) {
        std::function<void()> job;

        {
            std::unique_lock<std::mutex> lock(m_queue_lock); // TODO: We lock the queue while we wait. is this a good idea?

            m_num_waiting++;
            m_wait_cond.wait(
                lock, [this]() { return (m_is_enabled && !m_job_queue.empty()) || m_stop_workers; });
            m_num_waiting--;

            if (m_stop_workers) {
                return;
            }

            job = m_job_queue.front();
            m_job_queue.pop();
        }

        job();
    }
}