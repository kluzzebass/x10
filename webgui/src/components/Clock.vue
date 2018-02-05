<template>
  <div>
    <button class="btn" :class="{ 'btn-danger': !clockAdjusted, 'btn-success': clockAdjusted }" :disabled="adjustmentPending || clockAdjusted" @click="adjust">{{ buttonText }}</button>
  </div>
</template>

<script>
import http from 'axios'

export default {
  name: 'Clock',
  data () {
    return {
      buttonText: 'Adjust clock now',
      clockAdjusted: false,
      adjustmentPending: false
    }
  },
  props: [
    'baseUrl'
  ],
  methods: {
    adjust () {
      let months = ['Jan', 'Feb', 'Mar', 'Apr', 'May', 'Jun', 'Jul', 'Aug', 'Sep', 'Oct', 'Nov', 'Dec']
      if (this.adjustmentPending) return
      this.adjustMentPending = true
      // {"date":"Nov 25 2017","time":"18:28:45"}
      let now = new Date()

      let h = now.getHours()
      let m = now.getMinutes()
      let s = now.getSeconds()

      let timeStr = (h < 10 ? '0' : '') + h + ':' + (m < 10 ? '0' : '') + m + ':' + (s < 10 ? '0' : '') + s
      let dateStr = months[now.getMonth() - 1] + ' ' + now.getDate() + ' ' + now.getFullYear()

      console.log(dateStr)
      http.post(this.baseUrl + 'datetime', {
        date: dateStr,
        time: timeStr
      })
        .then((response) => {
          this.adjustmentPending = false
          this.clockAdjusted = true
          this.buttonText = 'Clock adjusted'
        })
        .catch((error) => {
          console.log(error)
          this.adjustmentPending = false
        })
    }
  }
}
</script>

<style lang="sass">

</style>
