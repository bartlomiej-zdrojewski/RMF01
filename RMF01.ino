#include "RMF01.h"

void setup ( ) {
  
  RMF01::Init( RMF01::PROFILE_LONG_RANGE, RMF01::BAND_433_MHZ, RMF01::GetFrequency( RMF01::BAND_433_MHZ, 433000000 ), RMF01::GetBaud( 4800 ) );

  }

void loop ( ) {
  
  if ( RMF01::IsReady() ) {

    RMF01::Update();

    if ( RMF01::GetData().Valid ) {

      uint8_t Data = RMF01::GetData().Data;

      // SHOW
      // Data

      }

    else if ( RMF01::GetStatus().Interrupt ) {

      RMF01::StatusStruct Status = RMF01::GetStatus();
      
      // SHOW
      // Status.WakeUp
      // Status.LowVoltage
      // Status.BufferOverflow

      }

    else {

      // SHOW
      // "Unexpected behaviour"

      } }
  
  }

