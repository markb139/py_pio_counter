int initialise();

int claim_statemachine();
void release_statemachine(int sm);
int capture(int sm, uint32_t *bigbuf, uint32_t data_size);
uint load_program(int sm, int gpio_pin, int clock_rate);
void start_program(int sm);
void stop_program(int sm, uint offset);
int rx_fifo_level(int sm);