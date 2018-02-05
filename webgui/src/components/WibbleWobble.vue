<template>
  <div v-if="wibbleRange">
    <div class="row mb-4">
      <div class="col-2">
        Wibble X
      </div>
      <div class="col-10">
        <vue-slider v-model="wibbleX" :min="0" :max="wibbleRange" @callback="changeWibbleWobble" ></vue-slider>
      </div>
    </div>
    <div class="row mb-4">
      <div class="col-2">
        Wibble Y
      </div>
      <div class="col-10">
        <vue-slider v-model="wibbleY" :min="0" :max="wibbleRange" @callback="changeWibbleWobble" ></vue-slider>
      </div>
    </div>
    <div class="row mb-4">
      <div class="col-2">
        Wobble X
      </div>
      <div class="col-10">
        <vue-slider v-model="wobbleX" :min="0" :max="wobbleRange" @callback="changeWibbleWobble" ></vue-slider>
      </div>
    </div>
    <div class="row mb-4">
      <div class="col-2">
        Wobble Y
      </div>
      <div class="col-10">
        <vue-slider v-model="wobbleY" :min="0" :max="wobbleRange" @callback="changeWibbleWobble" ></vue-slider>
      </div>
    </div>
    <div class="row mb-4">
      <div class="col-2">
        Randomize rate
      </div>
      <div class="col-10">
        <vue-slider v-model="changeRate" :min="0" :max="300" :formatter="function (value) { return value ? `${value} seconds` : 'off' }" @callback="changeWibbleWobble" ></vue-slider>
      </div>
    </div>
  </div>
</template>

<script>
import http from 'axios'
import _ from 'lodash'
import vueSlider from 'vue-slider-component'

export default {
  name: 'WibbleWobble',
  data () {
    return {
      wibbleRange: null,
      wobbleRange: null,
      wibbleX: null,
      wibbleY: null,
      wobbleX: null,
      wobbleY: null,
      changeRate: null,
      interval: null,
      wibbleWobblePending: false
    }
  },
  props: [
    'baseUrl'
  ],
  components: {
    vueSlider
  },
  mounted () {
    this.getWibbleWobble()

    this.interval = setInterval(() => {
      this.getWibbleWobble()
    }, 2000)
  },
  beforeDestroy () {
    clearInterval(this.interval)
    this.interval = null
  },
  methods: {
    getWibbleWobble () {
      if (this.wibbleWobblePending) return
      this.wibbleWobblePending = true
      http.get(this.baseUrl + 'wibblewobble')
        .then((response) => {
          this.wibbleRange = response.data.wibbleRange
          this.wobbleRange = response.data.wobbleRange
          this.wibbleX = response.data.wibbleX
          this.wibbleY = response.data.wibbleY
          this.wobbleX = response.data.wobbleX
          this.wobbleY = response.data.wobbleY
          this.changeRate = response.data.changeRate
          this.wibbleWobblePending = false
        })
        .catch((error) => {
          console.log(error)
          this.wibbleWobblePending = false
        })
    },
    changeWibbleWobble: _.debounce(function (value) {
      http.post(this.baseUrl + 'wibblewobble', {
        wibbleX: this.wibbleX,
        wibbleY: this.wibbleY,
        wobbleX: this.wobbleX,
        wobbleY: this.wobbleY,
        changeRate: this.changeRate
      })
        .then((response) => {
          this.getWibbleWobble()
        })
        .catch((error) => {
          console.log(error)
        })
    }, 500)
  }
}
</script>

<style lang="sass">

</style>
