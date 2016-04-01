#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>

#include <boost/asio.hpp>

#include "../rpi_ws281x/ws2811.h"

using boost::asio::ip::udp;

#define ARRAY_SIZE(stuff)                        (sizeof(stuff) / sizeof(stuff[0]))

#define TARGET_FREQ                              WS2811_TARGET_FREQ
#define GPIO_PIN                                 18
#define DMA                                      5

#define WIDTH                                    64
#define HEIGHT                                   1
#define LED_COUNT                                (WIDTH * HEIGHT)



int main(int argc, char* argv[])
{

	FILE *fp= NULL;
	pid_t process_id = 0;
	pid_t sid = 0;
	// Create child process
	process_id = fork();
	// Indication of fork() failure
	if (process_id < 0)
	{
		printf("fork failed!\n");
		// Return failure in exit status
		exit(1);
	}
	// PARENT PROCESS. Need to kill it.
	if (process_id > 0)
	{
		printf("process_id of child process %d \n", process_id);
		// return success in exit status
		exit(0);
	}
	//unmask the file mode
	umask(0);
	//set new session
	sid = setsid();
	if(sid < 0)
	{
		// Return failure
		exit(1);
	}
	// Change the current working directory to root.
	chdir("/");
	// Close stdin. stdout and stderr
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	// Open a log file in write mode.
	fp = fopen ("Log.txt", "w+");
	fprintf(fp, "Logging info...\n");
	fflush(fp);

	ws2811_t strip;
	strip.freq = TARGET_FREQ;
	strip.dmanum = DMA;
	strip.channel[0].gpionum = GPIO_PIN;
	strip.channel[0].count = LED_COUNT;
	strip.channel[0].invert = 0;
	strip.channel[0].brightness = 255;
	strip.channel[0].strip_type = WS2811_STRIP_RGB;

	strip.channel[1].gpionum = 0;
	strip.channel[1].count = 0;
	strip.channel[1].invert = 0;
	strip.channel[1].brightness = 0;
	
	ws2811_init(&strip);

	boost::asio::io_service io_service;
	udp::socket socket(io_service, udp::endpoint(udp::v4(), 50000));
	char rx_buffer[3];
	while (1)
	{
		//Dont block context switches, let the process sleep for some time
		sleep(1);
		// Implement and call some function that does core work for this daemon.
		udp::endpoint remote;
		boost::system::error_code error;
		int recv = socket.receive_from(boost::asio::buffer(rx_buffer, 3), remote, 0, error);
		fprintf(fp, "received message %d bytes: %d %d %d\n", recv, rx_buffer[0], rx_buffer[1], rx_buffer[2]);
		fflush(fp);
		for(int i = 0; i<64; i++){
			strip.channel[0].leds[i] = ((uint32_t)(((uint32_t)255)<<8));
		}
		ws2811_render(&strip);
	}
	fclose(fp);
	return (0);
}
