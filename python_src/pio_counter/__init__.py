from ._pio_counter import _claim_statemachine, _release_statemachine, _initialise, _capture, _load_program, _start_program, _stop_program, _rx_fifo_level


class PIOCounter:
    gpio_pin = 0
    clock_rate = 0
    sm = -1
    offset = -1
    
    def claim_statemachine(self):
        self.sm = _claim_statemachine()

    def release_statemachine(self):
        _release_statemachine(self.sm)
    
    def initialise(self, gpio_pin, clock_rate):
        self.gpio_pin = gpio_pin
        self.clock_rate = clock_rate
        return _initialise()

    def capture(self, buffer):
        return _capture(self.sm, buffer)
    
    def load_program(self):
        self.offset = _load_program(self.sm, self.gpio_pin, self.clock_rate)
    
    def start_program(self):
        return _start_program(self.sm)
        
    def stop_program(self):
        return _stop_program(self.sm, self.offset)
    
    def rx_fifo_level(self):
        return _rx_fifo_level(self.sm)

    def __repr__(self):
        return f"PIOCounter: sm={self.sm} gpio={self.gpio_pin} clock={self.clock_rate} offset={self.offset}"

__all__ = [PIOCounter]
