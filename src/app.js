import "./scss/app.scss";

import { createApp } from "vue";
import App from "./components/App.vue";

import axios from "axios";
window.axios = axios;

// Bootstrap
import BootstrapVue from "bootstrap-vue-next";
// Toast
import Vue3Toastify, { toast } from "vue3-toastify";
window.toast = toast;

createApp(App)
  .use(BootstrapVue)
  .use(Vue3Toastify, {
    position: toast.POSITION.TOP_CENTER,
    theme: toast.THEME.DARK,
  })
  .mount("#app");
