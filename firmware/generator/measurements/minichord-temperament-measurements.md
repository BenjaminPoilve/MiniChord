# Temperament measurements

Measured from the minichord's USB audio, harp strings, master tuning 440.0 Hz.
Cents are offsets from equal temperament at A4 = 440. *expected* is the firmware table,
*measured* the median of the readings for that pitch class. The last two columns are the
largest difference from the table, raw and after removing the run's common shift.

| Temperament | | C | C# | D | Eb | E | F | F# | G | Ab | A | Bb | B | shift | worst | worst after shift |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 0 equal | expected | +0 | +0 | +0 | +0 | +0 | +0 | +0 | +0 | +0 | +0 | +0 | +0 | | | |
| | measured | -0.7 | -0.1 | -0.1 | -0.2 | -0.4 | +0.0 | +0.2 | -0.3 | -0.3 | -0.4 | -0.4 | -0.1 | -0.24 | 0.66 | 0.43 |
| 1 quarter-comma meantone | expected | +10 | -14 | +3 | +21 | -3 | +14 | -10 | +7 | -17 | +0 | +17 | -7 | | | |
| | measured | +9.6 | -14.2 | +2.8 | +20.6 | -3.2 | +13.9 | -10.4 | +6.7 | -16.9 | -0.4 | +16.7 | -7.3 | -0.28 | 0.45 | 0.39 |
| 2 five-limit just (C major) | expected | +16 | +27 | +20 | +31 | +2 | +14 | +6 | +18 | +29 | +0 | +12 | +4 | | | |
| | measured | +15.7 | +26.8 | +20.0 | +30.7 | +1.3 | +13.9 | +5.9 | +17.5 | +28.8 | -0.4 | +11.7 | +3.8 | -0.24 | 0.67 | 0.43 |
| 3 pythagorean | expected | -6 | +8 | -2 | -12 | +2 | -8 | +6 | -4 | +10 | +0 | -10 | +4 | | | |
| | measured | -6.1 | +8.0 | -2.3 | -11.9 | +1.3 | -8.1 | +5.9 | -4.0 | +9.9 | -0.0 | -10.0 | +3.9 | -0.07 | 0.74 | 0.66 |
| 4 werckmeister III | expected | +12 | +2 | +4 | +6 | +2 | +10 | +0 | +8 | +4 | +0 | +8 | +4 | | | |
| | measured | +11.6 | +1.9 | +3.9 | +6.2 | +1.4 | +9.4 | +0.0 | +7.9 | +3.6 | -0.2 | +7.9 | +3.8 | -0.15 | 0.61 | 0.46 |
| 5 kirnberger III | expected | +10 | +0 | +3 | +4 | -3 | +8 | +0 | +7 | +2 | +0 | +6 | -1 | | | |
| | measured | +9.7 | -0.2 | +2.8 | +3.4 | -3.2 | +7.6 | +0.1 | +6.8 | +1.8 | -0.5 | +5.9 | -1.4 | -0.22 | 0.57 | 0.34 |
| 6 vallotti | expected | +6 | +0 | +2 | +4 | -2 | +8 | -2 | +4 | +2 | +0 | +6 | -4 | | | |
| | measured | +5.6 | -0.2 | +1.9 | +3.4 | -2.3 | +7.7 | -2.5 | +3.8 | +1.8 | -0.4 | +5.9 | -4.3 | -0.30 | 0.59 | 0.28 |
| 7 young no. 2 | expected | +6 | -4 | +2 | +0 | -2 | +4 | -6 | +4 | -2 | +0 | +2 | -4 | | | |
| | measured | +5.6 | -4.2 | +1.9 | -0.3 | -2.4 | +3.6 | -6.0 | +3.8 | -2.2 | -0.5 | +1.7 | -4.2 | -0.25 | 0.47 | 0.28 |
| 8 kellner | expected | +8 | -2 | +3 | +2 | -3 | +6 | -4 | +5 | +0 | +0 | +4 | -1 | | | |
| | measured | +7.7 | -2.2 | +2.8 | +1.4 | -3.3 | +5.7 | -4.0 | +4.8 | -0.3 | -0.4 | +3.8 | -1.4 | -0.27 | 0.63 | 0.36 |
| 9 sixth-comma meantone | expected | +5 | -7 | +2 | +10 | -2 | +7 | -5 | +3 | -8 | +0 | +8 | -3 | | | |
| | measured | +4.6 | -7.2 | +1.8 | +9.9 | -2.4 | +6.5 | -4.9 | +2.8 | -8.4 | -0.5 | +7.9 | -3.3 | -0.24 | 0.48 | 0.38 |
