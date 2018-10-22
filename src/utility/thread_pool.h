#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class Thread_Pool
{
public:
    Thread_Pool();
    Thread_Pool(int num_threads);
    ~Thread_Pool();

    Thread_Pool(Thread_Pool const& o) = delete;            // copy ctor
    Thread_Pool(Thread_Pool&& o) = delete;                 // move ctor
    Thread_Pool& operator=(Thread_Pool const& o) = delete; // copy assignment
    Thread_Pool& operator=(Thread_Pool&& o) = delete;      // move assignment

    void add_work(std::function<void()> job);
    // void add_work(iterator begin, iterator end);

    void enable();
    void disable();
    bool is_done();

    // void add_thread();
    // void remove_thread();

    // TODO(sdsmith): join threads on destruction

private:
    std::vector<std::thread> m_workers = {};
    std::queue<std::function<void()>> m_job_queue = {};
    std::mutex m_queue_lock = {};
    std::condition_variable m_wait_cond = {};
    std::atomic<bool> m_is_enabled{false};
    std::atomic<bool> m_stop_workers{false};
    std::atomic<size_t> m_num_waiting{0};

    void create_workers(unsigned num_threads);
    void do_work();
    void kill_workers();
};
