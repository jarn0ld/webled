from django.shortcuts import render
from django.http import HttpResponse

import socket
WS2811D_IP = '127.0.0.1'
WS2811D_PORT = 50000

# Create your views here.
def index(request):
    context = {}
    return render(request, 'led/interface_ws2812.html', context)

def api_set_color(request, r, g, b):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto(chr(int(r))+chr(int(g))+chr(int(b)), (WS2811D_IP, WS2811D_PORT))
    #sock.sendto('\x00\xff\xff', (WS2811D_IP, WS2811D_PORT))
    return HttpResponse("OK")

def api_off(request):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.sendto('\x00\x00\x00', (WS2811D_IP, WS2811D_PORT))
    return HttpResponse("OK")
