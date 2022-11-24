import { createApp } from "vue";
import { createPinia } from "pinia";
import { FontAwesomeIcon } from "@fortawesome/vue-fontawesome";

import App from "../App.vue";
import router from "../router";

import "/scss/main.scss";
import "bootstrap-dark-5/dist/css/bootstrap-dark-plugin.css";

import * as bootstrap from "bootstrap";

const app = createApp(App);

app.component('font-awesome-icon', FontAwesomeIcon);

app.use(createPinia());
app.use(router);

app.mount('#app');
