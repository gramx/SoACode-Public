#pragma once
#include <vector>
#include <thread>
#include <condition_variable>

#include "concurrentqueue.h"
#include "IThreadPoolTask.h"

class Chunk;
struct LoadData;

#define NUM_WORKER_CONTEXT 20

enum class MeshJobType;

// Worker data for a threadPool
class WorkerData {
public:
    volatile bool waiting;
    volatile bool stop;

    // Each Thread Gets Its Own Mesher
    ChunkMesher* chunkMesher;
};

class ThreadPool {
public:
    ThreadPool();
    ~ThreadPool();

    /// Initializes the threadpool
    /// @param size: The number of worker threads
    void init(ui32 size);

    /// Frees all resources
    void destroy();

    /// Clears all unprocessed tasks from the task queue
    void clearTasks();

    /// Adds a task to the task queue
    /// @param task: The task to add
    void addTask(IThreadPoolTask* task) { 
        _tasks.enqueue(task);
        _cond.notify_one(); 
    }

    /// Add an array of tasks to the task queue
    /// @param tasks: The array of tasks to add
    /// @param size: The size of the array
    void addTasks(IThreadPoolTask* tasks[], size_t size) { 
        _tasks.enqueue_bulk(tasks, size);
        _cond.notify_all(); 
    }
   
    /// Adds a vector of tasks to the task queue
    /// @param tasks: The vector of tasks to add
    void addTasks(std::vector<IThreadPoolTask*> tasks) { 
        _tasks.enqueue_bulk(tasks.data(), tasks.size());
        _cond.notify_all();
    }
    
    /// Gets a bulk of tasks from the finished tasks
    /// @param taskBuffer: Buffer to store the tasks
    /// @param maxSize: Max tasks to deque
    /// @return: The number of dequeued tasks
    size_t getFinishedTasks(IThreadPoolTask** taskBuffer, size_t maxSize) {
        return _finishedTasks.try_dequeue_bulk(taskBuffer, maxSize);
    }

    /// Checks if the threadpool is finished with all it's work
    /// @return true when all threads are sleeping and all work is done
    bool isFinished();

    /// Getters
    const i32& getSize() const { return _workers.size(); }

private:
    // Typedef for func ptr
    typedef void(ThreadPool::*workerFunc)();

    /// Class definition for worker thread
    class WorkerThread {
    public:
        /// Creates the thread
        /// @param func: The function the thread should execute
        WorkerThread(workerFunc func) {
            thread = new std::thread(func, &data);
        }

        /// Joins the worker thread
        void join() { thread->join(); } 

        std::thread* thread; ///< The thread handle
        WorkerData data; ///< Worker specific data
    };

    /// Thread function that processes tasks
    /// @param data: The worker specific data
    void workerThreadFunc(WorkerData* data);

    /// Lock free task queues
    moodycamel::ConcurrentQueue<IThreadPoolTask*> _tasks; ///< Holds tasks to execute
    moodycamel::ConcurrentQueue<IThreadPoolTask*> _finishedTasks; ///< Holds finished tasks

    std::mutex _condMutex; ///< Mutex for the conditional variable
    std::condition_variable _cond; ///< Conditional variable that workers block on

    bool _isInitialized = false; ///< true when the pool has been initialized
    std::vector<WorkerThread*> _workers; ///< All the worker threads
};

