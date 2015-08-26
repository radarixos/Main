/*===========================================================*/
/*====================*/ //#define DEBUG /*====================*/
/*===========================================================*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Подключение библиотек
#include <SPI.h>
#include <Ethernet.h>										
#include <PubSubClient.h>
#include <Wire.h>	
#include <ExtPins.h>										
#include <artl.h>
#include <MFRC522.h>
#include <MsTimer2.h>
#include <avr/pgmspace.h>
#include <RFIDuart.h>
#include <avr/wdt.h>
#include <WiznetWatchdog.h>
#include <dBool.h>
#include <MatrixKeyboard.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Макросы
#define ROWS 			4
#define COLUMNS			3
#define NUMBER_BUTTON 	(ROWS * COLUMNS)
#define DEBOUNCE 		20
#define SLAVE_ADDRESS 	119
#define NUMBER_BYTE_MESSAGE ( NUMBER_BUTTON/8 + ((bool)NUMBER_BUTTON%8) )

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Глобальные переменные

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Прототипы и объявления
RCPins 		Rows (ROWS, 4, 5, 6, 7);
RCPins 		Columns (COLUMNS, 8, 9, 10);
MatrixKB 	MKB(Rows, Columns, DEBOUNCE);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Начальная настройка
void setup() 
{
	Wire.begin(SLAVE_ADDRESS);
	Wire.onRequest(onRequestSlave);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Основной цикл
void loop()
{
	MKB.loop();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Обработка прерываний

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Функции 

void onRequestSlave()
{
	uint8_t buffer[NUMBER_BYTE_MESSAGE + 1];

	for (uint8_t i = 0; i < NUMBER_BYTE_MESSAGE; i++) {
		buffer[i] = MKB.getDStruct()[i];
	}
	buffer[NUMBER_BYTE_MESSAGE] = calculateCRC();
	Wire.write(buffer, NUMBER_BYTE_MESSAGE + 1);
}

uint8_t calculateCRC()
{
	uint32_t temp = 0;
	for (uint8_t i = 0; i < NUMBER_BYTE_MESSAGE; i++) {
		temp += ((uint32_t)MKB.getDStruct()[i]) << ((i * 8) + 7);
	}

	return temp % 181;
}