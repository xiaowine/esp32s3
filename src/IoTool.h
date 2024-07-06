#ifndef IOTOOL_H
#define IOTOOL_H

#include <Arduino.h>
#include <vector>

class IoTool
{
public:
  IoTool();
  void setInterrupt(uint8_t pin, int mode);
  static void handleInterrupt();
  bool checkInterrupt(uint8_t pin);

private:
  struct InterruptInfo
  {
    uint8_t pin;
    volatile unsigned long lastInterruptTime;
    volatile bool interruptFlag;
  };

  static std::vector<InterruptInfo> interruptInfos;
};

#endif // IOTOOL_H