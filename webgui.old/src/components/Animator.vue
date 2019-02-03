<template>
  <div>
    <div class="row">
      <div class="col-3 mb-2">
        Randomize playback
      </div>
      <div class="col-9 mb-2">
        <button type="button" class="btn" :class="{ 'btn-success': randomize, 'btn-secondary': !randomize }" data-toggle="button" :aria-pressed="randomize" autocomplete="off" @click="toggleRandomization">
          {{ randomize ? 'Yes' : 'No' }}
        </button>
      </div>
    </div>
    <div class="row">
      <div class="col-3">
        Currently playing
      </div>
      <div class="col">
        <div class="input-group mb-3">
          <select class="custom-select select-sm" id="inputGroupSelect03" v-model="selectedAnimation" @change="changeAnimation">
            <option v-for="animation in animations" :key="animation" :selected="animation === currentAnimation" :value="animation">{{ animation }}</option>
          </select>
          <div class="input-group-append">
            <button class="btn btn-success" type="button" @click="nextAnimation">Next animation</button>
          </div>
        </div>
      </div>
    </div>
    <div v-if="cycle != null" class="row mb-4">
      <div class="col-3">
        Animation play time
      </div>
      <div class="col">
        <vue-slider :data="cycleTimes" tooltipDir="bottom" :piecewise="true" :formatter="function (value) { return `${value} seconds` }" @callback="changeCycle" ></vue-slider>
      </div>
    </div>
  </div>
</template>

<script>
import http from 'axios'
import _ from 'lodash'
import vueSlider from 'vue-slider-component'

export default {
  name: 'Animator',
  data () {
    return {
      currentAnimation: null,
      selectedAnimation: null,
      randomize: null,
      cycle: null,
      cycleTimes: null,
      animations: null,
      interval: null,
      animatorPending: false
    }
  },
  props: [
    'baseUrl'
  ],
  components: {
    vueSlider
  },
  mounted () {
    this.getAnimator()

    this.interval = setInterval(() => {
      this.getAnimator()
    }, 2000)
  },
  beforeDestroy () {
    clearInterval(this.interval)
    this.interval = null
  },
  methods: {
    getAnimator () {
      if (this.animatorPending) return
      this.animatorPending = true
      http.get(this.baseUrl + 'animator')
        .then((response) => {
          this.currentAnimation = response.data.currentAnimation
          this.selectedAnimation = response.data.currentAnimation
          this.randomize = response.data.randomize
          this.cycle = response.data.cycle
          this.cycleTimes = response.data.cycleTimes
          this.animations = response.data.animations
          this.animatorPending = false
        })
        .catch((error) => {
          console.log(error)
          this.animatorPending = false
        })
    },
    toggleRandomization () {
      http.post(this.baseUrl + 'animator', {
        randomize: !this.randomize
      })
        .then((response) => {
          this.getAnimator()
        })
        .catch((error) => {
          console.log(error)
        })
    },
    changeAnimation () {
      http.post(this.baseUrl + 'animator', {
        next: this.selectedAnimation
      })
        .then((response) => {
          this.getAnimator()
        })
        .catch((error) => {
          console.log(error)
        })
    },
    nextAnimation () {
      http.post(this.baseUrl + 'animator', {
        next: true
      })
        .then((response) => {
          this.getAnimator()
        })
        .catch((error) => {
          console.log(error)
        })
    },
    changeCycle: _.debounce(function (value) {
      http.post(this.baseUrl + 'animator', {
        cycle: this.cycleTimes.indexOf(value)
      })
        .then((response) => {
          this.getAnimator()
        })
        .catch((error) => {
          console.log(error)
        })
    }, 100)
  }
}
</script>

<style lang="sass">

</style>
