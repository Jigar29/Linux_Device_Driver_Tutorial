#include <stdio.h> 
#include <stdint.h> 
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>

#define DEVICE "/dev/chardevice"

int main()
{
    int file_descriptor; 
    char op; 
    char read_buffer[100]= "", write_buffer[100]="Hello World"; 
    // Let's open the device first
    file_descriptor = open(DEVICE, O_RDWR);
    
    if(file_descriptor < 0)
    {
        printf("The Device coudn't be opened. Quitting the program\n");
        return -1;
    }

    printf("The Device has been successfully opened...\n");
    
    // printf("Please Enter the text to write to the device\n");
    // scanf("%s", &write_buffer[0]);
    write(file_descriptor, &write_buffer[0], strlen(write_buffer)+1);
    
    read(file_descriptor, &read_buffer[0], strlen(write_buffer)+1);
    printf("Data received is : %s\n", read_buffer);

    close(file_descriptor);
    return 0; 
}
