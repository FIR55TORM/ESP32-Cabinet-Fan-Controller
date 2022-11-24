<template>
  <div class="container-fluid">
    <div class="row">
      <div class="col-12">
        <form action="/WifiDetails" method="POST">
          <div class="row">
            <div class="col-6">
              <div class="mb-3">
                <label for="txtSSID" class="form-label">Wifi Name (SSID)</label>
                <input type="text" class="form-control" id="txtSSID" aria-describedby="ssidHelp"
                  v-model="ssid" />
                <div id="ssidHelp" class="form-text">
                  The name of your WiFi network (SSID)
                </div>
              </div>

              <div class="mb-3">
                <label for="txtWiFiPassword" class="form-label">Wifi Password</label>
                <input type="password" class="form-control" id="txtWiFiPassword" aria-describedby="passHelp"
                  v-model="passkey" />
                <div id="passHelp" class="form-text">
                  Password for your named WiFi network
                </div>
              </div>

              <div class="mb-3">
                <label for="txtIp" class="form-label">IP of this device</label>
                <input type="text" class="form-control" id="txtIp" aria-describedby="ipHelp"
                  v-model="deviceIp" />
                <div id="ipHelp" class="form-text">
                  IP address for this device (IPv4)
                </div>
              </div>

              <div class="mb-3">
                <label for="txtGateway" class="form-label">Gateway IP Address</label>
                <input type="text" class="form-control" id="txtGateway" aria-describedby="gatewayHelp"
                  v-model="gatewayIp" />
                <div id="gatewayHelp" class="form-text">
                  Gateway IP address for this device (IPv4)
                </div>
              </div>

              <div class="form-check form-switch form-control-lg">
                <input class="form-check-input" type="checkbox" role="switch" id="chkMQTT" v-model="isUsingMQTT" />
                <label class="form-check-label" for="chkMQTT">Use MQTT</label>
              </div>
            </div>

            <div class="col-6" v-if="isUsingMQTT">
              <div id="mqtt-setup">
                <div class="mb-3">
                  <label for="txtMqttClientName" class="form-label">Client Name For Device</label>
                  <input type="text" class="form-control" id="txtMqttClientName" aria-describedby="mtqqClientNameHelp"
                    v-model="clientName" />
                  <div id="mtqqClientNameHelp" class="form-text">
                    Client name for MQTT broker
                  </div>
                </div>

                <div class="mb-3">
                  <label for="txtMqttIp" class="form-label">IP of MQTT broker</label>
                  <input type="text" class="form-control" id="txtMqttIp" aria-describedby="mtqqIPHelp"
                    v-model="brokerIp" />
                  <div id="mtqqIPHelp" class="form-text">
                    IP address for MQTT broker (IPv4)
                  </div>
                </div>

                <div class="mb-3">
                  <label for="txtMqttPort" class="form-label">Port number of MQTT port number</label>
                  <input type="text" class="form-control" id="txtMqttPort" aria-describedby="mtqqPortHelp"
                    v-model="brokerPort" />
                  <div id="mtqqPortHelp" class="form-text">
                    Port number of MQTT broker
                  </div>
                </div>

                <div class="mb-3">
                  <label for="txtMqttUsername" class="form-label">Username</label>
                  <input type="text" class="form-control" id="txtMqttUsername" aria-describedby="mtqqUsernameHelp" 
                  v-model="brokerUsername"/>
                  <div id="mtqqUsernameHelp" class="form-text">
                    Username for MQTT broker
                  </div>
                </div>

                <div class="mb-3">
                  <label for="txtMqttPassword" class="form-label">Password</label>
                  <input type="password" class="form-control" id="txtMqttPassword"
                    aria-describedby="mtqqPasswordHelp" 
                    v-model="brokerPassword"/>
                  <div id="mtqqPasswordHelp" class="form-text">
                    Password for MQTT broker
                  </div>
                </div>
              </div>
            </div>
          </div>
          <hr />
          <button type="submit" class="btn btn-primary">Submit</button>
        </form>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, reactive } from "vue";

const isUsingMQTT = ref(false);

//Wifi
const ssid = ref("");
const passkey = ref("");
const deviceIp = ref("192.168.1.50");
const gatewayIp = ref("192.168.1.22");

//MQTT details
const clientName = ref("Cabinet Fan Controller");
const brokerIp = ref("192.168.1.110");
const brokerPort = ref("1883");
const brokerUsername = ref("");
const brokerPassword = ref("");

</script>
