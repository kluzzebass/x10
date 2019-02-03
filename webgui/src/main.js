import Vue from 'vue'
import App from './App.vue'
import axios from 'axios'

axios.defaults.baseURL = (process.env.NODE_ENV === 'development' ? 'http://x10' : '') + '/api/';
axios.defaults.headers.post['Content-Type'] = 'application/json';

Vue.config.productionTip = false

new Vue({
  render: h => h(App),
}).$mount('#app')

