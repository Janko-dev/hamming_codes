# Hamming Codes
Linear algebra to illustrate hamming codes. 4 bits of data must be provided by which 4 additional parity bits are generated to encode the data as a byte. In the ASCII standard, 4 bits are encoded with 3 parity bits, because the most significant bit is unused, leading to a [7, 3] hamming code. However I used the most significant bit by which [8, 4] hamming codes can be generated. The mechanism allows for the localisation and correction of singe bit errors.

## Quick start
```
> make
> ./hc 0010
```
Specify a 4 bit nibble as the argument of the program as shown above.

```
input nibble as vector:
---------------
  0
  0
  1
  0

Matrix G:
---------------
  1   0   0   0
  0   1   0   0
  0   0   1   0
  0   0   0   1 
  0   1   1   1
  1   0   1   1
  1   1   0   1
  1   1   1   0

Matrix H:
---------------
  0   1   1   1   1   0   0   0
  1   0   1   1   0   1   0   0
  1   1   0   1   0   0   1   0
  1   1   1   0   0   0   0   1

Encoded byte: 0x2d
Altered byte by making last bit 1: 0x29

Parity vector
When vector == 0, there was no change
When vector != 0, the index of the columns of H-matrix determine where the error is:
---------------
  0
  1
  0
  0

Corrected byte: 0x2d
```
## References
- https://en.wikipedia.org/wiki/Hamming_code
