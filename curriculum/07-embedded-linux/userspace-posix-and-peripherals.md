# 💻 User-Space POSIX Systems Programming & Hardware I/O

> Multi-threading, network sockets, IPC, and controlling GPIO/I2C/SPI from user space.

---

## 1. Controlling Hardware from Linux User Space

In modern Linux, you often don't need to write a kernel driver just to toggle a pin or read an I2C sensor:

### 1. Modern GPIO (`libgpiod`):
The deprecated `/sys/class/gpio` has been replaced by the character device `/dev/gpiochipN`. It provides atomic multi-line reading, hardware debouncing, and interrupt edge events via file descriptors.

```c
#include <gpiod.h>

struct gpiod_chip *chip = gpiod_chip_open_by_number(0);
struct gpiod_line *line = gpiod_chip_get_line(chip, 17);
gpiod_line_request_output(line, "my_app", 0);
gpiod_line_set_value(line, 1); // Turn ON GPIO 17
```

### 2. User-Space I2C (`/dev/i2c-N`):
```c
int fd = open("/dev/i2c-1", O_RDWR);
ioctl(fd, I2C_SLAVE, 0x76); // Set 7-bit slave address
write(fd, reg_buf, 2);       // Write to sensor
read(fd, data_buf, 6);       // Read from sensor
```

---

## 2. Multi-Threading with POSIX Threads (`pthreads`)

```c
#include <pthread.h>
#include <stdio.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *WorkerThread(void *arg) {
    pthread_mutex_lock(&lock);
    printf("Critical section running in worker thread\n");
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(void) {
    pthread_t thread;
    pthread_create(&thread, NULL, WorkerThread, NULL);
    pthread_join(thread, NULL);
    return 0;
}
```
