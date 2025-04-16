#ifndef _ENC28J60_DRIVER_H_
#define _ENC28J60_DRIVER_H_

#include "EthDriver.h"

class ENC28J60Driver : public EthSpiDriver {
public:

  ENC28J60Driver(int8_t cs = SS, int8_t irq = -1, int8_t rst = -1)
      : EthSpiDriver(cs, irq, rst) {
  }

  virtual bool read(uint32_t cmd, uint32_t addr, void *data, uint32_t data_len);
  virtual bool write(uint32_t cmd, uint32_t addr, const void *data, uint32_t data_len);

protected:
  virtual esp_eth_mac_t* newMAC();
  virtual esp_eth_phy_t* newPHY();

};

#endif
