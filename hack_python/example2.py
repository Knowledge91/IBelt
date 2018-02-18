import platform

import serial
import time
import sys

def send_data(inst, data, ser):
    if ser:
        inst.write(data)
    else:
        inst.send(data)

if len(sys.argv) != 2:
    print 'USAGE: python example2.py {port|name}'
    print 'Windows EXAMPLE: python example2.py COM4'
    print 'Linux EXAMPLE: python example2.py Flora_1; Be carefull, linux uses the actual name of the device not the port...'
    print 'Mac EXAMPLE: python example2.py /dev/tty.bluetoothport'
    exit(-2);

if platform.system() == 'Darwin' or platform.system() == 'Windows':
    bp = serial.Serial(str(sys.argv[1]), timeout=0, baudrate=9600, xonxoff=False, rtscts=False, dsrdtr=False)
    serial = True
else:
    from blue import blue
    bp = blue()
    dev = bp.scanDevices(str(sys.argv[1]))
    serial = False
    if len(dev) > 0:
        bp.connectDevice(dev[0]['addr'])
    else:
        exit(-1)

time.sleep(1)
send_data(bp, "N;0;255;255;255\n", serial)
time.sleep(1)
send_data(bp, "N;0;000;255;000\n", serial)
time.sleep(1)
send_data(bp, "N;0;255;000;000\n", serial)
time.sleep(1)
send_data(bp, "N;0;000;000;255\n", serial)
time.sleep(1)

from http.server import BaseHTTPRequestHandler, HTTPServer
import logging
import simplejson as json

class S(BaseHTTPRequestHandler):
    def _set_response(self):
        self.send_response(200)
        self.send_header('Content-type', 'text/html')
        self.end_headers()

    def do_GET(self):
        send_data(bp, "F;\n", serial)
        logging.info("GET request,\nPath: %s\nHeaders:\n%s\n", str(self.path), str(self.headers))
        self._set_response()
        self.wfile.write("GET request for {}".format(self.path).encode('utf-8'))

    def do_POST(self):
        content_length = int(self.headers['Content-Length']) # <--- Gets the size of data
        post_data = self.rfile.read(content_length) # <--- Gets the data itself
        logging.info("POST request,\nPath: %s\nHeaders:\n%s\n\nBody:\n%s\n",
                str(self.path), str(self.headers), post_data.decode('utf-8'))
        body = json.loads(post_data)
        intent = body['result']['metadata']['intentName']
        if intent == "activate":
                send_data(bp, "N;0;000;255;000\n", serial)
        elif intent == "deactivate":
            send_data(bp, "N;0;255;000;000\n", serial)

        self._set_response()
        self.wfile.write("POST request for {}".format(self.path).encode('utf-8'))

def run(server_class=HTTPServer, handler_class=S, port=5000):
    logging.basicConfig(level=logging.INFO)
    server_address = ('', port)
    httpd = server_class(server_address, handler_class)
    logging.info('Starting httpd...\n')
    try:
        httpd.serve_forever()
    except KeyboardInterrupt:
        pass
    httpd.server_close()
    logging.info('Stopping httpd...\n')

if __name__ == '__main__':
    from sys import argv

    if len(argv) == 2:
        run(port=int(5000))
    else:
        run()
