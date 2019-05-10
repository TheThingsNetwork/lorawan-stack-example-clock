# Example LoRaWAN application for Arm Mbed OS

This is an example application to synchronize the device clock based on Arm Mbed OS LoRaWAN protocol APIs. The Arm Mbed OS LoRaWAN stack implementation is compliant with LoRaWAN v1.0.3 specification. 

## Configuration

Open the configuration file `mbed_app.json`. This file contains the user specific configuration your application and the Mbed OS LoRaWAN stack needs.

Set `DevEUI`, `DevEUI` and `AppKey` needed for OTAA. For example:

```json
"lora.device-eui": "{ 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }",
"lora.application-eui": "{ 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }",
"lora.application-key": "{ 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }"
```

Set the region:

```json
"lora.phy": "EU868"
```

Valid values are `AS923`, `AU915`, `CN470`, `CN779`, `EU433`, `IN865`, `KR920` and `US915`.

When deploying in a duty-cycle region (i.e. `EU868`), enable duty-cycle:

```json
"lora.duty-cycle-on": true,
```

## Module support

Here is a non-exhaustive list of boards and modules that we have tested with the Mbed OS LoRaWAN stack:

- MultiTech mDot
- MultiTech xDot
- L-TEK FF1705
- Advantech Wise 1510
- ST B-L072Z-LRWAN1 LoRa®Discovery kit (with muRata radio chip)

## Compiling the application

Use [Mbed CLI](https://os.mbed.com/docs/mbed-os/v5.12/tools/developing-mbed-cli.html) to deploy and compile the binary:

First, deploy the dependencies:

```sh
$ mbed deploy
```

Then, compile the application:

```
$ mbed compile -m YOUR_TARGET -t GCC_ARM
```

>Note: when you do not specify a target, `mbed` will attempt to detect the target from the device that is connected to your computer.

## Running the application

Drag and drop the application binary from `BUILD/YOUR_TARGET/GCC_ARM/mbed-os.bin` to your Mbed enabled target hardware, which appears as a USB device on your host machine. 

Attach a serial console emulator of your choice (for example, PuTTY, Minicom or Serial) to your USB device. Set the baudrate to 115200 bit/s, and reset your board by pressing the reset button.

You should see an output similar to this:

```
Initialized Arm Mbed LoRaWAN stack
Joining network...
Joined network
Switched to class C
Sending 0 bytes
Message sent
```

When you send a `SYNC_CLOCK` message using the [controller](../controller) application, you should see an output similar to this:

```
Received message on port 2 (4 bytes): 9c 5e d5 5c 
Synchronized clock to Fri May 10 11:21:00 2019
```

## Optional: Enable tracing

To enable Mbed trace, set the following field in `mbed_app.json`:

```json
"target_overrides": {
  "*": {
    "mbed-trace.enable": true
  }
}
```

The trace is disabled by default to save RAM and reduce main stack usage.

>Please note that some targets with small RAM size (e.g. DISCO_L072CZ_LRWAN1 and MTB_MURATA_ABZ) mbed traces cannot be enabled without increasing the default `"main_stack_size": 1024`
