//
//  BluetoothSerial.cpp
//
//
//  Created by Marc Mateu Mateus on 16/10/17.
//  Universitat Politécnica de Catalunya
//  Departament d'Enginyeria Electrónica
//  Grup d'Instrumentació Electrónica i Biomédica
//

#include "BluetoothSerial.hpp"

/**
 * The following constructor is only used to change the external name of the bluetooth device.
 */
//BSerial::BSerial(char * n) {
//  Serial1.begin(9600);
//  Serial1.print("AT");
//  delay(1000);
//  char atn[] = "AT+NAME";
//  strcat(atn, n);
//  Serial1.write(atn, strlen(atn));
//  delay(1000);
//  char atr[] = "AT+RESET";
//  Serial1.write(atr,strlen(atr));
//  Serial1.write("\r\n");
//  Serial1.flush();
//}

BSerial::BSerial() {
  Serial1.begin(9600);
}

BSerial::~BSerial() {
  Serial1.flush();
  Serial1.end();
}

int16_t BSerial::write(char *buff, uint16_t size) {
  int16_t r = Serial1.write(buff, size);
  Serial1.flush();
  return r;
}

int16_t BSerial::read(char *buff, uint16_t size) {
  char ch;
  for (uint16_t index = 0; index < size; index++) {
    while (Serial1.available() == 0) {
      //            delay(10);
    }
    char ch = Serial1.read();
    if (ch == '\n') {
      buff[index] = 0; // end of line reached: null terminate string
      return index; // success: return length of string (zero if string is empty)
    }
    buff[index] = ch; // Append character to buffer
  }
}

//Code obtained and modified from arduino.cc
int16_t BSerial::read_line(char* buffer, uint16_t bufsize)
{

  for (uint16_t index = 0; index < bufsize; index++) {
    // Wait until characters are available
    while (Serial1.available() == 0) {
      //            delay(10);
    }

    char ch = Serial1.read(); // read next character
    if (ch == '\n') {
      buffer[index] = '\0'; // end of line reached: null terminate string
      return index; // success: return length of string (zero if string is empty)
    }

    buffer[index] = ch; // Append character to buffer
  }

  // Reached end of buffer, but have not seen the end-of-line yet.
  // Discard the rest of the line (safer than returning a partial line).

  char ch;
  do {
    // Wait until characters are available
    while (Serial1.available() == 0) {
      //            delay(10);
    }
    ch = Serial1.read(); // read next character (and discard it)
    //Serial.print(ch); // echo it back
  } while (ch != '\n');

//  buffer[0] = 0; // set buffer to empty string even though it should not be used
  return bufsize-1; // error: return negative one to indicate the input was too long
}

uint16_t BSerial::findFirstChar(char *buffer, uint16_t n, char ch) {
  char *p1 = strstr(buffer, ";");
  return p1 - buffer;
}

char * BSerial::substringFromString(char *buffer, uint16_t ini, uint16_t fi, uint16_t *si) {
  char *aux;
  if ((fi - ini) >= 0) { //condition to verify that the memory will not be allocated to zero
    aux = (char *)malloc((fi - ini) * sizeof(char)); //allocation of the desired memory
    memset(aux, '\0', fi - ini); //initialization of the vector to '\0' character
  }
  else {
    aux = (char *)malloc(sizeof(char)); //if the string we want to select is null, return empty vector.
    aux[0] = '\0';
    *si = 1;
    return aux;
  }
  strncpy(aux, (const char*)(buffer + ini), (fi - ini)*sizeof(char));
  *si = fi - ini;
  return aux; //return vector.

}


