# Example: Clock Synchronization

This is an example project for synchronizing clocks of end devices using class C.

In LoRaWAN, there are at least three ways for synchronizing end device clocks:

1. Using [application layer clock synchronization](https://lora-alliance.org/resource-hub/lorawan-application-layer-clock-synchronization-specification-v100). This requires request/response messages per end device and do not guarantee exact time synchronization between end devices;
2. Using class B beacons. This requires gateways with GPS sync and the end device to be aware of time zone and leap seconds;
3. Using class C messages. This requires the end device only to be aware of time zone.

This is an example application for method 3: scheduling a class C message at a fixed time, with the absolute time as message payload. Since this is downlink only, this method can be used for unicast and multicast sessions.

## Requirements

1. A LoRaWAN network powered by The Things Network Stack V3 for LoRaWAN. If you do not have a network available, please see the [Getting Started](https://github.com/TheThingsNetwork/lorawan-stack/blob/master/doc/gettingstarted.md).
2. A LoRa® end device capable of running the Arm Mbed OS, for example the [ST B-L072Z-LRWAN1 LoRa®Discovery kit](https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html) or [L-TEK FF1705](https://l-tek.si/web-shop/ltek-ff1705/).
3. A gateway that has either GPS lock or provides round-trip times (RTTs). When relying on RTTs, gateways running LoRa Basics™ Station or Semtech UDP Packet Forwarder 4.0 or higher (sending `TX_ACK`) can be used.

## Getting Started

1. Provision an end device in the LoRaWAN stack supporting class C, for example:
   ```sh
   $ ttn-lw-cli end-devices create app1 dev1 \
     --dev-eui 0004A30B001C0530 \
     --app-eui 800000000000000C \
     --frequency-plan-id EU_863_870 \
     --root-keys.app-key.key 752BAEC23EAE7964AF27C325F4C23C9A \
     --lorawan-version 1.0.3 \
     --lorawan-phy-version 1.0.3-a
     --supports-class-c true
   ```
2. [Configure, compile and run the device application](./mbed-os)
3. [Configure, compile and run the controller](./controller)

## Example

Controller output:

```
sync clock dev1: now = 2019-05-10T12:13:33.275Z; target = 2019-05-10T12:13:43.000Z; unix time = 1557490423
```

Device output:

```
Received message on port 2 (4 bytes): f7 6a d5 5c 
Synchronized clock to Fri May 10 12:13:43 2019
```
