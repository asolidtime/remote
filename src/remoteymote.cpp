/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/home/maxwell/CTD-IoT/capstone/remoteymote/src/remoteymote.ino"
#include "LIS3DH.h"
#include "Wire.h"

void setup();
void loop();
#line 4 "/home/maxwell/CTD-IoT/capstone/remoteymote/src/remoteymote.ino"
SYSTEM_THREAD(ENABLED);

// This sample only uses serial, not data, so cellular is turned off to save data
SYSTEM_MODE(MANUAL);
#define PRINT_SAMPLE_PERIOD 100

const size_t UART_TX_BUF_SIZE = 20;
unsigned long long lastTime = 0;

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

unsigned long lastPrintSample = 0;

LIS3DHI2C accel(Wire);

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) {
  // if (String(data[0]).equals("49")) { // i could probably ditch string conversion but this works and honestly I'm tired of doing this lab lol
  //   display.fillScreen(WHITE);
  //   display.setCursor(0, 0);
  //   display.setTextColor(BLACK);
  //   display.setTextSize(4);
  //   display.println("Hell");
  //   display.display();
  // } else if (String(data[0]).equals("48")) {
  //   BLE.disconnect();
  // }

  // todo
}
// setup() runs once, when the device is first turned on.
void setup() {
  // Put initialization like pinMode and begin functions here.
  BLE.on();

  BLE.addCharacteristic(txCharacteristic);
  BLE.addCharacteristic(rxCharacteristic);

  BleAdvertisingData data;
  data.appendServiceUUID(serviceUuid);
  data.appendLocalName("maximus");  //max 8 characters 
  BLE.advertise(&data);
  uint8_t txBuf[20]; // an array of 10 bytes we can send
  txCharacteristic.setValue(txBuf, 20);  // here we are sending all 10 bytes

  Wire.setSpeed(CLOCK_SPEED_400KHZ);
  Wire.begin();

  LIS3DHConfig config;
	config.setAccelMode(LIS3DH::RATE_400_HZ);

	bool setupSuccess = accel.setup(config);
	//Serial.printlnf("setupSuccess=%d", setupSuccess);
  

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  // The core of your code will likely live here.

  if (millis() - lastPrintSample >= PRINT_SAMPLE_PERIOD) {
		lastPrintSample = millis();

		LIS3DHSample sample;
		if (accel.getSample(sample) && BLE.connected()) {
			//Serial.printlnf("%d,%d,%d", sample.x, sample.y, sample.z);

      uint8_t txBuf[UART_TX_BUF_SIZE];
      String message = String(sample.x) + String(" ") + String(sample.y) + String(" ") + String(sample.z);
      message.toCharArray((char *)txBuf, message.length() + 1);
      txCharacteristic.setValue(txBuf, message.length() + 1); 

		}
	}

}