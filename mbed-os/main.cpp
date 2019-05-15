// Copyright © 2019 The Things Network Foundation, The Things Industries B.V.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>

#include "events/EventQueue.h"
#include "lora_radio_helper.h"
#include "lorawan/LoRaWANInterface.h"
#include "lorawan/system/lorawan_data_structures.h"
#include "mbed.h"
#include "trace_helper.h"

using namespace events;

uint8_t tx_buffer[30];
uint8_t rx_buffer[30];

#define F_PORT_NOOP 1
#define F_PORT_SYNC_CLOCK 2
#define MAX_NUMBER_OF_EVENTS 10
#define CONFIRMED_MSG_RETRY_COUNTER 3

static void lora_event_handler(lorawan_event_t event);

static EventQueue ev_queue(MAX_NUMBER_OF_EVENTS *EVENTS_EVENT_SIZE);
static LoRaWANInterface lorawan(radio);
static lorawan_app_callbacks_t callbacks;

int main(void) {
  setup_trace();
  lorawan_status_t retcode;

  // Initialize LoRaWAN stack
  retcode = lorawan.initialize(&ev_queue);
  if (retcode != LORAWAN_STATUS_OK) {
    printf("Failed to initialize Arm Mbed LoRaWAN stack; code = %d\r\n",
           retcode);
    return -1;
  }
  printf("Initialized Arm Mbed LoRaWAN stack\r\n");

  // Register application callbacks
  callbacks.events = mbed::callback(lora_event_handler);
  lorawan.add_app_callbacks(&callbacks);

  // Set number of retries in case of CONFIRMED messages
  retcode = lorawan.set_confirmed_msg_retries(CONFIRMED_MSG_RETRY_COUNTER);
  if (retcode != LORAWAN_STATUS_OK) {
    printf("Failed to set confirmed message retries; code = %d\r\n", retcode);
    return -1;
  }

  // Enable adaptive data rate
  retcode = lorawan.enable_adaptive_datarate();
  if (retcode != LORAWAN_STATUS_OK) {
    printf("Failed to enable adaptive data rate; code = %d\r\n", retcode);
    return -1;
  }

  // Start OTAA procedure.
  retcode = lorawan.connect();
  if (retcode == LORAWAN_STATUS_OK ||
      retcode == LORAWAN_STATUS_CONNECT_IN_PROGRESS) {
    printf("Joining network...\r\n");
  } else {
    printf("Failed to join; code = %d\r\n", retcode);
    return -1;
  }

  ev_queue.dispatch_forever();
  return 0;
}

static void send_message(uint8_t port, uint16_t packet_len) {
  int16_t retcode =
      lorawan.send(port, tx_buffer, packet_len, MSG_UNCONFIRMED_FLAG);
  if (retcode < 0) {
    retcode == LORAWAN_STATUS_WOULD_BLOCK
        ? printf("Failed to send; duty-cycle enforced\r\n")
        : printf("Failed to send; code = %d\r\n", retcode);
    return;
  }

  printf("Sending %d bytes\r\n", retcode);
}

static void receive_message() {
  uint8_t port;
  int flags;
  int16_t retcode = lorawan.receive(rx_buffer, sizeof(rx_buffer), port, flags);
  if (retcode < 0) {
    printf("Failed to receive; code = %d\r\n", retcode);
    return;
  }

  printf("Received message on port %u (%d bytes, flags = %d): ", port, retcode, flags);
  for (uint8_t i = 0; i < retcode; i++) {
    printf("%02x ", rx_buffer[i]);
  }
  printf("\r\n");

  switch (port) {
  case F_PORT_SYNC_CLOCK:
    if (retcode != 4) {
      printf("Unexpected length of SYNC_CLOCK message (%d)\r\n", retcode);
      break;
    }
    time_t *t = (time_t *)rx_buffer;
    // TODO: Communicate DST out-of-band and apply timezone offset.
    set_time(*t);
    printf("Synchronized clock to %s", ctime(t));
  }
}

static void lora_event_handler(lorawan_event_t event) {
  switch (event) {
  case CONNECTED: {
    printf("Joined network\r\n");
    lorawan_status_t retcode = lorawan.set_device_class(CLASS_C);
    if (retcode != LORAWAN_STATUS_OK) {
      printf("Failed to switch to class C, code = %d\n", retcode);
      return;
    }
    printf("Switched to class C\r\n");
    // Send an empty message to confirm the session.
    send_message(1, 0);
    break;
  }
  case DISCONNECTED:
    ev_queue.break_dispatch();
    printf("Reset session\r\n");
    break;
  case TX_DONE:
    printf("Message sent\r\n");
    break;
  case TX_TIMEOUT:
  case TX_ERROR:
  case TX_CRYPTO_ERROR:
  case TX_SCHEDULING_ERROR:
    printf("Failed to transmit; code = %d\r\n", event);
    break;
  case RX_DONE:
    receive_message();
    break;
  case RX_TIMEOUT:
  case RX_ERROR:
    printf("Failed to receive; code = %d\r\n", event);
    break;
  case JOIN_FAILURE:
    printf("Failed to join; check credentials\r\n");
    break;
  case UPLINK_REQUIRED:
    printf("Network Server requests uplink\r\n");
    send_message(1, 0);
    break;
  default:
    MBED_ASSERT("Unknown event");
  }
}
