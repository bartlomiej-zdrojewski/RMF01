#include "RMF01.h"

namespace RMF01 {
  
  uint8_t Data;
  uint16_t Status;
  
  }

//
// Initialises the RMF01 device using custom settings.
//
void RMF01::Init ( BAND Band, BANDWIDTH Bandwidth, uint16_t Frequency, uint16_t Baud, LNA_GAIN Gain, SIGNAL_THRESHOLD Threshold, SIGNAL_DEVATION Devation, AFC FrequencyControl, VDI DataIndicator, DQF DataQualityFactor, bool HighAccuracyMode, bool LowPowerMode, uint16_t WakeUpTime, uint8_t DutyCycle, uint8_t LowVoltage, INTERRUPT Interrupt, bool InitSPI ) {

  for ( uint16_t i = 0; i < 1024; i++ ); // Waiting for RMF01's clock to stabilize
  
  if ( InitSPI ) {

    RMF01_SCK_DDR |= ( 1 << RMF01_SCK_PIN );
    RMF01_SDO_DDR |= ( 1 << RMF01_SDO_PIN );
    RMF01_SEL_DDR |= ( 1 << RMF01_SEL_PIN );

    RMF01_SDI_DDR &= ~( 1 << RMF01_SDI_PIN );
    RMF01_INT_DDR &= ~( 1 << RMF01_INT_PIN );
    
    SPCR = ( 1 << SPE ) | ( 1 << MSTR ); }

  RMF01_SEL_PORT &= ~( 1 << RMF01_SEL_PIN );

  uint8_t CMDL = 0x80;
  uint8_t CMDR = 0xF1; // Parameters are 16pF load capacitor, clock output disabled.

  switch ( Band ) {
    
    }

  switch ( Bandwidth ) {
    
    }

  switch ( Interrupt ) {
    
    }
  
  Command( CMDL );
  Command( CMDR );
  
  Command( 0xA0 | ( ( Frequency >> 8 ) & 0x0F ) );
  Command( Frequency >> 8 );

  // Data Rate Command 
  // Wake-Up Timer Command
  // Low Duty-Cycle Command 
  // Low Battery Detector and Microcontroller  Clock Divider Comman -> 10Mhz

  Command( 0xDA );
  Command( 0x00 );

  Command( 0xCE );
  Command( 0x80 );
  Command( 0xCE );
  Command( 0x83 );
  
  // AFC Command 
  // Data Filter Command

  CMDR = 0x01;

  switch ( DataIndicator ) {
    
    }
  
  switch ( Gain ) {
    
    }

  switch ( Threshold ) {
    
    }

  Command( 0xC0 );
  Command( CMDR );
  
  RMF01_SEL_PORT |= ( 1 << RMF01_SEL_PIN );

  Update(); }

//
// Initialises the RMF01 device using profile.
//
void RMF01::Init ( PROFILE Profile, BAND Band, uint16_t Frequency, uint16_t Baud, uint16_t WakeUpTime, uint8_t DutyCycle, uint8_t LowVoltage, INTERRUPT Interrupt, bool InitSPI ) {

  switch ( Profile ) {

    case PROFILE_AVERANGE : Init( Band, BANDWIDTH_200_KHZ, Frequency, Baud, LNA_GAIN_MINUS_6_DBM, SIGNAL_THRESHOLD_MINUS_91_DBM, SIGNAL_DEVATION_31_STEPS, AFC_LOW_SIGNAL, VDI_DIGITAL_RSSI_AND_DATA_QUALITY_DETECTOR, DQF_4, false, false, WakeUpTime, DutyCycle, LowVoltage, Interrupt, InitSPI ); break;
    case PROFILE_LOW_POWER : Init( Band, BANDWIDTH_200_KHZ, Frequency, Baud, LNA_GAIN_MINUS_6_DBM, SIGNAL_THRESHOLD_MINUS_91_DBM, SIGNAL_DEVATION_31_STEPS, AFC_STEADY, VDI_DIGITAL_RSSI, DQF_4, false, true, WakeUpTime, DutyCycle, LowVoltage, Interrupt, InitSPI ); break;
    case PROFILE_LOW_NOISE : Init( Band, BANDWIDTH_67_KHZ, Frequency, Baud, LNA_GAIN_MINUS_20_DBM, SIGNAL_THRESHOLD_MINUS_73_DBM, SIGNAL_DEVATION_7_STEPS, AFC_POWERUP, VDI_DIGITAL_RSSI_AND_DATA_QUALITY_DETECTOR, DQF_6, true, false, WakeUpTime, DutyCycle, LowVoltage, Interrupt, InitSPI ); break;
    case PROFILE_LONG_RANGE : Init( Band, BANDWIDTH_67_KHZ, Frequency, Baud, LNA_GAIN_0_DBM, SIGNAL_THRESHOLD_MINUS_103_DBM, SIGNAL_DEVATION_31_STEPS, AFC_LOW_SIGNAL, VDI_DIGITAL_RSSI_AND_DATA_QUALITY_DETECTOR, DQF_4, true, false, WakeUpTime, DutyCycle, LowVoltage, Interrupt, InitSPI ); break;
    case PROFILE_MULTIPLE_TRANSMITTERS : Init( Band, BANDWIDTH_400_KHZ, Frequency, Baud, LNA_GAIN_MINUS_6_DBM, SIGNAL_THRESHOLD_MINUS_91_DBM, SIGNAL_DEVATION_7_STEPS, AFC_LOW_SIGNAL, VDI_DIGITAL_RSSI_AND_DATA_QUALITY_DETECTOR, DQF_4, false, false, WakeUpTime, DutyCycle, LowVoltage, Interrupt, InitSPI ); break;

    } }

//
// Commands the RMF01 device using SPI protocol.
//
uint8_t RMF01::Command ( uint8_t Data ) {

  SPDR = Data;
  
  while( !( SPSR & ( 1 << SPIF ) ) );
  
  return SPDR; }

//
// Checks whether the RMF01 device is ready to provide data or an programmed interrupt occurred.
//
bool RMF01::IsReady ( ) {
  
  return !( RMF01_INT_PORT & ( 1 << RMF01_INT_PIN ) ); }

//
// Reads status and data bits from the RMF01 device.
//
void RMF01::Update ( ) {

  RMF01_SEL_PORT &= ~( 1 << RMF01_SEL_PIN );

  // READ STATUS
  // READ DATA BYTE
  // ???
  // PROFIT

  // CLEAR FIFO
  Command( 0xCE );
  Command( 0x80 );
  Command( 0xCE );
  Command( 0x83 );
  // CLEAR FIFO

  RMF01_SEL_PORT |= ( 1 << RMF01_SEL_PIN ); }

//
// Resets the RMF01 device without turning it off.
//
void RMF01::Reset ( ) {

  Command( 0xFF );
  Command( 0x00 ); }

//
// Interprets the data value and returns it as a structure.
//
RMF01::DataStruct RMF01::GetData ( ) {

  DataStruct Struct;

  Struct.Valid = ( Status >> 8 ) & 0x80;
  Struct.Data = Data;

  return Struct; }

//
// Interprets the status value and returns it as a structure.
//
RMF01::StatusStruct RMF01::GetStatus ( ) {

  StatusStruct Struct;

  Struct.WakeUp = ( Status >> 8 ) & 0x20;
  Struct.LowVoltage = ( Status >> 8 ) & 0x10;
  Struct.BufferOverflow = ( Status >> 8 ) & 0x40;
  Struct.BufferEmpty = ( Status >> 8 ) & 0x08;
  Struct.Interrupt = Struct.WakeUp | Struct.LowVoltage | Struct.BufferOverflow;

  Struct.StrongSignal = ( Status >> 8 ) & 0x04; // Good Signal Strength
  Struct.GoodQualitySignal = ( Status >> 8 ) & 0x02; // Good Signal Quality
  Struct.FrequencyToggling = Status & 0x80; // Stabilizing Frequency
  Struct.FrequencyStable = Status & 0x40; // Frequency Stabilized

  Struct.SignalDevation = Status & 0x3F;

  return Struct; }

// 
// Calculates proper frequency value for the init function.
// Parameters are band and expected frequency in hertzs.
// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// BAND : STEP : MINIMUM - MAXIMUM
// 315 MHz : 2500 Hz : 310 240 000 Hz - 319 757 500 Hz
// 433 MHz : 2500 Hz : 430 240 000 Hz - 439 757 500 Hz
// 868 MHz : 5000 Hz : 860 480 000 Hz - 879 515 000 Hz
// 915 MHz : 7500 Hz : 900 720 000 Hz - 929 272 500 Hz
// 
uint16_t RMF01::GetFrequency ( BAND Band, uint32_t Hertz ) {

  uint16_t Frequency;

  if ( Band == BAND_315_MHZ ) {

    if ( Hertz < 310240000L ) {

      return 0x60; }

    if ( Hertz > 319757500L ) {

      return 0x0F3F; }

    Frequency = 0x60 + ( Hertz - 310240000L ) / 2500; }

  if ( Band == BAND_433_MHZ ) {

    if ( Hertz < 430240000L ) {

      return 0x60; }

    if ( Hertz > 439757500L ) {

      return 0x0F3F; }

    Frequency = 0x60 + ( Hertz - 430240000L ) / 2500; }

  if ( Band == BAND_868_MHZ ) {

    if ( Hertz < 860480000L ) {

      return 0x60; }

    if ( Hertz > 879515000L ) {

      return 0x0F3F; }

    Frequency = 0x60 + ( Hertz - 860480000L ) / 5000; }

  if ( Band == BAND_915_MHZ ) {

    if ( Hertz < 900720000L ) {

      return 0x60; }

    if ( Hertz > 929272500L ) {

      return 0x0F3F; }

    Frequency = 0x60 + ( Hertz - 900720000L ) / 7500; }

  return Frequency; }

// 
// Calculates proper baud value for the init function.
// Parameter is expected bitrate in bits per second.
// Bitrate range from 336 bps to 344 827 bps.
//
uint16_t RMF01::GetBaud ( uint32_t Bitrate ) {

  if ( Bitrate < 336 ) {

    return 0xFF; }

  if ( Bitrate > 344827 ) {

    return 0x00; }

  uint16_t FirstBaud = 0x00;
  uint16_t SecondBaud = 0x80;

  while ( GetBitrate( FirstBaud ) > Bitrate && FirstBaud < 0x7F ) FirstBaud++;
  while ( GetBitrate( SecondBaud ) > Bitrate ) SecondBaud++;

  if ( GetBitrate( FirstBaud ) < GetBitrate( SecondBaud ) || GetBitrate( FirstBaud ) > Bitrate ) {

    FirstBaud = SecondBaud; }

  return FirstBaud; }

// 
// Calucates bitrate value from the given baud.
//
uint32_t RMF01::GetBitrate ( uint16_t Baud ) {

  return ( 10000000L / 29 / ( ( Baud & 0x7F ) + 1 ) / ( 1 + ( ( Baud & 0x80 ) >> 7 ) * 7 ) ); }

// 
// Calculates proper wake-up time value for the init function.
// Parameter is expected time in milliseconds.
// Time range from 1 ms to 4 294 967 295 ms.
//
uint16_t RMF01::GetWakeUpTime ( uint32_t Milliseconds ) {

  uint8_t M, R;
  uint32_t D = 0xFFFFFFFF;

  if ( Milliseconds == 0 ) {

    return 0x0001; }

  for ( uint8_t _R = 0; _R < 32; _R++ ) {

    uint32_t _M = Milliseconds / pow( 2, _R );

    if ( _M > 255 ) {

      continue; }

    uint32_t _D = Milliseconds - ( _M * pow( 2, _R ) );

    if ( _D < D ) {

      M = _M;
      R = _R;
      D = _D; }

    if ( _D == 0 ) {

      break; } }

  return ( R << 8 | M ); }

// 
// Calculates proper duty-cycle value for the init function.
// Parameter are expected time in milliseconds and percent of that time.
// Time range from 1 ms to 4 294 967 295 ms and percent from 0 to 100.
//
uint8_t RMF01::GetDutyCycle ( uint32_t Milliseconds, uint8_t Percent ) {

  uint8_t M, D;

  if ( Percent > 100 ) {

    Percent = 100; }

  M = GetWakeUpTime( Milliseconds ) && 0x00FF;
  D = ( (unsigned int) ( Percent * M ) / 100 - 1 ) / 2;

  if ( D > 0x7F ) {

    D = 0x7F; }

  return ( ( D << 1 ) & 0x01 ); }

// 
// Calculates proper low voltage value for the init function.
// Parameter is expected voltage in millivolts.
// Voltage range from 2250 mV to 5350 mV.
//
uint8_t RMF01::GetLowVoltage ( uint16_t Millivolts ) {

  if ( Millivolts < 2250 ) {

    return 0x00; }

  if ( Millivolts > 5350 ) {

    return 0xFF; }

  return ( 0xE0 | (uint8_t) ( ( Millivolts - 2250 ) / 100 ) ); }
