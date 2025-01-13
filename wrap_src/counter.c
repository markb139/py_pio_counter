#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hardware/pio.h"
#include "piolib.h"

#include "counter.pio.h"

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

    printf("config xfer %d\r\n",pio_sm_config_xfer(pio, sm, PIO_DIR_FROM_SM, 1024, 2));

    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, &c);
    // Set the state machine running
    pio_sm_set_enabled(pio, sm, true);
}

void get_readings(PIO pio, uint sm)
{
    int ret = 0;
    uint32_t readings = 32;
    uint32_t data_size = readings * sizeof(uint32_t);
    uint32_t *bigbuf = (uint32_t *) malloc(data_size+4);
    for (uint32_t i = 0; i < readings; i++)
    {
        bigbuf[i]= 0xdeadbeef;
    }

    printf("bigbuf %d (bytes) : 0x%08x\r\n", data_size, bigbuf);
    for(uint32_t j=0;j<32;j++)
    {
        ret = pio_sm_xfer_data(pio, sm, PIO_DIR_FROM_SM, data_size, bigbuf);
        if (!ret) 
        {
            for (uint32_t i = 0; i < readings; i++)
            {
                uint32_t v = bigbuf[i];
                uint32_t count = ~v;
                uint32_t rate = 60e4/(count);
                printf(" 0x%08x: %d %d\r\n", i, count, 100*rate);
                fflush(stdout);
            }
            
        }
        else
        {
            printf("xfer ret %d\r\n", ret);
        }
    }
    printf("\r\n");
    free(bigbuf);
}

int main(int argc, const char **argv) 
{
    stdio_init_all();

    PIO pio = pio0;
    printf("is claimed %d\r\n", pio_sm_is_claimed(pio,0));
    if (pio_sm_is_claimed(pio,0))
        pio_sm_unclaim(pio, 0);

    int sm = pio_claim_unused_sm(pio, true);
    printf("SM %d\r\n", sm);
    float div = clock_get_hz(clk_sys) / 2375000;
    printf("clk div %02f\r\n", div);

    uint offset = pio_add_program(pio, &counter_program);
    uint gpio = 23;
    if (argc == 2)
        gpio = (uint)strtoul(argv[1], NULL, 0);
    printf("Loaded program at %d, using sm %d, gpio %d\n", offset, sm, gpio);

    counter_program_init(pio, sm, offset, gpio, div);
    
    printf("Fifo Level 0x%08x\r\n", pio_sm_get_rx_fifo_level(pio, sm));

    get_readings(pio,sm);

    // time_t start = clock();
    // uint32_t diff = 0;
    // do {
    //     time_t end = clock();
    //     diff = end - start;
    //     // printf("diff %d\r\n", diff);
    //     if(!pio_sm_is_rx_fifo_empty(pio, 0) )
    //     {
    //     uint32_t val = ~ pio_sm_get_blocking(pio, sm);
    //     printf("Fifo val %d\r\n", 60000000/val);
    //     }

    // }while(diff < 10000000);

    
    pio_sm_set_enabled(pio, sm, false);

    while(pio_sm_get_rx_fifo_level(pio, 0) >0 )
    {
        uint32_t val = ~ pio_sm_get_blocking(pio, sm);
        printf("Fifo val 0x%08x\r\n", val);
    }

    pio_sm_unclaim(pio, sm);

    printf("Done\r\n");
    
    return 0;
}