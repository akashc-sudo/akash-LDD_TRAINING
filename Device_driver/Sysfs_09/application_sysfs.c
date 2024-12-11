/* Application program for sysfs driver(SysfsSriver)*/


#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define SYSFS_PATH "/sys/kernel/sysfs_device/device_value"

int main() {
    int fd;
    char buffer[256];
    ssize_t bytes_read, bytes_written;
    int value;

    // Open the sysfs attribute
    fd = open(SYSFS_PATH, O_RDWR);
    if (fd < 0) {
        perror("Failed to open sysfs file");
        return EXIT_FAILURE;
    }

    // Read the current value
    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0) {
        perror("Failed to read from sysfs file");
        close(fd);
        return EXIT_FAILURE;
    }

    buffer[bytes_read] = '\0';
    printf("Current sysfs value: %s", buffer);

    // Write a new value
    printf("Enter a new value: ");
    scanf("%d", &value);
    snprintf(buffer, sizeof(buffer), "%d", value);
    bytes_written = write(fd, buffer, sizeof(buffer));
    if (bytes_written < 0) {
        perror("Failed to write to sysfs file");
        close(fd);
        return EXIT_FAILURE;
    }

    printf("New value written successfully.\n");

    close(fd);
    return EXIT_SUCCESS;
}
