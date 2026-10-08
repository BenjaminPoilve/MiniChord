#!/usr/bin/env python3
"""Temperaments for the minichord: the one place they are defined.

Each temperament is one entry in PROFILES. Running this script writes

  ../include/temperament_profiles.h   the table the firmware reads
  parameters.json                     the temperament parameter's max_value and tooltip

and prints the options and notes for an editor's temperament control.

A temperament's number is stored in presets, so its position in PROFILES must
never change: add new ones at the end only.

Every temperament is defined the way a tuner would describe it and turned into
whole-cent offsets from equal temperament for the twelve pitch classes:

  fifths(...)   twelve fifths round the circle, C-G, G-D, D-A, A-E, E-B, B-F#,
                F#-C#, C#-G#, G#-D#, D#-A#, A#-F, F-C, each given as its deviation
                from a pure 3:2 in cents. Exactly one is None: it takes whatever
                is needed for the circle to close (the wolf, in a meantone).
  ratios(...)   the twelve notes C, C# ... B as frequency ratios to C.
  edo(n)        a division of the octave into n steps instead of twelve. These
                need note tables in main.cpp (the edo_* arrays), so only the
                divisions that already have them are accepted.

Offsets are then shifted so that A has none. A therefore sounds at the master
tuning pitch in every temperament, the way a tuner sets A to the fork and
tempers the rest around it. The tuning is still fixed to the keyboard: which
keys are sweet and which are rough does not follow the key signature.

Run from anywhere:
  python3 temperaments.py           regenerate
  python3 temperaments.py --check   exit 1 if a generated file is out of date
"""
import json, os, re, sys
from math import log2

PURE_FIFTH = 1200 * log2(3 / 2)
SYNTONIC_COMMA = 1200 * log2(81 / 80)
PYTHAGOREAN_COMMA = 1200 * log2(531441 / 524288)
SCHISMA = PYTHAGOREAN_COMMA - SYNTONIC_COMMA
ANCHOR = 9  # A
# divisions main.cpp has note tables for, and their index into edo_steps[]
EDO_INDEX = {12: 0, 19: 1, 31: 2, 24: 3}


def fifths(deviations):
    assert len(deviations) == 12 and deviations.count(None) == 1, "twelve fifths, exactly one None"
    dev = list(deviations)
    dev[dev.index(None)] = 7 * 1200 - 12 * PURE_FIFTH - sum(d for d in dev if d is not None)
    pitch, pos, pc = [0.0] * 12, 0.0, 0
    for k in range(11):
        pos += PURE_FIFTH + dev[k]
        pc = (pc + 7) % 12
        pitch[pc] = pos % 1200
    return [pitch[i] - 100 * i for i in range(12)]


def ratios(rs):
    assert len(rs) == 12 and rs[0] == 1, "twelve ratios starting from 1"
    return [1200 * log2(r) - 100 * i for i, r in enumerate(rs)]


def edo(n):
    assert n in EDO_INDEX and n != 12, f"{n}-EDO has no note tables in main.cpp; generate them with gen_edo.py first"
    return n


def fifths_meantone(fraction_of_syntonic_comma):
    """Eleven fifths narrowed alike, the wolf between G# and Eb (the usual Eb-G# keyboard)."""
    d = -SYNTONIC_COMMA * fraction_of_syntonic_comma
    return fifths([d] * 8 + [None] + [d] * 3)


# ---------------------------------------------------------------------------
# name:   in the parameters.json tooltip
# label:  a short name for an editor's option list
# note:   an editor's explanation of the option
# cents:  fifths(), fifths_meantone() or ratios()
PROFILES = [
    dict(name="equal", label="Equal",
         note="Twelve identical steps, the default. Every key sounds the same and no interval but the octave is quite in tune: a major third is 13.7 cents wide, the faint beating in a piano chord.",
         cents=[0.0] * 12),
    dict(name="quarter-comma meantone", label="Meantone",
         note="Quarter-comma meantone, what most Renaissance and early Baroque keyboard music was written for. Major thirds are pure and fifths pay for it at 5.4 cents narrow. The wolf sits between G# and Eb: Eb, Bb, F, C, G, D, A and E major are sweet, B, F#, Db and Ab major unusable.",
         cents=fifths_meantone(1 / 4)),
    dict(name="five-limit just (C major)", label="Just",
         note="Five-limit just intonation for C major. Thirds and fifths dead in tune in the home key, noticeably out in others.",
         cents=ratios([1, 16/15, 9/8, 6/5, 5/4, 4/3, 45/32, 3/2, 8/5, 5/3, 16/9, 15/8])),
    dict(name="pythagorean", label="Pythagorean",
         note="Pure 3:2 fifths from Eb round to G#. Bright, wide thirds at 21.5 cents sharp. Right for medieval music, wrong for most of what came after.",
         cents=fifths([0] * 8 + [None] + [0] * 3)),
    dict(name="werckmeister III", label="Werckmeister III",
         note="Andreas Werckmeister, 1691. Four fifths (C–G, G–D, D–A and B–F#) narrowed by a quarter of the Pythagorean comma, the rest pure. Every key is playable: C and F major have the calmest thirds, 3.9 cents wide, and Db, F# and Ab major the widest, 21.5.",
         cents=fifths([-PYTHAGOREAN_COMMA / 4] * 3 + [0, 0, -PYTHAGOREAN_COMMA / 4] + [0] * 5 + [None])),
    dict(name="kirnberger III", label="Kirnberger III",
         note="Johann Philipp Kirnberger, 1779. The fifths from C to E are narrowed by a quarter of the syntonic comma, so C–E is a pure 5:4; F#–C# gives up a schisma and the rest are pure. The home keys are very sweet and the far ones pointedly bright.",
         cents=fifths([-SYNTONIC_COMMA / 4] * 4 + [0, 0, -SCHISMA] + [0] * 4 + [None])),
    dict(name="vallotti", label="Vallotti",
         note="Francesco Antonio Vallotti, 18th century. The six fifths from F to B are narrowed by a sixth of the Pythagorean comma and the other six are pure. Smooth and even-handed, and a common choice today for Baroque music: thirds from 5.9 cents wide in F, C and G major to 21.5 in Db, F# and B.",
         cents=fifths([-PYTHAGOREAN_COMMA / 6] * 5 + [0] * 6 + [None])),
    dict(name="young no. 2", label="Young",
         note="Thomas Young, 1800. Vallotti's shape moved up a fifth: the six fifths from C to F# are narrowed by a sixth of the Pythagorean comma, the rest pure. C, D and G major have the calmest thirds, 5.9 cents wide.",
         cents=fifths([-PYTHAGOREAN_COMMA / 6] * 6 + [0] * 5 + [None])),
    dict(name="kellner", label="Kellner",
         note="Herbert Anton Kellner's 1977 proposal for the tuning of Bach's Well-Tempered Clavier. Five fifths (C–G, G–D, D–A, A–E and B–F#) narrowed by a fifth of the Pythagorean comma. Thirds from 2.7 cents wide in C major to 21.5 in Db, F# and Ab.",
         cents=fifths([-PYTHAGOREAN_COMMA / 5] * 4 + [0, -PYTHAGOREAN_COMMA / 5] + [0] * 5 + [None])),
    dict(name="sixth-comma meantone", label="1/6 Meantone",
         note="Meantone with fifths narrowed by a sixth of the syntonic comma. Major thirds are 7.2 cents wide instead of pure, and the wolf between G# and Eb shrinks to 16 cents, so more keys are usable. Often associated with Gottfried Silbermann's organs.",
         cents=fifths_meantone(1 / 6)),
    dict(name="19-EDO", label="19-EDO",
         note="Nineteen steps to the octave. The buttons mean exactly what they did, but C# and Db are now different notes a step apart, with C# the lower. Minor thirds land within a cent of pure; fifths pay 7 cents for it.",
         cents=edo(19)),
    dict(name="24-EDO (quarter tones)", label="24-EDO",
         note="Twenty-four steps: twelve, with a quarter-tone between each pair. The buttons and key signatures mean what they always did, and the modifier moves a note by a quarter-tone instead of a semitone, so it plays the notes between the frets. The neutral third, halfway between major and minor, is right there. Over MIDI the quarter-tones round to a semitone.",
         cents=edo(24)),
    dict(name="31-EDO", label="31-EDO",
         note="Thirty-one steps. Major thirds essentially exact, and the augmented sixth lands within a cent of the 7:4 harmonic seventh — the interval twelve-note tuning has no room for. Sharps and flats are two steps apart here.",
         cents=edo(31)),
    # add new temperaments here, at the end
]

TOOLTIP_TAIL = ("A keeps the master tuning pitch in the twelve-note ones, which are fixed to "
                "the keyboard, sweetest around C, so far keys sound progressively rougher, as "
                "they did historically. 10 and 11 divide the octave differently instead")
TEMPERAMENT_ADDRESS = 237
HERE = os.path.dirname(os.path.abspath(__file__))
HEADER = os.path.join(HERE, "..", "include", "temperament_profiles.h")
PARAMETERS = os.path.join(HERE, "parameters.json")


def division(profile):
    """The octave division a temperament uses: 12 unless it is an edo()."""
    return profile["cents"] if isinstance(profile["cents"], int) else 12


def offsets(profile):
    if division(profile) != 12:
        return [0] * 12
    c = profile["cents"]
    rounded = [round(x - c[ANCHOR]) for x in c]
    assert rounded[ANCHOR] == 0 and all(-128 <= x <= 127 for x in rounded), profile["name"]
    return rounded


def header_text():
    rows = []
    for i, p in enumerate(PROFILES):
        cents = ", ".join(f"{c:3d}" for c in offsets(p))
        rows.append(f"  {{{EDO_INDEX[division(p)]}, {{{cents}}}}},   // {i} {p['name']}")
    return (
        "// Generated by firmware/generator/temperaments.py. Edit that file, not this one.\n"
        "//\n"
        "// One row per temperament, in the order stored in presets (the temperament\n"
        "// parameter). edo_index selects the division of the octave: 0 is twelve notes,\n"
        "// 1, 2 and 3 the 19, 31 and 24 step tables in main.cpp. cents are offsets from equal\n"
        "// temperament for pitch classes C, C#, D ... B, used only when edo_index is 0,\n"
        "// with A at 0 so A sounds at the master tuning pitch.\n"
        "#pragma once\n"
        "#include <stdint.h>\n"
        "\n"
        "struct TemperamentProfile {\n"
        "  uint8_t edo_index;\n"
        "  int8_t cents[12];\n"
        "};\n"
        "\n"
        "const TemperamentProfile temperament_profiles[] = {\n"
        + "\n".join(rows) + "\n"
        "};\n"
        "const uint8_t temperament_count = sizeof(temperament_profiles) / sizeof(temperament_profiles[0]);\n"
    )


def tooltip_text():
    names = ", ".join(f"{i}={p['name']}" for i, p in enumerate(PROFILES))
    return f"tuning of the notes: {names}. {TOOLTIP_TAIL}"


def parameters_text(current):
    line_re = re.compile(r'^(.*"sysex_adress":\s*%d\s*,.*)$' % TEMPERAMENT_ADDRESS, re.M)
    lines = line_re.findall(current)
    assert len(lines) == 1, f"expected one parameter at address {TEMPERAMENT_ADDRESS} in parameters.json"
    line = lines[0]
    new = re.sub(r'"max_value":\s*\d+', f'"max_value":{len(PROFILES) - 1}', line, count=1)
    new = re.sub(r'"tooltip":"(?:[^"\\]|\\.)*"',
                 lambda m: '"tooltip":' + json.dumps(tooltip_text(), ensure_ascii=False), new, count=1)
    return current.replace(line, new)


def main():
    sys.stdout.reconfigure(encoding="utf-8")
    check = "--check" in sys.argv
    stale = []
    header = header_text()
    old = open(HEADER, encoding="utf-8").read() if os.path.exists(HEADER) else None
    if old != header:
        stale.append("include/temperament_profiles.h")
        if not check:
            open(HEADER, "w", encoding="utf-8").write(header)
    params = open(PARAMETERS, encoding="utf-8").read()
    new_params = parameters_text(params)
    json.loads(new_params)
    if new_params != params:
        stale.append("generator/parameters.json")
        if not check:
            open(PARAMETERS, "w", encoding="utf-8").write(new_params)
    if check:
        if stale:
            print("out of date: " + ", ".join(stale) + " (run python3 generator/temperaments.py)")
            sys.exit(1)
        print(f"temperaments: {len(PROFILES)} temperaments, generated files up to date")
        return
    print(("updated: " + ", ".join(stale)) if stale else "already up to date")
    if stale and "generator/parameters.json" in stale:
        print("parameters.json changed: run generate.py to regenerate the handler and minicontrol")
    print(f"\nEditor control for address {TEMPERAMENT_ADDRESS}: max {len(PROFILES) - 1}")
    print("options: " + json.dumps([p["label"] for p in PROFILES], ensure_ascii=False))
    print("optionNotes:")
    for p in PROFILES:
        print("  " + json.dumps(p["note"], ensure_ascii=False) + ",")


if __name__ == "__main__":
    main()
