# SmartFabrics Hackathon - February 2018

Our "IBelt" project: Giving blind people an eye through smart fabrics and personal assistent.
The Hardware consists of an Arduino "Flora", a bluetooth receiver, an ultrasound transmitter/ receiver and a vibrator.
The Software is to one half the arduino and to the other half a chain from a speech recognition API (dialogflow) to a local webserver.

## Arduino
- controls the P8 LED, the ultrasound module and the vibrator 
- calculates the distance from the speed of sound waves, the temperature, the moisture and the time given by the ultrasound module
- computes the intensity of the vibration
- receives bluetooth variables from the connected computer 

## Personal Assitant Chain 
Chains a chatbot on Dialogflow, which is an integrated action from Google Assistent over a online webhook given from ngrok to our localwebhost, which sends the intent over bluetooth.

## Install
1. open `hack_arduino/bluetooth_Flora.ino` 
2. load code into Flora
3. create Dialogflow intents "activate" and "deactivate"
4. connect webhook of Dialogflow to ngrok endpoint

## Usage
1. Turn on Battery
2. Connect computer to Flora via Bluetooth (Password: 1234)
3. run `hack_python/example.2 /dev/tty.Flora-9-DevB` (might have a different name, check /dev) to start the local webserver and the bluetooth communication 
4. run `hack_python/requestGET.py` to fix the bug, that the bluetooth Serial listener on the arduino Flora is blocking the code

### Info
The modules where testet in **Python2.7**
For the python `import Serial` you have to install the `pyserial` package (notice the **py** infront?)
