# User-Level Thread Scheduling

**Background:**

Threads can be separated into two kinds: kernel-level threads and user-level threads. Kernel-level
threads are managed and scheduled by the kernel. User-level threads are needed to be managed by
the programmer and they are seen as a single-threaded process from the kernel’s point of view. The
user-level threads have some advantages and disadvantages over kernel-level threads.

*Advantages:*
* User-level threads can be implemented on operating systems which do not support threads.
* Because there is no trapping in kernel, context switching is faster.
* Programmer has direct control over the scheduling policy.

*Disadvantages:*
* Blocking system calls block the entire process.
* In the case of a page fault, the entire process is blocked, even if some threads might be
runnable.
* Because kernel sees user-level threads as a single process, they cannot take advantage of
multiple CPUs.
* Programmer has to make sure that threads give up the CPU voluntarily or has to implement a
periodic interrupt which schedules the threads.

<ucontext.h> library is used to implement different user level threads. The threads can be in three different states: ready, running, or finished.
