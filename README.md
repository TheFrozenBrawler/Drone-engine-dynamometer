# Hamownia_software
This is a project hosting software for "hamownia" - dynamometr of drone engines. This is a part of our master's thesis

## Timer Interrupt - IMPORTANT!
This version of software implements timer interrupt. Because there are different ways to implement it between chips, this software version works only with Renesas RA4M1 chips, ex. Arduino UNO R4.

## Anemometer scaling
### Calibration points
| Type of point | Voltage (V) | Wind speed (m/s) |
|---------------|-------------|------------------|
| Zero          | 0.0         | 0.0              |
| Setting 1     | 0.8         | 4.8              |
| Setting 2     | 2.45        | 6.5              |
| Setting 3     | 3.44        | 9.3              |

Use more precise air device and real anemometer to scale

## MLX90614 sensor - info
### Effective distance
The effective distance of the MLX90614 sensor is approximately 1.5 cm, FOV = 90deg.

### Emissivity
The emissivity value for the MLX90614 sensor is set to XX
The surface of measurement should be coated with black electric tape.
