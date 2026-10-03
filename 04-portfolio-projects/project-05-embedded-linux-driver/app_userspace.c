#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE_PATH "/dev/pwm_gpio_dev"
#define BUFFER_SIZE 256

int main(int argc, char *argv[]) {
    printf("=== Linux User-Space Character Driver Control Utility ===\n");

    int fd = open(DEVICE_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device at " DEVICE_PATH);
        printf("Note: If testing without root / hardware, ensure module is loaded and permissions are 0666\n");
        return EXIT_FAILURE;
    }

    char rx_buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(fd, rx_buffer, sizeof(rx_buffer) - 1);
    if (bytes_read > 0) {
        rx_buffer[bytes_read] = '\0';
        printf("[DRIVER CURRENT STATE]: %s\n", rx_buffer);
    }

    const char *new_command = "PWM_STATE=ACTIVE,FREQ=5000,DUTY=80\n";
    printf("Sending new configuration to driver: %s", new_command);
    ssize_t bytes_written = write(fd, new_command, strlen(new_command));
    if (bytes_written < 0) {
        perror("Failed to write to device");
        close(fd);
        return EXIT_FAILURE;
    }

    close(fd);
    printf("Device updated and closed successfully.\n");
    return EXIT_SUCCESS;
}
