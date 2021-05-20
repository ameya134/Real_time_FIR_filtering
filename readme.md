# Real time Digital FIR filtering

* Author:	Ameya Phadke
* License:	GPL 2.0



## About

This project is a real time digital filtering program that runs on Tiva C board(tm4c129exl).
When flashed on tiva the program will continuously sample the analog input at standard
audio frequency 48KHz and apply an FIR filter. The result will be outputed through the PWM.
All the data transfers are accelerated through the use micro DMA. This project served the 
purpose of learning about various Tiva C peripherals, FreeRTOS, Digital Signal Processing 
as well as learning about system design principals.


## Get Started

1.      Clone the Repository on to your machine

2.      build the project by issuing 'make all' command in terminal from root directory of the repo.

3.      The elf file generated can be loaded on to ek-tm4c129exl by using either openocd with on board 
        icdi debugger or alternatevely any other usb flashser tool can be used. Openocd can also be
        used with Visual Studio Code as done in this project. The settings for vscode can be found
        in .vscode directory.

        For using openocd, issue following commands in terminal from root dir of repo clone

        i)	make load

        Open another terminal window and enter

        ii)	telnet localhost 4444

        In the same window

        iii) reset init

        iv)	 flash write_image erase tm4c.elf

        v)	reset

        vi) shutdown (used to exit openocd )

Now the Real Time FIR digital filtering code will be running on your controller !



## Seeing the log data of the Application.

Once the program is loaded on to tm4c the board can be disconnected and the program will
remain in the flash. To see the Messages printed by the program follow the steps below.

1. Connect the tm4c evaluation board to your pc using a usb cable

2. Open any serial communication program like putty or cu.

3. Connect using the following settings
	* 115200 baudrate
        * 8 bit data
        * parity none
        * stop bit 1
        * (if using putty )change the backspace key to use control-H 


4.	After connection is done press reset on board and you will be greated with welcome
	screen on terminal.



## How the program works ?
The program makes use of FreeRTOS to run its tasks on cortex m4 cpu. The program first
initializes all the reqiured peripherals and clocks. It then creates the FreeRTOS tasks for
led blink as well as for the FIR filter task.

### TIMER0 trigger
The program first initializes the on chip TIMER0 peripheral to generate a trigger at 48KHz.
This trigger is used to sample the input pin by ADC at the required frequency. This TIMER0
frequency can be changed to any desired sampling frequency. TIMER0 triggers are also used
to output data to PWM at the same frequency. after setting up TIMER0 it is disabled until
FreeRTOS tasks are not initialized.

### ADC sampling
ADC is initialized such that every time the TIMER0 generates a trigger the ADC samples the
analog input pin 0 (AIN0). This results in ADC sampling the input at the desired sampling
frequency. The 12 bit ADC samples are then stored inside ADC FIFO register. When the ADC
FIFO reaches half full level it generates a DMA transfer request. The DMA is setup to
transfer some fixed number of samples after which the DMA generates a DMA done signal.
The ADC generates a DMA done interrupt after this signal is generated.

### DMA transfers
The on chip DMA is used to transfer data to and from different peripherals on the chip
without the need of CPU intervention. The DMA is set to transfer the ADC samples from
the ADC FIFO to a designated input array in the RAM. The DMA is also set up to transfer
the samples from output buffer in RAM to the PWM peripheral. The DMA is reconfigured
after the transfer is done inside the ADC dma transfer complete ISR.

### PWM ouput
The pwm module is set to ouput the data generated after applying the digital filter to 
the input signal. The DMA transfers 1 byte of data from the output buffer to the PWM
module every time the TIMER0 generates a trigger. Hence the output frequency is maintained
the same as input frequency.

### Dual buffer logic
The program uses a dual buffer logic to hold and process the input and the output samples.
Initially the hardware uses buffer A for input and output. This means that the ADC will
initially save the sampled data in input buffer A and the dma will output data to pwm
from a 0x0 initialized output buffer A. When set number of input samples are stored and
set number of output samples are transfered to output the hardware starts using B buffers
for input and output. This is when the software uses input buffer A to compute the output and
stores it in output buffer A. Similarly when the hardware completes the data transfers for
buffer B the software uses it to compute the output and stores it in output buffer B.

### ADC dma_done interrupt
When DMA is done transferring a set amount of ADC samples into the input buffer it generates
a dma done signal after which the ADC0 generates an interrupt on Sample sequencer 0 line.
The interrupt service routine for adc interrupt is responsible changing the buffers used by
the DMA to transfer the input and output data. Here the DMA channels for the input and output
are reconfigured to use the buffer other than previous one. Also the ADC ISR release the
ADC_data_ready semaphore which the FIR_filter_task waits on. This ensures that the FIR task
always processes the new data every time a set number of samples have been transfered to the
input buffer and fixed number of samples have been sent to the output pwm.

### FIR filter task
FIR_filter_task is created by the app_task_init function. This task is responsible for computing
the output of the FIR filter. This task pends on the ADC_data_ready semaphore. When the task
aquires the semaphores it pre-processes the input output buffers with data from previous output
as the application uses OVERLAP ADD METHOD to maintain a continous real time stream of output
data. After this the task computes the convolution between the input data and the FIR filter
impulse response sequence. This data is stored in the output buffer and the task again waits
on the ADC_buffer_ready semaphore to be realesed by the ADC interrupt when new data is available
for processing.


## Directory Structure
The application has following directory structure
```
/-
  |
  |
  |
  |
  |
  All the makefiles,
  executable
  debug related files
  |
  |
  ---------src/---------Application related source files
			|
			|
			------hal/------HAL source files
			|
			|
                        |
                        ------FreeRTOS/------FreeRTOS source files
                        |
                        |
			|
			------inc/------Application related headers
					|
                                        |
                                        ------FreeRTOS/-----FreeRTOS include files
                                        |
					|
					-------hal/-----HAL headers
					|
					-------core/----microcontroller
							specific headers

```

## Future scope and modifications

* The sampling frequency can be easily changed from the app.h header file.
* There is a tradeoff between bit depth and speed as SIMD instructions can be
  used to increase the speed by reducing the bit depth of the data processing
  pipeline.