//
//  BluetoothSerial.hpp
//  
//
//  Created by Marc Mateu Mateus on 16/10/17.
//  Universitat Politécnica de Catalunya
//  Departament d'Enginyeria Electrónica
//  Grup d'Instrumentació Electrónica i Biomédica
//

#ifndef BluetoothSerial_hpp
#define BluetoothSerial_hpp

#include <stdio.h>
#include "Arduino.h"

class BSerial{
public:
//    BSerial(char * n);
    BSerial();
    ~BSerial();
    int16_t read(char *buff, uint16_t size);
    int16_t read_line(char* buffer, uint16_t bufsize);
    char * substringFromString(char *buffer, uint16_t ini, uint16_t fi, uint16_t *si);
    uint16_t findFirstChar(char *buffer, uint16_t n, char ch); //It finds the first delimiter
    int16_t write(char * buff, uint16_t size);
private:

};

#endif /* BluetoothSerial_hpp */
