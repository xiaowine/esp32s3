#include "IoTool.h"

std::vector<IoTool::InterruptInfo> IoTool::interruptInfos;

IoTool::IoTool() {}

void IoTool::setInterrupt(uint8_t pin, int mode)
{
  pinMode(pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pin), handleInterrupt, mode);
  interruptInfos.push_back({pin, 0, false});
}

void IoTool::handleInterrupt()
{
  unsigned long interruptTime = millis();
  for (auto &info : interruptInfos)
  {
    if (interruptTime - info.lastInterruptTime > 50)
    {
      info.lastInterruptTime = interruptTime;
      info.interruptFlag = true;
    }
  }
}

bool IoTool::checkInterrupt(uint8_t pin)
{
  for (auto &info : interruptInfos)
  {
    if (info.pin == pin && info.interruptFlag)
    {
      info.interruptFlag = false;
      return true;
    }
  }
  return false;
}