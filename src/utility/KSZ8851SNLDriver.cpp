
#include "KSZ8851SNLDriver.h"

#include "esp_eth_mac.h"
#include <Arduino.h>

esp_eth_mac_t* KSZ8851SNLDriver::newMAC() {

  pinMode(pinCS, OUTPUT);
  digitalWrite(pinCS, HIGH);

  spi->begin();

  eth_ksz8851snl_config_t mac_config;
  mac_config.int_gpio_num = digitalPinToGPIONumber(pinIRQ);
  mac_config.poll_period_ms = (pinIRQ < 0) ? 10 : 0;
  initCustomSPI(mac_config.custom_spi_driver);

  eth_mac_config_t eth_mac_config = ETH_MAC_DEFAULT_CONFIG();
  return esp_eth_mac_new_ksz8851snl(&mac_config, &eth_mac_config);
}

esp_eth_phy_t* KSZ8851SNLDriver::newPHY() {
  eth_phy_config_t phy_config = ETH_PHY_DEFAULT_CONFIG();
  phy_config.phy_addr = phyAddr;
  phy_config.reset_gpio_num = digitalPinToGPIONumber(pinRst);

  return esp_eth_phy_new_w5500(&phy_config);
}

bool KSZ8851SNLDriver::read(uint32_t cmd, uint32_t addr, void* data, uint32_t data_len) {
  spi->beginTransaction(SPISettings(1000000L * spiFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(pinCS, LOW);

  if (cmd > 1) {
    spi->write(cmd << 6 | addr);
  } else {
    spi->write16(cmd << 14 | addr);
  }
  spi->transferBytes(NULL, (uint8_t*) data, data_len);

  digitalWrite(pinCS, HIGH);
  spi->endTransaction();
  return ESP_OK;
}

bool KSZ8851SNLDriver::write(uint32_t cmd, uint32_t addr, const void* data, uint32_t data_len) {
  spi->beginTransaction(SPISettings(1000000L * spiFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(pinCS, LOW);

  if (cmd > 1) {
    spi->write(cmd << 6 | addr);
  } else {
    spi->write16(cmd << 14 | addr);
  }
  spi->writeBytes((const uint8_t*) data, data_len);

  digitalWrite(pinCS, HIGH);
  spi->endTransaction();
  return ESP_OK;
}
