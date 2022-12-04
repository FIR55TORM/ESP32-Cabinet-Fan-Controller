import { fileURLToPath, URL } from 'node:url'

import { resolve } from "path"
import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import viteCompression from 'vite-plugin-compression';

//Change this to your selected IP address to point to the MCU
const DEV_PROXY_URL = "http://192.168.1.50/";

// https://vitejs.dev/config/
export default defineConfig({
  root: resolve(__dirname, 'src'),
  base: "",
  plugins: [vue(), viteCompression({ algorithm: 'gzip', deleteOriginFile: true })],
  build: {
    outDir: "../../data/",
    rollupOptions: {
      output: {
        entryFileNames: `assets/[name].js`,
        chunkFileNames: `assets/[name].js`,
        assetFileNames: `assets/[name].[ext]`
      }
    }
  },
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url)),
      '~bootstrap': resolve(__dirname, 'node_modules/bootstrap'),
    }
  },
  server: {
    proxy: {
      '/api': DEV_PROXY_URL,
      '/events': DEV_PROXY_URL
    }
  }
})
