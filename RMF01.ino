#include "RMF01.h"

void PrintStatus ( RMF01::StatusStruct Status ) { // TEMP

  Serial.println();
  Serial.println();

  Serial.print( F("Interrupt: ") );
  Serial.println( Status.Interrupt ? F("true") : F("false") );
  
  Serial.print( F("WakeUp: ") );
  Serial.println( Status.WakeUp ? F("true") : F("false") );
  
  Serial.print( F("LowVoltage: ") );
  Serial.println( Status.LowVoltage ? F("true") : F("false") );
  
  Serial.print( F("BufferOverflow: ") );
  Serial.println( Status.BufferOverflow ? F("true") : F("false") );
  
  Serial.print( F("StrongSignal: ") );
  Serial.println( Status.StrongSignal ? F("true") : F("false") );
  
  Serial.print( F("GoodQualitySignal: ") );
  Serial.println( Status.GoodQualitySignal ? F("true") : F("false") );
  
  Serial.print( F("FrequencyToggling: ") );
  Serial.println( Status.FrequencyToggling ? F("true") : F("false") );
  
  Serial.print( F("FrequencyStable: ") );
  Serial.println( Status.FrequencyStable ? F("true") : F("false") );

  Serial.print( F("SignalDevation: ") );
  Serial.println( Status.SignalDevation, DEC );
  
  Serial.println();
  
  }

uint32_t Time = 0; // TEMP

void setup ( ) {

  Serial.begin( 9600 );
  
  RMF01::Init( RMF01::PROFILE_LONG_RANGE, RMF01::BAND_433_MHZ, RMF01::GetFrequency( RMF01::BAND_433_MHZ, 433920000 ), RMF01::GetBaud( 4800 ) );
  
  }

void loop ( ) {
  
  if ( RMF01::IsReady() ) {

    RMF01::Update();

    if ( RMF01::GetData().Valid ) {

      uint8_t Data = RMF01::GetData().Data;
      
      Serial.print( Data, DEC ); // TEMP
      Serial.print( F(" ") ); // TEMP

      }

    else if ( RMF01::GetStatus().Interrupt ) {

      RMF01::StatusStruct Status = RMF01::GetStatus();

      PrintStatus( Status ); // TEMP

      }

    else {

      Serial.println( F("Unexpected behaviour!") ); // TEMP

      } }

  if ( millis() - Time > 1000 ) { // TEMP

    Time = millis();

    RMF01::Update();

    PrintStatus( RMF01::GetStatus() );
    
    }
  
  }

