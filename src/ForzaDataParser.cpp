#include "ForzaDataParser.h"

ForzaDataParser::ForzaDataParser(int port) : port(port) {}

void ForzaDataParser::begin()
{
  udp.begin(port);
  Serial.printf("UDP server started on port %d\n", port);
}

void ForzaDataParser::update()
{
  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    udp.read(packetBuffer, UDP_BUFFER_SIZE);
    decodeData(packetBuffer);
  }
}

void ForzaDataParser::decodeData(const uint8_t *buffer)
{
  memcpy(&data, buffer, sizeof(ForzaData));
}

const ForzaData &ForzaDataParser::getData() const
{
  return data;
}
