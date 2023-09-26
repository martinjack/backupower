import { fileURLToPath, URL } from "node:url";

import { defineConfig } from "vite";
import vue from "@vitejs/plugin-vue";
import Components from "unplugin-vue-components/vite";
import { BootstrapVueNextResolver } from "unplugin-vue-components/resolvers";
import appConfig from "./package.json";

export default defineConfig({
  plugins: [
    vue(),
    Components({
      resolvers: [BootstrapVueNextResolver()],
    }),
  ],
  resolve: {
    alias: {
      "@": fileURLToPath(new URL("./src", import.meta.url)),
      icons: "vue-material-design-icons",
    },
  },
  server: {
    port: 8000,
  },
  preview: {
    port: 8000,
  },
  build: {
    outDir: "./data",
    rollupOptions: {
      output: {
        manualChunks: {},
        entryFileNames: `[name]-[hash].js`,
        chunkFileNames: `[name]-[hash].js`,
        assetFileNames: `[name]-[hash].[ext]`,
      },
    },
  },
  define: {
    "import.meta.env.VERSION": JSON.stringify(appConfig.version),
  },
});
