
#include "ENC28J60Driver.h"

#include "enc28j60/esp_eth_enc28j60.h"
#include <Arduino.h>

esp_eth_mac_t* ENC28J60Driver::newMAC() {

  pinMode(pinCS, OUTPUT);
  digitalWrite(pinCS, HIGH);

  spi->begin();

  eth_enc28j60_config_t mac_config;
  mac_config.int_gpio_num = digitalPinToGPIONumber(pinIRQ);
  mac_config.poll_period_ms = (pinIRQ < 0) ? 10 : 0;
  initCustomSPI(mac_config.custom_spi_driver);

  eth_mac_config_t eth_mac_config = ETH_MAC_DEFAULT_CONFIG();
  return esp_eth_mac_new_enc28j60(&mac_config, &eth_mac_config);
}

esp_eth_phy_t* ENC28J60Driver::newPHY() {
  eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();
  phy_config.phy_addr = phyAddr;
  phy_config.reset_gpio_num = digitalPinToGPIONumber(pinRst);

  return esp_eth_phy_new_enc28j60(&phy_config);
}

bool ENC28J60Driver::read(uint32_t cmd, uint32_t addr, void* data, uint32_t data_len) {
  spi->beginTransaction(SPISettings(1000000L * spiFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(pinCS, LOW);

  // op. code is in bits 5,6,7, argument in bits 0 to 4
  spi->write((cmd << 5) | addr);
  spi->transferBytes(NULL, (uint8_t*) data, data_len);

  digitalWrite(pinCS, HIGH);
  spi->endTransaction();
  return ESP_OK;
}

bool ENC28J60Driver::write(uint32_t cmd, uint32_t addr, const void* data, uint32_t data_len) {
  spi->beginTransaction(SPISettings(1000000L * spiFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(pinCS, LOW);

  // op. code is in bits 5,6,7, argument in bits 0 to 4
  spi->write((cmd << 5) | addr);
  spi->writeBytes((const uint8_t*) data, data_len);

  digitalWrite(pinCS, HIGH);
  spi->endTransaction();
  return ESP_OK;
}
