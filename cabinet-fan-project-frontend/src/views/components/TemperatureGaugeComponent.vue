<template>    
    <v-chart class="gauge" :option="temperatureGaugeOptions" :autoresize="true"  />
</template>

<script setup>
import { useHomeStore } from "../../stores/home";
import { reactive } from "vue";
import * as echarts from "echarts/core";
import { GaugeChart } from "echarts/charts";
import { CanvasRenderer } from "echarts/renderers";
import VChart from "vue-echarts";
import { storeToRefs } from "pinia";

echarts.use([
    GaugeChart,    
    CanvasRenderer    
]);

const store = useHomeStore();
const { temperatureRounded } = storeToRefs(store);
const temperatureGaugeOptions = reactive({
    backgroundColor: "transparent",
    series: [
        {
            type: "gauge",
            center: ["50%", "60%"],
            startAngle: 200,
            endAngle: -20,
            min: 0,
            max: 40,
            splitNumber: 10,
            itemStyle: {
                color: "#FFAB91",
            },
            progress: {
                show: true,
                width: 30,
            },
            pointer: {
                show: false,
            },
            axisLine: {
                lineStyle: {
                    width: 30,
                },
            },
            axisTick: {
                distance: -45,
                splitNumber: 5,
                lineStyle: {
                    width: 2,
                    color: "#999",
                },
            },
            splitLine: {
                distance: -52,
                length: 14,
                lineStyle: {
                    width: 3,
                    color: "#999",
                },
            },
            axisLabel: {
                distance: 0,
                color: "#999",
                fontSize: 12,
            },
            anchor: {
                show: false,
            },
            title: {
                show: true,
            },
            detail: {
                valueAnimation: true,
                width: "60%",
                lineHeight: 40,
                borderRadius: 8,
                offsetCenter: [0, "-15%"],
                fontSize: 30,
                fontWeight: "bolder",
                formatter: "{value} °C",
                color: "auto",
            },
            data: [
                {
                    value: temperatureRounded,
                    name: "Current Temp",
                },
            ],
        },
        {
            type: "gauge",
            center: ["50%", "60%"],
            startAngle: 200,
            endAngle: -20,
            min: 0,
            max: 60,
            itemStyle: {
                color: "#FD7347",
            },
            progress: {
                show: false,
                width: 8,
            },
            pointer: {
                show: false,
            },
            axisLine: {
                show: false,
            },
            axisTick: {
                show: false,
            },
            splitLine: {
                show: false,
            },
            axisLabel: {
                show: false,
            },
            detail: {
                show: false,
            },
            data: [
                {
                    value: 20,
                },
            ],
        },
    ],

});

</script>
