#ifndef RMF01_MODULE
#define RMF01_MODULE

#include <avr/io.h>
#include <stdint.h>
#include <math.h>

/*
#define RMF01_SCK_DDR   DDRB
#define RMF01_SDI_DDR   DDRB
#define RMF01_SDO_DDR   DDRB
#define RMF01_SEL_DDR   DDRB
#define RMF01_INT_DDR   DDRB

#define RMF01_SCK_PORT  PORTB
#define RMF01_SDI_PORT  PORTB
#define RMF01_SDO_PORT  PORTB
#define RMF01_SEL_PORT  PORTB
#define RMF01_INT_PORT  PORTB

#define RMF01_SCK_PIN   0
#define RMF01_SDI_PIN   0
#define RMF01_SDO_PIN   0
#define RMF01_SEL_PIN   0
#define RMF01_INT_PIN   0
*/

#define RMF01_SCK_DDR   DDRB
#define RMF01_SDI_DDR   DDRB
#define RMF01_SDO_DDR   DDRB
#define RMF01_SEL_DDR   DDRB
#define RMF01_INT_DDR   DDRB

#define RMF01_SCK_PORT  PORTB
#define RMF01_SDI_PORT  PORTB
#define RMF01_SDO_PORT  PORTB
#define RMF01_SEL_PORT  PORTB
#define RMF01_INT_PORT  PORTB

#define RMF01_SCK_PIN   0
#define RMF01_SDI_PIN   0
#define RMF01_SDO_PIN   0
#define RMF01_SEL_PIN   0
#define RMF01_INT_PIN   0

namespace RMF01 {

  enum BAND {

    BAND_315_MHZ,
    BAND_433_MHZ,
    BAND_868_MHZ,
    BAND_915_MHZ

    };

  enum BANDWIDTH {

    BANDWIDTH_67_KHZ,
    BANDWIDTH_134_KHZ,
    BANDWIDTH_200_KHZ,
    BANDWIDTH_270_KHZ,
    BANDWIDTH_340_KHZ,
    BANDWIDTH_400_KHZ

    };

  enum LNA_GAIN {

    LNA_GAIN_0_DBM,
    LNA_GAIN_MINUS_6_DBM,
    LNA_GAIN_MINUS_14_DBM,
    LNA_GAIN_MINUS_20_DBM

    };

  enum SIGNAL_THRESHOLD {   // To obtain the actual signal threshold, you have to add the LNA gain value.

    SIGNAL_THRESHOLD_MINUS_73_DBM,  // Equal to 5.01 * 10^(-8) mW
    SIGNAL_THRESHOLD_MINUS_79_DBM,  // Equal to 1.26 * 10^(-8) mW
    SIGNAL_THRESHOLD_MINUS_85_DBM,  // Equal to 3.16 * 10^(-9) mW
    SIGNAL_THRESHOLD_MINUS_91_DBM,  // Equal to 7.94 * 10^(-10) mW
    SIGNAL_THRESHOLD_MINUS_97_DBM,  // Equal to 2.00 * 10^(-10) mW
    SIGNAL_THRESHOLD_MINUS_103_DBM  // Equal to 5.01 * 10^(-11) mW

    };

  enum SIGNAL_DEVATION {

    SIGNAL_DEVATION_7_STEPS,
    SIGNAL_DEVATION_15_STEPS,
    SIGNAL_DEVATION_31_STEPS,
    SIGNAL_DEVATION_UNLIMITED

    };

  enum AFC {

    AFC_POWERUP,
    AFC_LOW_SIGNAL,
    AFC_STEADY

    };

  enum VDI {

    VDI_DIGITAL_RSSI,
    VDI_DATA_QUALITY_DETECTOR,
    VDI_DIGITAL_RSSI_AND_DATA_QUALITY_DETECTOR

    };

  enum DQF {

    DQF_0,
    DQF_1,
    DQF_2,
    DQF_3,
    DQF_4,  // Default data quality factor
    DQF_5,
    DQF_6,
    DQF_7

    };

  enum INTERRUPT {

    INTERRUPT_NONE              = 0x00,
    INTERRUPT_WAKE_UP           = 0x01,
    INTERRUPT_LOW_VOLTAGE       = 0x02

    };

  enum PROFILE {

    PROFILE_AVERANGE,               // Parameters are medium bandwidth, medium gain, medium signal threshold, medium signal devation, frequency control on low signal, digital rssi and data quality detector as data indicators, high accuracy mode.
    PROFILE_LOW_POWER,              // Parameters are medium bandwidth, medium gain, medium signal threshold, low signal devation, steady frequency control, digital rssi as data indicator, low accuracy mode, low power mode.
    PROFILE_LOW_NOISE,              // Parameters are narrow bandwidth, low gain, high signal threshold, low signal devation, frequency control on power up, digital rssi and data quality detector as data indicators, and high accuracy mode.
    PROFILE_LONG_RANGE,             // Parameters are narrow bandwidth, high gain, low signal threshold, high signal devation, frequency control on low signal, digital rssi and data quality detector as data indicators, high accuracy mode. Baud (bitrate) value should be as low as possible.
    PROFILE_MULTIPLE_TRANSMITTERS   // Parameters are wide bandwidth, medium gain, medium threshold, high signal devation, frequency control on low signal, digital rssi and data quality detector as data indicators, low accuracy mode.

    };

  struct DataStruct {

    bool Valid;
    uint8_t Data;

    };

  struct StatusStruct {

    bool Interrupt;           // Preprogrammed interrupt occurred
    bool WakeUp;              // Wake-up interrupt occurred
    bool LowVoltage;          // Low voltage interrupt occurred
    bool BufferOverflow;      // Buffer overflow interrupt occurred
    bool BufferEmpty;         // Data buffer is empty

    bool StrongSignal;        // Digital RSSI detected a strong signal
    bool GoodQualitySignal;   // Data Quality Detector detected a good quality signal
    bool FrequencyToggling;   // Frequency controller is in toggling cycle  
    bool FrequencyStable;     // Frequency is stable
    
    uint8_t SignalDevation;   // Signal frecuency devation in steps

    };
  
  void Init ( BAND Band, BANDWIDTH Bandwidth, uint16_t Frequency, uint8_t Baud, LNA_GAIN Gain, SIGNAL_THRESHOLD Threshold, SIGNAL_DEVATION Devation, AFC FrequencyControl, VDI DataIndicator, DQF DataQualityFactor, bool HighAccuracyMode, bool LowPowerMode, uint16_t WakeUpTime, uint8_t DutyCycle, uint8_t LowVoltage, uint8_t Interrupt, bool InitSPI );
  void Init ( PROFILE Profile, BAND Band, uint16_t Frequency, uint8_t Baud, uint16_t WakeUpTime = 0x0001, uint8_t DutyCycle = 0x00, uint8_t LowVoltage = 0xE0, uint8_t Interrupt = INTERRUPT_NONE, bool InitSPI = true );
  
  uint8_t Command ( uint8_t Data );
  
  bool IsReady ( );
  void Update ( );
  void Reset ( );
  
  DataStruct GetData ( );
  StatusStruct GetStatus ( );
  
  uint16_t GetFrequency ( BAND Band, uint32_t Hertz );
  uint8_t GetBaud ( uint32_t Bitrate );
  uint32_t GetBitrate ( uint8_t Baud );
  uint16_t GetWakeUpTime ( uint32_t Milliseconds );
  uint8_t GetDutyCycle ( uint32_t Milliseconds, uint8_t Percent );
  uint8_t GetLowVoltage ( uint16_t Millivolts );
  
  extern uint8_t Data;
  extern uint16_t Status;
  
  }

#endif
