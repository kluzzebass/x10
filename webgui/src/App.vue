<template>
  <div id="app">
    <mainmenu/>
  </div>
</template>

<script>
import MainMenu from './components/MainMenu'

export default {
  name: 'App',
  components: {
    'mainmenu': MainMenu
  }
}
</script>

<style lang="scss">
@import 'styles/global.scss';
@import '~bootstrap/scss/bootstrap.scss';
</style>
