
val = 0x42123A

mantissa = val & 0x00FFFF
exponent = val>>15

print(bin(val), val)
print(bin(mantissa), mantissa)
print(bin(exponent), exponent)
