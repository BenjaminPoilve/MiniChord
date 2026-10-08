This script uses a Venv. Use the requirements.txt to set it up. 

## Scripts

`generate.py` builds everything that follows from `parameters.json`: `include/sysex_handler.h`, `include/parameter_introduction.h`, `lib/potentiometer/src/parameter_lookup.h`, and minicontrol (`minicontrol/index.html` and `minicontrol/json/parameters.json`).

`temperaments.py` defines the temperaments. It writes `include/temperament_profiles.h` and keeps the temperament parameter's range and tooltip in `parameters.json` in step, so run it before `generate.py` whenever it changes:

```bash
python3 temperaments.py
python3 generate.py
```

`python3 temperaments.py --check` exits non-zero if either generated file is out of date. It needs only the Python standard library.

## Adding a temperament

Add an entry at the end of `PROFILES` in `temperaments.py`. Never insert or reorder: a temperament's position is the number stored in presets. (24-EDO sits between 19 and 31 because the three divisions arrived together, before any of them was released.) Describe it the way a tuner would, with `fifths()` (the twelve fifths round the circle as deviations from pure, one left as `None` to close the circle) or `ratios()` (the twelve notes as ratios to C), then run both scripts.

`temperament_test.cpp` checks the arithmetic in `include/temperament.h` against the generated table, including the chord glide path:

```bash
g++ -std=c++17 -O2 -I../include temperament_test.cpp -o /tmp/temperament_test && /tmp/temperament_test
```
