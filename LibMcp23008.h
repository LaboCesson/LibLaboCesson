//
// Librairie de gestion du MCP23008
//

#ifndef _LIB_MCP23008_H_
#define _LIB_MCP23008_H_

#include "arduino.h"
#include "wire.h"

class LibMcp23008
{
  public:
    LibMcp23008(unsigned char addrDevice = 0x20, bool doWireBegin = false);

    bool begin();
    unsigned char getDeviceRegister(unsigned char * p_reg, unsigned char nbReg);

    void pinMode( unsigned char pin, unsigned char mode);
    void digitalWrite( unsigned char pin, unsigned char value);
    unsigned char digitalRead( unsigned char pin);

  private:
    void writeRegister(unsigned char reg, unsigned char data);
    unsigned char readRegister(unsigned char reg);
    bool isConnected();

    unsigned char m_addrDevice;
  protected:
};


#endif