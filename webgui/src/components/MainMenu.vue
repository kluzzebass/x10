<template>
  <div>
    <h1 class="headline ml-2 text-center">
      X10 Configurator
    </h1>
    <div v-if="display" class="card m-2">
      <div class="card-header">
        Display
      </div>
      <div class="card-body">
        <div class="row">
          <div class="col-3 mb-2">
            Power
          </div>
          <div class="col-9 mb-2">
            <button type="button" class="btn" :class="{ 'btn-success': display.power, 'btn-secondary': !display.power }" data-toggle="button" :aria-pressed="display.power" autocomplete="off" @click="togglePower">
              {{ display.power ? 'On' : 'Off' }}
            </button>
          </div>
        </div>
        <div class="row">
          <div class="col-3 mb-2">
            Brightness
          </div>
          <div class="col-9 mb-2">
            <vue-slider v-model="display.brightness" :min="display.minBrightness" :max="display.maxBrightness" @callback="changeBrightness" ></vue-slider>
          </div>
        </div>
      </div>
    </div>
    <div v-if="current != null" class="card m-2">
      <div class="card-header">
        Effects
      </div>
      <div class="card-body">
        <div class="accordion">

          <div class="card" v-for="(effect, index) in effects" :key="index" >
            <div class="card-header" :id="'heading' + effect.id">
              <button :class="{ 'btn': true, 'btn-link': true, 'collapsed': effect.id !== current,  }" data-toggle="collapse" :data-target="'#collapse' + effect.id" aria-expanded="true" :aria-controls="'collapse' + effect.id"  @click="changeEffect(effects[index].id)">
                {{ effect.name }}
              </button>
            </div>
            <div :id="'collapse' + effect.id" :class="{ 'collapse': true, 'show': effect.id === current }" :aria-labelledby="'heading' + effect.id" data-parent="#accordion">
              <div class="card-body">
                <component v-if="current === effect.id" v-bind:is="effect.name.replace(' ', '')"/>
              </div>
            </div>
          </div>

        </div>
      </div>
    </div>
  </div>
</template>

<script>

import http from 'axios'
import _ from 'lodash'
import vueSlider from 'vue-slider-component'
import Animator from './Animator'
import Clock from './Clock'
import ColorCycle from './ColorCycle'
import WibbleWobble from './WibbleWobble'

export default {
  name: 'MainMenu',
  data () {
    return {
      display: null,
      current: null,
      effects: null,
      interval: null,
      displayPending: false,
      effectsPending: false
    }
  },
  components: {
    vueSlider,
    Animator,
    Clock,
    ColorCycle,
    WibbleWobble
  },
  mounted () {
    this.getDisplay()
    this.getEffects()

    this.interval = setInterval(() => {
      this.getDisplay()
      this.getEffects()
    }, 2000)
  },
  beforeDestroy () {
    clearInterval(this.interval)
    this.interval = null
  },
  methods: {
    getDisplay () {
      if (this.displayPending) return
      this.displayPending = true
      http.get('display')
        .then(response => {
          this.display = response.data
          this.displayPending = false
        })
        .catch(error => {
          console.log(error)
          this.displayPending = false
        })
    },
    togglePower () {
      let power = !this.display.power
      http.post('display', {
        power: power
      })
        .then((response) => {
          this.display.power = power
        })
        .catch((error) => {
          console.log(error)
        })
    },
    changeBrightness: _.debounce(function (value) {
      http.post('display', {
        brightness: value,
        power: true
      })
        .then(response => {
          this.display.brightness = value
          this.display.power = true
        })
        .catch(error => {
          console.log(error)
        })
    }, 100),
    getEffects () {
      if (this.effectsPending) return
      this.effectsPending = true
      http.get('effect')
        .then(response => {
          this.current = response.data.current
          this.effects = response.data.effects
          this.effectsPending = false
        })
        .catch(error => {
          console.log(error)
          this.effectsPending = false
        })
    },
    isEffectSelected: (effect, c) => effect === c,
    changeEffect (effect) {
      if (effect === this.current) return
      http.post('effect', {
        current: effect
      })
        .then(response => {
          this.current = effect
        })
        .catch(error => {
          console.log(error)
        })
    }
  }
}

</script>

<style lang="scss">
h1.headline {
  background-color: #666666;
  -webkit-background-clip: text;
  -moz-background-clip: text;
  background-clip: text;
  color: transparent;
  text-shadow: rgba(255,255,255,0.5) 0px 3px 3px;
}
</style>
