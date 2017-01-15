#include "sub_levitation.h"

static char* Levitation::State_str[] = {
  "STOPPED",
  "RUNNING"
};

Levitation::Levitation(int levitation_pin) : levitation_pin(levitation_pin) {
  target_state = STOPPED;
  current_state = STOPPED;
}

Levitation::State Levitation::get_state() {
  return current_state;
}

void Levitation::set_state(State s) {
  target_state = s;
}

// Main processing logic
void Levitation::update() {
  switch (current_state) {
    case STOPPED:
      if (target_state == RUNNING) {
        digitalWrite(levitation_pin, RELAY_ON);
        current_state = RUNNING;
        break;
      }
      break;

    case RUNNING:
      if (target_state == STOPPED) {
        digitalWrite(levitation_pin, RELAY_OFF);
        current_state = STOPPED;
        break;
      }
      break;
  }
}

void Levitation::process_msg(MQTT mqtt, char* topic, JsonObject& root) {
  if (strncmp(topic, "subsystem/levitation", 10 + 3) != 0)
    return;

  mqtt.debug("LEVITATION");
}

void Levitation::send_heartbeat(MQTT mqtt) {
  JsonObject& root = mqtt.jsonBuffer.createObject();

  root["state"] = State_str[current_state];
  root["t_state"] = State_str[target_state];

  String string_status;
  root.printTo(string_status);
  mqtt.client.publish("subsystem/levitation", string_status.c_str());
}