input.calibrate_compass()
VFORWARDS = -60
FORWARDS = -30
BACKWARDS = 30
LEFT = -30
RIGHT = 30
led.plot(0, 0)
led.plot(1, 1)
led.plot(2, 2)
led.plot(3, 3)
led.plot(4, 4)

def on_forever():
    if input.rotation(Rotation.PITCH) > BACKWARDS:
        pins.digital_write_pin(DigitalPin.P0, 0)
        pins.digital_write_pin(DigitalPin.P1, 0)
        pins.digital_write_pin(DigitalPin.P2, 1)
    elif input.rotation(Rotation.PITCH) < VFORWARDS:
        pins.digital_write_pin(DigitalPin.P0, 1)
        pins.digital_write_pin(DigitalPin.P1, 1)
        pins.digital_write_pin(DigitalPin.P2, 1)
    elif input.rotation(Rotation.PITCH) < FORWARDS:
        if input.rotation(Rotation.ROLL) > RIGHT:
            pins.digital_write_pin(DigitalPin.P0, 1)
            pins.digital_write_pin(DigitalPin.P1, 0)
            pins.digital_write_pin(DigitalPin.P2, 1)
        elif input.rotation(Rotation.ROLL) < LEFT:
            pins.digital_write_pin(DigitalPin.P0, 1)
            pins.digital_write_pin(DigitalPin.P1, 0)
            pins.digital_write_pin(DigitalPin.P2, 0)
        else:
            pins.digital_write_pin(DigitalPin.P0, 1)
            pins.digital_write_pin(DigitalPin.P1, 1)
            pins.digital_write_pin(DigitalPin.P2, 0)
    elif input.rotation(Rotation.ROLL) > RIGHT:
        pins.digital_write_pin(DigitalPin.P0, 0)
        pins.digital_write_pin(DigitalPin.P1, 1)
        pins.digital_write_pin(DigitalPin.P2, 1)
    elif input.rotation(Rotation.ROLL) < LEFT:
        pins.digital_write_pin(DigitalPin.P0, 0)
        pins.digital_write_pin(DigitalPin.P1, 1)
        pins.digital_write_pin(DigitalPin.P2, 0)
    else:
        pins.digital_write_pin(DigitalPin.P0, 0)
        pins.digital_write_pin(DigitalPin.P1, 0)
        pins.digital_write_pin(DigitalPin.P2, 0)
basic.forever(on_forever)
