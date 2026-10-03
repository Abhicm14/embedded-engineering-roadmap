# 🚨 Priority Inversion & The Mars Pathfinder Anomaly

> The classic real-time concurrency bug and how Priority Inheritance Protocol (PIP) prevents system lockup.

---

## 1. What is Unbounded Priority Inversion?

Priority Inversion occurs when a high-priority task ($H$) is indirectly blocked by a low-priority task ($L$) due to an intermediate medium-priority task ($M$):

```
Time Sequence:
1. Low-priority task (L) takes a shared Mutex protecting a bus.
2. High-priority task (H) wakes up and preempts L.
3. Task H requests the Mutex, finds it locked, and enters the Blocked state.
4. Medium-priority task (M), which does NOT need the mutex, wakes up.
5. Task M preempts L (since Priority M > Priority L).
6. Result: Task M runs indefinitely, preventing Task L from finishing and releasing the mutex!
   High-priority Task H is starved by Medium task M!
```

---

## 2. The Solution: Priority Inheritance Protocol (PIP)

When high-priority Task $H$ blocks waiting for a Mutex held by low-priority Task $L$:
- The RTOS **temporarily elevates the priority of Task $L$ to match Task $H$**.
- Medium-priority Task $M$ can no longer preempt Task $L$.
- Task $L$ quickly finishes its critical section and releases the Mutex.
- As soon as the Mutex is given back, Task $L$'s priority drops back to its original low level, and Task $H$ immediately preempts and acquires the Mutex!

> 💡 **Always Use `xSemaphoreCreateMutex()` for Shared Resources:** FreeRTOS binary semaphores do NOT implement Priority Inheritance! Only Mutexes created via `xSemaphoreCreateMutex()` include PIP.
