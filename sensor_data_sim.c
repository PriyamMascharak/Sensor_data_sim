#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

/*
    Set DEBUG to 1 to see data generation every second
*/
#define DEBUG 0

/*
    suitable Data Structure to store sensor data
*/
typedef struct Arraylist
{
    unsigned char *data;
    int size;
    int capacity;
}Arraylist_t;

Arraylist_t buffer;


/* 
    funtion to initialize sensor data buffer 
*/
void initBuffer(int capacity)
{
    buffer.data = malloc(capacity*(sizeof(int)));
    buffer.size = 0;
    buffer.capacity = capacity;
}

/* 
    function to add independent data points to the buffer 
    Automaticly resizes the buffer if needed
*/
void addElement(char value)
{
    if(buffer.size+1> buffer.capacity)
    {
        buffer.capacity *=2;
        buffer.data = realloc(buffer.data, (buffer.capacity*sizeof(char)));
    }
    buffer.data[buffer.size] = value;
    buffer.size++;
}

/*
    function to add multiple data points as an array to the buffer 
*/
void addElements(char *data, int size)
{
    for(int i= 0; i<size; i++)
    {
        addElement(*(data+i));
    }
}


/*
    Simulates a sensor which geneates random number of bytes per second
    Sensor data is also random
*/
int external_Sensor_sim()
{
    int byte_num = rand() % 6;
    
    if(byte_num > 0)
    {
        char sensor_data[byte_num];
        for(int i=0; i<byte_num; i++)
        {
            sensor_data[i] = rand()%256;
        }

        addElements(sensor_data, byte_num);
        return byte_num;
    }
    else
    {
        return 0;
    }
}

/*
    Displays stored data and processess the buffer 
    to remove already displayed data
*/
void display_data()
{
    if(buffer.size > 50)
    {
        printf("50 bytes detected, printing sensor data\n");
        for(int i= buffer.size-50; i< buffer.size; i++)
        {
            printf("%02X, ", buffer.data[i]);
            if ((i - (buffer.size - 50) + 1) % 10 == 0)  //Makes the printed data more readable
                printf("\n");
        }

        printf("removing 50 bytes from buffer\n");

        memmove(buffer.data, buffer.data, buffer.size-50);
        buffer.size -=50;

    }
    else
    {
        printf("Not enough data to display\n");
    }
}

int main()
{
    //initialize random number generator
    srand(time(NULL));                  

    //set initial buffer capacity to 100
    initBuffer(100);               
    
    printf("Sensor simulation started\n");

    unsigned int second_Counter=0;
    unsigned int sensor_byte_num=0;

    while(1)
    {
        sensor_byte_num = external_Sensor_sim();

        
        #if DEBUG
        
        /*only executed if the debug flag is set to one*/
        if(sensor_byte_num>0)
            printf("Data fetched: %dbytes Time: %d\n",sensor_byte_num, second_Counter);
        else
            printf("No sensor data Time: %d\n", second_Counter);

        #endif

        if(second_Counter == UINT_MAX-2) // Prevents second_Counter from overflowing
            {
                printf("Max counter duration reached..... Reseting second_Counter value to 0");
                second_Counter = 0;
            }
        else
            second_Counter++;           // increment counter

        if(second_Counter%10 == 0)
        {
            printf("10 seconds elapsed, Time: %d, Checking Buffer\n", second_Counter);
            display_data();
        }
        sleep(1);                       //1 second delay

    }

    free(buffer.data);
    buffer.data =NULL;
    return 0;

}
