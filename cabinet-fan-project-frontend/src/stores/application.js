import { ref, computed } from "vue";
import { defineStore } from "pinia";
import axios from "axios";

import { API_ENDPOINTS, EVENT_SOURCE_ENDPOINT } from "./api-endpoints"

export const useApplicationStore = defineStore("application", () => {  
  const isSoftAPMode = ref(false);

  const isDarkMode = computed(() => {
    return window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches;
  });

  const getIsSoftAPMode = async() => {
    return await axios.get(API_ENDPOINTS.IS_SOFT_AP_MODE)
    .then((response) => {      
      isSoftAPMode.value = response.data.isSoftAPMode;
    });
  }

  return { isDarkMode, isSoftAPMode, getIsSoftAPMode };
});
