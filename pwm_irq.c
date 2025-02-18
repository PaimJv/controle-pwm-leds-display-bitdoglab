#include "pico/bootrom.h"
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define JOYSTICK_X_PIN 26
#define JOYSTICK_Y_PIN 27
#define JOYSTICK_PB 22
#define Botao_A 5
#define botaoB 6

#define LED_R 13
#define LED_G 11
#define LED_B 12
#define PWM_WRAP 4095
#define JOY_CENTER 2048
#define JOY_THRESHOLD 100
#define SQUARE_SIZE 8
#define VELOCITY_DIVISOR 256  
#define JOY_MAX 4095

int ultimo_estado_pb;
int ultimo_estado_a;
uint intervalo_micro = 400000;

ssd1306_t ssd;
bool cor = true;
uint16_t red_intensity;
uint16_t blue_intensity;
bool led_g_state = false;
bool pwm_enabled = true;
bool borda;
volatile bool joystick_pb_pressed = false;  // Flag para interrupção

// Função para mapear valores de um intervalo para outro
int32_t map_value(int32_t value, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min; 
}

void init_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_wrap(&config, PWM_WRAP);
    pwm_init(slice_num, &config, true);
}

void set_pwm_duty(uint gpio, uint16_t duty) {
    pwm_set_gpio_level(gpio, duty);
}

void irq_handler(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (gpio == JOYSTICK_PB && current_time - ultimo_estado_pb > intervalo_micro) {
        ultimo_estado_pb = current_time;
        borda = !borda;
        led_g_state = !led_g_state;
        gpio_put(LED_G, led_g_state);
    }
    if (gpio == Botao_A && current_time - ultimo_estado_a > intervalo_micro) {
        ultimo_estado_a = current_time;
        pwm_enabled = !pwm_enabled;
        while (!gpio_get(Botao_A));
    }
}

int main() {
    gpio_init(JOYSTICK_PB);
    gpio_set_dir(JOYSTICK_PB, GPIO_IN);
    gpio_pull_up(JOYSTICK_PB);
    
    gpio_init(Botao_A);
    gpio_set_dir(Botao_A, GPIO_IN);
    gpio_pull_up(Botao_A);

    gpio_set_irq_enabled_with_callback(JOYSTICK_PB, GPIO_IRQ_EDGE_FALL, true, &irq_handler);
    gpio_set_irq_enabled(Botao_A, GPIO_IRQ_EDGE_FALL, true);
    
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);

    // Inicializa I2C e display
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);

    init_pwm(LED_R);
    init_pwm(LED_B);

    uint16_t adc_value_x, adc_value_y;
    int16_t square_x = (HEIGHT - SQUARE_SIZE) / 2;
    int16_t square_y = (WIDTH - SQUARE_SIZE) / 2;

    while (true) {
        // Leitura do joystick
        adc_select_input(0); // Bitdoglab
        // adc_select_input(1); // Simulador
        adc_value_x = adc_read();
        adc_select_input(1);
        // adc_select_input(0); // Simulador
        adc_value_y = adc_read();

        int16_t square_x = map_value(adc_value_x, JOY_MAX, 0, 0, HEIGHT - SQUARE_SIZE);
        int16_t square_y = map_value(adc_value_y, 0, JOY_MAX, 0, WIDTH - SQUARE_SIZE);

        int16_t delta_x = adc_value_y - JOY_CENTER;
        int16_t delta_y = adc_value_x - JOY_CENTER;

        if (abs(delta_x) > JOY_THRESHOLD) {
            square_x -= delta_x / VELOCITY_DIVISOR; 
        }
        if (abs(delta_y) > JOY_THRESHOLD) {
            square_y += delta_y / VELOCITY_DIVISOR; 
        }

        // Limites do movimento
        if (square_x < 0) square_x = 0;
        else if (square_x > HEIGHT - SQUARE_SIZE) square_x = HEIGHT - SQUARE_SIZE;
        
        if (square_y < 0) square_y = 0;
        else if (square_y > WIDTH - SQUARE_SIZE) square_y = WIDTH - SQUARE_SIZE;

        // Atualiza o PWM dos LEDs
        if (pwm_enabled) {
            red_intensity = (abs(delta_x) > JOY_THRESHOLD) ? abs(delta_x) * PWM_WRAP / JOY_CENTER : 0;
            blue_intensity = (abs(delta_y) > JOY_THRESHOLD) ? abs(delta_y) * PWM_WRAP / JOY_CENTER : 0;

            set_pwm_duty(LED_R, red_intensity);
            set_pwm_duty(LED_B, blue_intensity);
        } else {
            set_pwm_duty(LED_R, 0);
            set_pwm_duty(LED_B, 0);
        }

        // Atualiza o display
        ssd1306_fill(&ssd, false);
        if (borda) {
          ssd1306_rect(&ssd, 3, 3, 122, 60, cor, !cor);
        } else {
            ssd1306_line(&ssd, 3, 3, 122, 3, cor);
            ssd1306_line(&ssd, 3, 60, 122, 60, cor);
            ssd1306_line(&ssd, 3, 8, 3, 55, cor);
            ssd1306_line(&ssd, 122, 8, 122, 55, cor);
        }
        ssd1306_rect(&ssd, square_x, square_y, SQUARE_SIZE, SQUARE_SIZE, cor, !cor);
        ssd1306_send_data(&ssd);

        sleep_ms(20);
    }
}
