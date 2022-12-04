<template>
  <main>
    <div class="container-fluid">
      <div class="row" id="loader" v-if="store.isLoading">
        <div class="col-12 text-center">
          <i class="fa-solid fa-fan fa-spin"></i> Loading...
        </div>
      </div>

      <div class="row" id="error" v-if="store.errors.hasErrors">
        An error occurred, please try again.
      </div>

      <div class="row" id="main-area" v-show="!store.isLoading">
        <div class="col-3" id="tempGauge">
          <TemperatureGauge />
        </div>
        <div class="col-3">
          <HumidityGauge />
        </div>
        <div class="col-3"></div>
        <div class="col-3"></div>

        <div class="col-6">
          <div class="card">
            <div class="card-body">
              <h5 class="card-title">Automatic Fan Speed</h5>
              <p class="card-text">
                When on, adjusts fan speed according to target temperature.
              </p>
              <div class="form-check form-switch mb-3" id="auto-speed-area">
                <input class="form-check-input" type="checkbox" role="switch" id="chkAutomaticMode"
                  v-model="store.isFanAutoMode" @change="onChangeAutoMode" />
                <label class="form-check-label" for="chkAutomaticMode">Automatic Speed</label>
              </div>

              <div class="mb-3" id="target-temp-area">
                <div class="input-group">
                  <span class="input-group-text">Target Temperature</span>
                  <input type="text" class="form-control" id="txtTargetTemp" v-model="store.targetTemperature"
                    :disabled="!store.isFanAutoMode" :class="{ disabled: !store.isFanAutoMode }" @change="onChangeTargetTemperature"/>
                  <span class="input-group-text">°C</span>
                </div>
                <div class="form-text">Between 10 - 30°C only</div>
              </div>
            </div>
          </div>
        </div>

        <div class="col-6">
          <div class="card" id="fan-speed-area" v-if="!store.isFanAutoMode">
            <div class="card-body">
              <h5 class="card-title">Manual Fan Speed</h5>
              <p class="card-text">
                When automatic speed is off, you set the fan speed.
              </p>
              <div>
                <label for="rngFanSpeed" class="form-label">Fan Speed: <span
                    id="speed-readout"></span><span>%</span></label>
                <input type="range" class="form-range" min="0" max="100" id="rngFanSpeed" v-model="store.fanSetSpeedPercentage" @change="onChangeManualFanSpeed"/>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </main>
</template>

<script setup>
import { onMounted } from "vue";
import { useHomeStore } from "../stores/home";
import TemperatureGauge from './components/TemperatureGaugeComponent.vue';
import HumidityGauge from "./components/HumidityGaugeComponent.vue";

const store = useHomeStore();

const onChangeManualFanSpeed = () => {
  store.setFanSpeed();
};

const onChangeAutoMode = () => {
  store.setAutoMode();
}

const onChangeTargetTemperature = () => {
  store.setTargetTemperature();
}

onMounted(() => {
  store.getConfig();
  store.initServerEvents();
});
</script>