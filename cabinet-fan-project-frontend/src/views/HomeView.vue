<script setup></script>

<template>
  <main>
    <div class="container-fluid">
      <div class="row" id="loader">
        <div class="col-12 text-center">
          <i class="fa-solid fa-fan fa-spin"></i> Loading...
        </div>
      </div>

      <div class="row d-none" id="error">
        An error occurred, please try again.
      </div>

      <div class="row d-none" id="main-area">
        <div class="col-3">
          <div id="tempGauge" style="width: 310px; height: 310px"></div>
        </div>
        <div class="col-3">
          <div id="humidityGauge" style="width: 310px; height: 310px"></div>
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
                <input
                  class="form-check-input"
                  type="checkbox"
                  role="switch"
                  id="chkAutomaticMode"
                />
                <label class="form-check-label" for="chkAutomaticMode"
                  >Automatic Speed</label
                >
              </div>

              <div class="mb-3" id="target-temp-area">
                <div class="input-group">
                  <span class="input-group-text">Target Temperature</span>
                  <input type="text" class="form-control" id="txtTargetTemp" />
                  <span class="input-group-text">°C</span>
                </div>
                <div class="form-text">Between 10 - 30°C only</div>
              </div>
            </div>
          </div>
        </div>

        <div class="col-6">
          <div class="card" id="fan-speed-area">
            <div class="card-body">
              <h5 class="card-title">Manual Fan Speed</h5>
              <p class="card-text">
                When automatic speed is off, you set the fan speed.
              </p>
              <div>
                <label for="rngFanSpeed" class="form-label"
                  >Fan Speed: <span id="speed-readout"></span
                  ><span>%</span></label
                >
                <input
                  type="range"
                  class="form-range"
                  min="0"
                  max="100"
                  id="rngFanSpeed"
                />
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </main>
</template>
