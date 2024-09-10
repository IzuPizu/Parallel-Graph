
### Thread Pool Description

A thread pool contains a given number of active threads that simply wait to be given specific tasks.
The threads are created when the thread pool is created.
Each thread continuously polls the task queue for available tasks.
Once tasks are put in the task queue, the threads poll tasks, and start running them.
A thread pool creates **N** threads upon its creation and does not destroy (join) them throughout its lifetime.
That way, the penalty of creating and destroying threads ad-hoc is avoided.
As such, you must implement the following functions (marked with `TODO` in the provided skeleton, in `src/os_threadpool.c`):

- `enqueue_task()`: Enqueue task to the shared task queue.
  Use synchronization.
- `dequeue_task()`: Dequeue task from the shared task queue.
  Use synchronization.
- `wait_for_completion()`: Wait for all worker threads.
  Use synchronization.
- `create_threadpool()`: Create a new thread pool.
- `destroy_threadpool()`: Destroy a thread pool.
  Assume all threads have been joined.


### Input Files

Reading the graphs from the input files is being taken care of the functions implemented in `src/os_graph.c`.
A graph is represented in input files as follows:

- First line contains 2 integers `N` and `M`: `N` - number of nodes, `M` - numbed or edges
- Second line contains `N` integer numbers - the values of the nodes.
- The next `M` lines contain each 2 integers that represent the source and the destination of an edge.



### Data Structures

#### Graph

A graph is represented internally by the `os_graph_t` structure (see `src/os_graph.h`).

#### List

A list is represented internally by the `os_queue_t` structure (see `src/os_list.h`).
You will use this list to implement the task queue.

#### Thread Pool

A thread pool is represented internally by the `os_threadpool_t` structure (see `src/os_threadpool.h`).
The thread pool contains information about the task queue and the threads.



### Building

To build both the serial and the parallel versions, run `make` in the `src/` directory:

```console
student@so:~/.../content/assignments/parallel-graph$ cd src/

student@so:~/.../assignments/parallel-graph/src$ make
```


## Testing and Grading

Testing is automated.
Tests are located in the `tests/` directory.

```console
student@so:~/.../assignments/parallel-graph/tests$ ls -F
Makefile  checker.py  grade.sh@  in/
```
