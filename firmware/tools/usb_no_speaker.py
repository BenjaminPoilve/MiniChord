# No USB speaker. The minichord sends its sound to the phone or computer it's plugged into, for
# recording (USB_out in audio_definition.h). The Teensy core's MIDI16_AUDIO_SERIAL type that gives it
# that also declares a USB speaker, and nothing has ever played what's sent there: a phone sends all
# its sound to it the moment the minichord is plugged in, and goes quiet. The core declares both
# directions in a fixed table in usb_desc.c, so this compiles a copy of that file without the
# host-to-device half: the AudioControl interface keeps its USB-streaming output (the recording) and
# loses the speaker's input terminal, volume feature unit and output terminal, and the speaker's
# streaming interface goes. The core's own file is left as it is (it's shared by every project on
# the machine); the copy is made in the build directory and compiled in its place. A core whose
# table doesn't read as expected stops the build rather than being half patched.
import os, re
Import("env")

CORE_DESC = os.path.join(env.PioPlatform().get_package_dir("framework-arduinoteensy"), "cores", "teensy4", "usb_desc.c")
PATCHED = os.path.join(env.subst("$BUILD_DIR"), "usb_desc_no_speaker.c")


def fail(why):
    raise SystemExit(f"usb_no_speaker.py: {why} in {CORE_DESC}; the Teensy core has changed, so check the patch")


def without_speaker(block):
    """one speed's audio function, the speaker taken out"""
    def sub(pattern, repl, what):
        nonlocal block
        block, n = re.subn(pattern, repl, block, count=1)
        if n != 1:
            fail(f"no {what}")
    # the interface association: control and the recording's streaming, two interfaces, not three
    sub(r"\n(\s*)3,(\s*// bInterfaceCount)", r"\n\g<1>2,\g<2>", "bInterfaceCount 3")
    # the class-specific header: one streaming interface in the collection, so a byte shorter, and the
    # descriptors that follow it (header 9, input terminal 12, output terminal 9) total 30
    sub(r"\n(\s*)10,(\s*// bLength\n\s*0x24,\s*// bDescriptorType, 0x24 = CS_INTERFACE\n\s*0x01,\s*// bDescriptorSubtype, 1 = HEADER)",
        r"\n\g<1>9,\g<2>", "AC header length 10")
    sub(r"LSB\(62\), MSB\(62\)", "LSB(30), MSB(30)", "AC wTotalLength 62")
    sub(r"\n(\s*)2,(\s*// bInCollection)", r"\n\g<1>1,\g<2>", "bInCollection 2")
    sub(r"\n\s*AUDIO_INTERFACE\+2,\s*// baInterfaceNr\(2\)[^\n]*", "", "baInterfaceNr(2)")
    # the speaker's terminals and volume: from the second input terminal to the first streaming interface
    first_it = block.find("// Input Terminal Descriptor")
    second_it = block.find("// Input Terminal Descriptor", first_it + 1)
    if first_it < 0 or second_it < 0:
        fail("second input terminal")
    line_start = block.rfind("\n", 0, second_it) + 1
    rest = block[line_start:]
    as_at = rest.find("// Standard AS Interface Descriptor")
    if as_at < 0 or "0x31" not in rest[:as_at]:
        fail("speaker terminals before the first streaming interface")
    block = block[:line_start] + rest[rest.rfind("\n", 0, as_at) + 1:]
    # the speaker's streaming interface, to the end of the function
    first_as = block.find("// Standard AS Interface Descriptor")
    second_as = block.find("// Standard AS Interface Descriptor", first_as + 1)
    if second_as < 0 or "AUDIO_INTERFACE+2" not in block[second_as:] or "AUDIO_SYNC_ENDPOINT" not in block[second_as:]:
        fail("speaker streaming interface")
    block = block[:block.rfind("\n", 0, second_as) + 1]
    if "AUDIO_INTERFACE+2" in block or "AUDIO_RX_ENDPOINT" in block:
        fail("speaker left over after the cut")
    return block


def patch():
    src = open(CORE_DESC).read()
    # each speed's audio function: "#ifdef AUDIO_INTERFACE" then "// configuration for", to its "#endif"
    pieces, at, found = [], 0, 0
    for m in re.finditer(r"#ifdef AUDIO_INTERFACE\n\t// configuration for [^\n]*\n", src):
        end = src.find("\n#endif", m.end())
        if end < 0:
            fail("end of an audio function")
        pieces += [src[at:m.end()], without_speaker(src[m.end():end])]
        at = end
        found += 1
    if found != 2:
        fail(f"{found} audio functions (expected the high-speed and the full-speed one)")
    src = "".join(pieces) + src[at:]
    # the descriptors' length, which the configuration's wTotalLength and the descriptor list both take
    old_size = "#define AUDIO_INTERFACE_DESC_SIZE\t8 + 9+10+12+9+12+10+9 + 9+9+7+11+9+7 + 9+9+7+11+9+7+9"
    if old_size not in src:
        fail("AUDIO_INTERFACE_DESC_SIZE")
    src = src.replace(old_size, "#define AUDIO_INTERFACE_DESC_SIZE\t8 + 9+9+12+9 + 9+9+7+11+9+7")
    # one interface fewer: the audio function is the last, so the count is its two after it starts
    inc = '#include "usb_desc.h"\n'
    if inc not in src:
        fail("usb_desc.h include")
    src = src.replace(inc, inc + "#undef NUM_INTERFACE\n#define NUM_INTERFACE (AUDIO_INTERFACE+2)   // usb_no_speaker.py: no speaker interface\n", 1)
    os.makedirs(os.path.dirname(PATCHED), exist_ok=True)
    if not os.path.exists(PATCHED) or open(PATCHED).read() != src:
        open(PATCHED, "w").write(src)


patch()
env.AddBuildMiddleware(lambda env, node: env.Object(os.path.join("$BUILD_DIR", "usb_desc_no_speaker.o"), PATCHED), "*cores/teensy4/usb_desc.c")
