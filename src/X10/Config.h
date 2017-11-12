
#ifndef X10_CONFIG_H
#define X10_CONFIG_H

#define LED_PIN D3
#define WIDTH 16
#define HEIGHT 16
#define NUM_LEDS WIDTH * HEIGHT
#define CHIPSET WS2812
#define COLOR_ORDER GRB
#define MAX_BRIGHTNESS 100
#define COLOR_CORRECTION 0xFFB0F0
#define COLOR_TEMPERATURE 0xffffff

#define CS_PIN D8
#define SPI_SPEED SPI_FULL_SPEED

// Generic buffer length used for buffers and ini file lines
#define BUFFER_LEN 100

// Maximum path length
#define PATH_MAX 256


#define PIXEL_BUFFER_SIZE 32
#define X10_CFG_DIR "/"
#define X10_CFG_FILE "x10.ini"
#define ANIM_DIR "/anim"
#define ANIM_CFG_FILE "config.ini"
#define ANIM_HOLD_TIME 200
#define ANIM_CYCLE_MODE 2


#endif /* X10_CONFIG_H */



