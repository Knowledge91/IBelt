import requests
import time

while(True):
    url="http://localhost:5000"
    print requests.get(url)
    time.sleep(0.1)
