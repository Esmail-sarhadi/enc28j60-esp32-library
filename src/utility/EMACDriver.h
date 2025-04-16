
#ifndef _EMAC_DRIVER_H_
#define _EMAC_DRIVER_H_

#include "esp_system.h"
#include "esp_eth.h"

#include "EthDriver.h"

#ifdef CONFIG_ETH_USE_ESP32_EMAC

enum EthPhyType {
  ETH_PHY_LAN8720,
  ETH_PHY_TLK110,
  ETH_PHY_RTL8201,
  ETH_PHY_DP83848,
  ETH_PHY_KSZ80XX,
  ETH_PHY_MAX
};

class EMACDriver : public EthDriver {
public:

  EMACDriver(EthPhyType phyType, int mdcPin = 23, int mdioPin = 18, int powerPin = -1, emac_rmii_clock_gpio_t clockPin = EMAC_APPL_CLK_OUT_GPIO, emac_rmii_clock_mode_t clockMode = EMAC_CLK_EXT_IN);

  virtual bool usesIRQ() {
    return false;
  }

protected:
  virtual esp_eth_mac_t* newMAC();
  virtual esp_eth_phy_t* newPHY();

private:

  EthPhyType type;

  int8_t mdcPin;
  int8_t mdioPin;
  int8_t powerPin;
  emac_rmii_clock_gpio_t clockPin;
  emac_rmii_clock_mode_t clockMode;
};

#endif /* CONFIG_ETH_USE_ESP32_EMAC */

#endif
