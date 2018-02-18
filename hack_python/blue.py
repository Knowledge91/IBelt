# simple inquiry example
import platform
if platform.system() != 'Darwin' and platform.system() != 'Windows':
    import bluetooth
import threading


class blue(threading.Thread):

    def __init__(self):
        threading.Thread.__init__(self)

        self.port = 1
        self.sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
        self.finish = False

    def scanDevices(self, name=None):
        nearby_devices = bluetooth.discover_devices(lookup_names=True)
        print("found %d devices" % len(nearby_devices))
        dev = []
        for addr, hrname in nearby_devices:
            print("  %s - %s" % (addr, hrname))
            if isinstance(name, str):
                if hrname.find(name) != -1:
                    dev.append({'name': hrname, 'addr': addr})
            else:
                dev.append({'name': hrname, 'addr': addr})
        return dev

    def connectDevice(self, addr):
        try:
            self.sock.connect((addr, self.port))
            return 1
        except bluetooth.btcommon.BluetoothError as error:
            print error
            return -2
        except:
            return -1
    def send(self, data):
        self.sock.send(data)

    def receiveLine(self):
        msg = ''
        while True:
            d = self.sock.recv(1)
            msg = msg + d
            if d == '\n':
                break
        print msg
        return msg

    def __del__(self):
        self.sock.close()




if __name__ == "__main__":
    import time
    bp = blue()
    dev = bp.scanDevices("Flora-1")
    print dev
    if len(dev) > 0:
        print bp.connectDevice(dev[0]['addr'])
        time.sleep(1)
        bp.send("N;0;255;255;255\n")
        time.sleep(1)
        bp.send("N;0;000;255;000\n")
        time.sleep(1)
        bp.send("N;0;255;000;000\n")
        time.sleep(1)
        bp.send("N;0;000;000;255\n")
        time.sleep(1)
    else:
        print "device not found"
