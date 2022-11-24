var baseURL = "//192.168.1.50"

const API_ENDPOINTS = {
    SET_FAN_SPEED: "/api/set-fan-speed",
    SET_IS_AUTOMATIC_MODE: "/api/set-automatic-mode",
    SET_TARGET_TEMPERATURE: "/api/set-target-temperature",
    EVENTS: {
        CURRENT_TEMPERATURE: "/events"
    }
}

const temperatureGaugeOptions = {
    backgroundColor: 'transparent',
    series: [
        {
            type: 'gauge',
            center: ['50%', '60%'],
            startAngle: 200,
            endAngle: -20,
            min: 0,
            max: 40,
            splitNumber: 10,
            itemStyle: {
                color: '#FFAB91'
            },
            progress: {
                show: true,
                width: 30
            },
            pointer: {
                show: false
            },
            axisLine: {
                lineStyle: {
                    width: 30
                }
            },
            axisTick: {
                distance: -45,
                splitNumber: 5,
                lineStyle: {
                    width: 2,
                    color: '#999'
                }
            },
            splitLine: {
                distance: -52,
                length: 14,
                lineStyle: {
                    width: 3,
                    color: '#999'
                }
            },
            axisLabel: {
                distance: 0,
                color: '#999',
                fontSize: 12
            },
            anchor: {
                show: false
            },
            title: {
                show: true
            },
            detail: {
                valueAnimation: true,
                width: '60%',
                lineHeight: 40,
                borderRadius: 8,
                offsetCenter: [0, '-15%'],
                fontSize: 30,
                fontWeight: 'bolder',
                formatter: '{value} °C',
                color: 'auto'
            },
            data: [
                {
                    value: 20,
                    name: "Current Temp"
                }
            ]
        },
        {
            type: 'gauge',
            center: ['50%', '60%'],
            startAngle: 200,
            endAngle: -20,
            min: 0,
            max: 60,
            itemStyle: {
                color: '#FD7347'
            },
            progress: {
                show: false,
                width: 8
            },
            pointer: {
                show: false
            },
            axisLine: {
                show: false
            },
            axisTick: {
                show: false
            },
            splitLine: {
                show: false
            },
            axisLabel: {
                show: false
            },
            detail: {
                show: false
            },
            data: [
                {
                    value: 20
                }
            ]
        }
    ]
};

const humidityGaugeOptions = {
    backgroundColor: 'transparent',
    series: [
        {
            type: 'gauge',
            center: ['50%', '60%'],
            startAngle: 200,
            endAngle: -20,
            min: 0,
            max: 100,
            splitNumber: 10,
            itemStyle: {
                color: '#FFAB91'
            },
            progress: {
                show: true,
                width: 30
            },
            pointer: {
                show: false
            },
            axisLine: {
                lineStyle: {
                    width: 30
                }
            },
            axisTick: {
                distance: -45,
                splitNumber: 5,
                lineStyle: {
                    width: 2,
                    color: '#999'
                }
            },
            splitLine: {
                distance: -52,
                length: 14,
                lineStyle: {
                    width: 3,
                    color: '#999'
                }
            },
            axisLabel: {
                distance: 0,
                color: '#999',
                fontSize: 12
            },
            anchor: {
                show: false
            },
            title: {
                show: true
            },
            detail: {
                valueAnimation: true,
                width: '60%',
                lineHeight: 40,
                borderRadius: 8,
                offsetCenter: [0, '-15%'],
                fontSize: 30,
                fontWeight: 'bolder',
                formatter: '{value}%',
                color: 'auto'
            },
            data: [
                {
                    value: 20,
                    name: "Humidity"
                }
            ]
        },
        {
            type: 'gauge',
            center: ['50%', '60%'],
            startAngle: 200,
            endAngle: -20,
            min: 0,
            max: 60,
            itemStyle: {
                color: '#FD7347'
            },
            progress: {
                show: false,
                width: 8
            },
            pointer: {
                show: false
            },
            axisLine: {
                show: false
            },
            axisTick: {
                show: false
            },
            splitLine: {
                show: false
            },
            axisLabel: {
                show: false
            },
            detail: {
                show: false
            },
            data: [
                {
                    value: 20
                }
            ]
        }
    ]
};

var temperatureGauge;
var humidityGauge;

var temperatureHumidityGaugeInit = () => {
    var temperatureGaugeElement = document.getElementById('tempGauge')
    var humidityGaugeElement = document.getElementById("humidityGauge");
    if (isDarkMode()) {
        temperatureGauge = echarts.init(temperatureGaugeElement, 'dark');
        humidityGauge = echarts.init(humidityGaugeElement, 'dark');
    }
    else {
        temperatureGauge = echarts.init(temperatureGaugeElement);
        humidityGauge = echarts.init(humidityGaugeElement);
    }

    temperatureGauge.setOption(temperatureGaugeOptions);
    humidityGauge.setOption(humidityGaugeOptions);
}

var initEventSources = () => {
    if (!!window.EventSource) {
        var source = new EventSource(baseURL + API_ENDPOINTS.EVENTS.CURRENT_TEMPERATURE);

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

            let tempRounded = parseFloat(tempHumDto.temperature).toFixed(2);
            let humiRouded = parseFloat(tempHumDto.humidity).toFixed(2);

            temperatureGauge.setOption({
                series: [
                    {
                        data: [
                            {
                                value: tempRounded,
                                name: "Current Temp"
                            }
                        ]
                    },
                    {
                        data: [
                            {
                                value: tempRounded,
                                name: "Current Temp"
                            }
                        ]
                    }
                ]
            });

            humidityGauge.setOption({
                series: [
                    {
                        data: [
                            {
                                value: humiRouded,
                                name: "Humidity"
                            }
                        ]
                    },
                    {
                        data: [
                            {
                                value: humiRouded,
                                name: "Humidity"
                            }
                        ]
                    }
                ]
            });
        }, false);

        source.addEventListener("onFanTachoReading", (e) => {
            console.log("onFanTachoReading", e.data);
        });
    }
}

var isDarkMode = () => {
    return window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches;
}

$(function () {
    var isDebug = false;
    if (!isDebug) {
        baseURL = "";
    }

    temperatureHumidityGaugeInit();
    getConfigData();
    initEventSources();

    $(window).on("resize", () => {
        temperatureGauge.resize();
        humidityGauge.resize();
    });

    $("#rngFanSpeed").on("change", (e) => {
        var setValue = $("#rngFanSpeed").val();
        $.post(baseURL + API_ENDPOINTS.SET_FAN_SPEED, { speed: setValue });
    });

    $("#rngFanSpeed").on("input", (e) => {
        var setValue = $("#rngFanSpeed").val();
        setFanSpeedReadout(setValue);
    });

    $("#chkAutomaticMode").on("change", (e) => {
        var isAuto = $("#chkAutomaticMode").is(":checked");
        $.post(baseURL + API_ENDPOINTS.SET_IS_AUTOMATIC_MODE, { isAutomaticMode: isAuto ? 1 : 0 });
        toggleSpeedAndTempUi(isAuto);
    });

    $("#txtTargetTemp").on("keyup", _.debounce((e) => {
        e.target.value = e.target.value.replace(/[^0-9]/g, '');
        if (e.target.value > 30) {
            e.target.value = 30;
        }
        else if (e.target.value < 10) {
            e.target.value = 10;
        }
        $.post(baseURL + API_ENDPOINTS.SET_TARGET_TEMPERATURE, { targetTemperature: e.target.value });
    }, 250));
})

function getConfigData() {
    $.getJSON(baseURL + "/api/get-config", (data) => {
        $("#rngFanSpeed").val(data.fanSpeedPercentage);
        $("#chkAutomaticMode").prop("checked", data.isAutomaticMode);
        $("#txtTargetTemp").val(data.targetTemperature);

        setFanSpeedReadout(data.fanSpeedPercentage);
        toggleSpeedAndTempUi(data.isAutomaticMode);

        $("#loader").addClass("d-none");
        $("#main-area").removeClass("d-none");

    }).fail((data) => {
        $("#loader").addClass("d-none");
        $("#main-area").addClass("d-none");
        $("#error").removeClass("d-none");
    })
}

function setTargetTemperatureInput(value) {
    $("#txtTargetTemp").val(value);
}

function setFanSpeedReadout(value) {
    $("#speed-readout").html(value);
}

function toggleSpeedAndTempUi(value) {
    //When we toggle automatic switch...
    if (value === true) {
        $("#rngFanSpeed").prop("disabled", true);
        $("#txtTargetTemp").prop("disabled", false);
        $("#txtTargetTemp").removeClass("disabled");
        $("#fan-speed-area").fadeOut();
    } else {
        $("#rngFanSpeed").prop("disabled", false);
        $("#txtTargetTemp").prop("disabled", true);
        $("#txtTargetTemp").addClass("disabled");
        $("#fan-speed-area").fadeIn();
    }
}