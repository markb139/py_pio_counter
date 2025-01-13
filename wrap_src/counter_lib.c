#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hardware/pio.h"
#include "piolib.h"

#include "counter.pio.h"

// #include "counter_lib.h"

static inline void counter_program_init(PIO pio, uint sm, uint offset, uint pin, float div) {
    pio_sm_config c = counter_program_get_default_config(offset);

    // Map the state machine's OUT pin group to one pin, namely the `pin`
    // parameter to this function.

    // Set this pin's GPIO function (connect PIO to the pad)
    pio_gpio_init(pio, pin);
    // Set the pin direction to output at the PIO

    sm_config_set_in_pins(&c, pin);
    sm_config_set_jmp_pin(&c, pin);
    sm_config_set_clkdiv(&c, div);

    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_RX);
    pio_sm_clear_fifos(pio, sm);

    pio_sm_config_xfer(pio, sm, PIO_DIR_FROM_SM, 0x1000, 4);
    // printf("pio_sm_config_xfer %d\r\n",);

    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, &c);
}

int get_readings(PIO pio, uint sm, uint32_t *bigbuf, uint32_t data_size)
{
    int ret = 0;
    uint32_t readings = data_size >> 2;
    // for (uint32_t i = 0; i < readings; i++)
    // {
    //     bigbuf[i]= 0xdeadbeef;
    // }
    ret = pio_sm_xfer_data(pio, sm, PIO_DIR_FROM_SM, data_size, bigbuf);
    // printf("xfer ret %d\r\n", ret);

    return ret;
}

int claim_statemachine()
{
    PIO pio = pio0;
    return pio_claim_unused_sm(pio, true);
}

void release_statemachine(int sm)
{
    PIO pio = pio0;
    pio_sm_unclaim(pio, sm);
}

int initialise()
{
    return 0;
}

int capture(int sm, uint32_t *bigbuf, uint32_t data_size)
{
    PIO pio = pio0;

    return get_readings(pio,sm, bigbuf, data_size);
}

uint load_program(int sm, int gpio_pin, int clock_rate)
{
    PIO pio = pio0;

    float div = clock_get_hz(clk_sys) / clock_rate; // 2375000
    // printf("clk div %02f\r\n", div);
    
    uint offset = pio_add_program(pio, &counter_program);

    // printf("Loaded program at %d, using sm %d, gpio %d\n", offset, sm, gpio_pin);

    counter_program_init(pio, sm, offset, gpio_pin, div);

    return offset;
}

void start_program(int sm)
{
    PIO pio = pio0;
    // Set the state machine running
    pio_sm_set_enabled(pio, sm, true);
}

void stop_program(int sm, uint offset)
{
    PIO pio = pio0;
    pio_sm_set_enabled(pio, sm, false);
    
    pio_remove_program(pio, &counter_program, offset);
    pio_clear_instruction_memory(pio);
}


uint rx_fifo_level(int sm)
{ 
    PIO pio = pio0;
    return pio_sm_get_rx_fifo_level(pio, sm);
}
