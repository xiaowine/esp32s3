#ifndef ForzaDataParser_h
#define ForzaDataParser_h

#include <WiFi.h>
#include <WiFiUDP.h>

#define UDP_BUFFER_SIZE 1500

// 数据字段的类型
enum DataType
{
  INT32,
  UINT32,
  FLOAT,
  UINT16,
  UINT8,
  INT8
};

// Forza数据结构体
struct ForzaData
{
  int32_t IsRaceOn;
  uint32_t TimestampMS;
  float EngineMaxRpm;
  float EngineIdleRpm;
  float CurrentEngineRpm;
  float AccelerationX;
  float AccelerationY;
  float AccelerationZ;
  float VelocityX;
  float VelocityY;
  float VelocityZ;
  float AngularVelocityX;
  float AngularVelocityY;
  float AngularVelocityZ;
  float Yaw;
  float Pitch;
  float Roll;
  float NormalizedSuspensionTravelFrontLeft;
  float NormalizedSuspensionTravelFrontRight;
  float NormalizedSuspensionTravelRearLeft;
  float NormalizedSuspensionTravelRearRight;
  float TireSlipRatioFrontLeft;
  float TireSlipRatioFrontRight;
  float TireSlipRatioRearLeft;
  float TireSlipRatioRearRight;
  float WheelRotationSpeedFrontLeft;
  float WheelRotationSpeedFrontRight;
  float WheelRotationSpeedRearLeft;
  float WheelRotationSpeedRearRight;
  int32_t WheelOnRumbleStripFrontLeft;
  int32_t WheelOnRumbleStripFrontRight;
  int32_t WheelOnRumbleStripRearLeft;
  int32_t WheelOnRumbleStripRearRight;
  float WheelInPuddleDepthFrontLeft;
  float WheelInPuddleDepthFrontRight;
  float WheelInPuddleDepthRearLeft;
  float WheelInPuddleDepthRearRight;
  float SurfaceRumbleFrontLeft;
  float SurfaceRumbleFrontRight;
  float SurfaceRumbleRearLeft;
  float SurfaceRumbleRearRight;
  float TireSlipAngleFrontLeft;
  float TireSlipAngleFrontRight;
  float TireSlipAngleRearLeft;
  float TireSlipAngleRearRight;
  float TireCombinedSlipFrontLeft;
  float TireCombinedSlipFrontRight;
  float TireCombinedSlipRearLeft;
  float TireCombinedSlipRearRight;
  float SuspensionTravelMetersFrontLeft;
  float SuspensionTravelMetersFrontRight;
  float SuspensionTravelMetersRearLeft;
  float SuspensionTravelMetersRearRight;
  int32_t CarOrdinal;
  int32_t CarClass;
  int32_t CarPerformanceIndex;
  int32_t DrivetrainType;
  int32_t NumCylinders;
  float HorizonPlaceholder[3]; // 3个float，大小为12字节
  float PositionX;
  float PositionY;
  float PositionZ;
  float Speed;
  float Power;
  float Torque;
  float TireTempFrontLeft;
  float TireTempFrontRight;
  float TireTempRearLeft;
  float TireTempRearRight;
  float Boost;
  float Fuel;
  float DistanceTraveled;
  float BestLap;
  float LastLap;
  float CurrentLap;
  float CurrentRaceTime;
  uint16_t LapNumber;
  uint8_t RacePosition;
  uint8_t Accel;
  uint8_t Brake;
  uint8_t Clutch;
  uint8_t HandBrake;
  uint8_t Gear;
  int8_t Steer;
  int8_t NormalizedDrivingLine;
  int8_t NormalizedAIBrakeDifference;
};

class ForzaDataParser
{
public:
  ForzaDataParser(int port);
  void begin();
  void update();
  const ForzaData &getData() const;

private:
  int port;
  WiFiUDP udp;
  uint8_t packetBuffer[UDP_BUFFER_SIZE];
  ForzaData data;

  void decodeData(const uint8_t *buffer);
};

#endif
