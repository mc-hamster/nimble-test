/** NimBLE_Secure_Server Demo:
 *  
 * This example demonstrates the secure passkey protected conenction and communication between an esp32 server and an esp32 client.
 * Please note that esp32 stores auth info in nvs memory. After a successful connection it is possible that a passkey change will be ineffective.
 * To avoid this clear the memory of the esp32's between security testings. esptool.py is capable of this, example: esptool.py --port /dev/ttyUSB0 erase_flash.
 *
 *  Created: on Jan 08 2021
 *      Author: mblasee
 */

#include <NimBLEDevice.h>
#include <Arduino.h>

#define MESH_SERVICE_UUID "6ba1b218-15a8-461f-9fa8-5dcae273eafd"
#define TORADIO_UUID "F75C76D2-129E-4DAD-A1DD-7866124401E7"
#define FROMRADIO_UUID "8BA2BCC2-EE02-4A55-A531-C525C5E454D5"
#define FROMNUM_UUID "ED9DA18C-A800-4F66-A670-AA7547E34453"

void setup() {
  Serial.begin(115200);
  Serial.println("Starting NimBLE Server");
  NimBLEDevice::init("Meshtastic_1234");
  NimBLEDevice::setPower(ESP_PWR_LVL_P9);

  NimBLEDevice::setSecurityAuth(true, true, true);
  NimBLEDevice::setSecurityPasskey(123456);
  NimBLEDevice::setSecurityIOCap(BLE_HS_IO_DISPLAY_ONLY);
  NimBLEServer *bleServer = NimBLEDevice::createServer();
  NimBLEService *bleService = bleServer->createService(MESH_SERVICE_UUID);
  //NimBLECharacteristic *pNonSecureCharacteristic = bleService->createCharacteristic("1234", NIMBLE_PROPERTY::READ );
  NimBLECharacteristic *pSecureCharacteristic = bleService->createCharacteristic("1235", NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::READ_ENC | NIMBLE_PROPERTY::READ_AUTHEN);

  //define the characteristics that the app is looking for
  NimBLECharacteristic *ToRadioCharacteristic = bleService->createCharacteristic(TORADIO_UUID, NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_AUTHEN | NIMBLE_PROPERTY::WRITE_ENC);
  NimBLECharacteristic *FromRadioCharacteristic = bleService->createCharacteristic(FROMRADIO_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::READ_AUTHEN | NIMBLE_PROPERTY::READ_ENC);
  NimBLECharacteristic *FromNumCharacteristic = bleService->createCharacteristic(FROMNUM_UUID, NIMBLE_PROPERTY::NOTIFY);
  
  

  bleService->start();
  //pNonSecureCharacteristic->setValue("Hello Non Secure BLE");
  pSecureCharacteristic->setValue("Hello Secure BLE");

  FromRadioCharacteristic->setValue("FromRadioString");
  //ToRadioCharacteristic->setCallbacks()

  NimBLEAdvertising *pAdvertising = NimBLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(MESH_SERVICE_UUID);
  pAdvertising->start();
}

void loop() {
}