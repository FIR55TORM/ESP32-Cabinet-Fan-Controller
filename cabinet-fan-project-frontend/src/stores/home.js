import { useApplicationStore } from "./application";
import { ref, computed, reactive } from "vue";
import { defineStore } from "pinia";
import axios from "axios";

const API_ENDPOINTS = {
    SET_FAN_SPEED: "/api/set-fan-speed",
    SET_IS_AUTOMATIC_MODE: "/api/set-automatic-mode",
    SET_TARGET_TEMPERATURE: "/api/set-target-temperature",
    EVENTS: {
        CURRENT_TEMPERATURE: "/events"
    },
    GET_CONFIG: "/api/get-config"
}

export const useHomeStore = defineStore("home", () => {
    const application = useApplicationStore();
    const temperatureRounded = ref("0.0");
    const humidityRounded = ref("0.0");
    const isDarkMode = application.isDarkMode;
    const fanReading = reactive({ fanTacho: 0, currentFanSpeedPercentage: 0 });
    const fanSetSpeedPercentage = ref(0);
    const isFanAutoMode = ref(false);
    const targetTemperature = ref(25);
    const isLoading = ref(true);
    const errors = reactive({ hasError: false, errors: [] })

    // Read more about EventSource: 
    // https://developer.mozilla.org/en-US/docs/Web/API/EventSource
    const initServerEvents = () => {
        var source = new EventSource(API_ENDPOINTS.EVENTS.CURRENT_TEMPERATURE);

        source.addEventListener('open', function (e) {
            console.log("Events Connected");
        }, false);

        source.addEventListener('error', function (e) {
            if (e.target.readyState != EventSource.OPEN) {
                console.log("Events Disconnected");
            }
        }, false);

        source.addEventListener('message', function (e) {
            console.log("message", e.data);
        }, false);

        source.addEventListener('onTemperatureHumidityReading', function (e) {
            console.log("onTemperatureHumidityReading", e.data);
            var tempHumDto = JSON.parse(e.data);

            temperatureRounded.value = parseFloat(tempHumDto.temperature).toFixed(2);
            humidityRounded.value = parseFloat(tempHumDto.humidity).toFixed(2);
        }, false);

        source.addEventListener("onFanTachoReading", (e) => {
            console.log("onFanTachoReading", e.data);
            fanReading.value = e.data;
        });
    }

    const getConfig = async () => {
        await axios.get(API_ENDPOINTS.GET_CONFIG)
            .then((response) => {
                fanSetSpeedPercentage.value = parseInt(response.data.fanSpeedPercentage);
                isFanAutoMode.value = response.data.isAutomaticMode;
                targetTemperature.value = response.data.targetTemperature
                isLoading.value = false;
            }).catch(() => {
                errors.value = { hasError: true, errors: ["Getting config data failed"] }
            });
    }

    const setFanSpeed = async () => {
        await axios.post(API_ENDPOINTS.SET_FAN_SPEED,
            {
                speed: parseInt(fanSetSpeedPercentage.value)
            }
        );
    }

    const setAutoMode = async () => {
        await axios.post(API_ENDPOINTS.SET_IS_AUTOMATIC_MODE, {
            isAutomaticMode: isFanAutoMode.value
        })
    }
    const setTargetTemperature = async () => {
        await axios.post(API_ENDPOINTS.SET_TARGET_TEMPERATURE, {
            targetTemperature: targetTemperature.value
        })
    }

    return {
        temperatureRounded,
        humidityRounded,
        isDarkMode,
        fanReading,
        fanSetSpeedPercentage,
        isFanAutoMode,
        targetTemperature,
        isLoading,
        errors,
        initServerEvents,
        getConfig,
        setFanSpeed,
        setAutoMode,
        setTargetTemperature
    };
});