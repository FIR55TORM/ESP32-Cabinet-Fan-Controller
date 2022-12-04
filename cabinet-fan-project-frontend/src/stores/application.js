import { ref, computed } from "vue";
import { defineStore } from "pinia";

export const useApplicationStore = defineStore("application", () => {  
  const isDarkMode = computed(() => {
    return window.matchMedia && window.matchMedia('(prefers-color-scheme: dark)').matches;
  });

  return { isDarkMode };
});
