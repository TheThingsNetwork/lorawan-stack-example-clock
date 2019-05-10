# Example clock controller

This is an example controller application for synchronizing clocks of end devices using LoRaWAN.

## Configuration

The controller application requires the following environment variables set:

| Key | Description | Example |
| --- | --- | --- |
| `TTN_SERVER` | HTTP(s) address of the Application Server | `http://localhost:1885` |
| `TTN_APPLICATION_ID` | Application ID | `app1` |
| `TTN_API_KEY` | Application API Key | `NNSXS.RAXV54...` |
| `TTN_DEVICE_ID` | Device ID | `dev1` |

## Compiling the application

First, install the dependencies:

```sh
$ yarn
```

Then, build the application:

```sh
$ npm run build
```

## Running the application

Run the application:

```sh
$ npm run start
```

You should see an output similar to this:

```
> lorawan-stack-example-clock@1.0.0 start /dev/src/lorawan-stack-example-clock/controller
> node dist/index.js

sync clock dev1: now = 2019-05-10T12:07:19.234Z; target = 2019-05-10T12:07:29.000Z; unix time = 1557490049
```

The controller schedules a downlink message 10 seconds from the current time.
