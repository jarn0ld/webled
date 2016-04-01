all:
	g++ -I/usr/include/boost/ -L../rpi_ws281x/ main.cpp -lws2811 -lboost_system -lboost_thread -lpthread -o static_test
	                                                                                      
	
