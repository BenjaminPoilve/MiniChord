# USB audio, a choice: the minichord's USB audio setting (address 244, src/main.cpp) decides whether
# the phone or computer it's plugged into is offered a USB speaker. The Teensy core's MIDI16_AUDIO_SERIAL
# type declares one in a fixed table, so this builds a copy of the core's usb_desc.c with both
# arrangements in it:
#   - the core's own descriptors, unchanged, now in RAM: what the host is told unless asked otherwise
#   - the same without the speaker: the AudioControl interface keeps its USB-streaming output (the
#     minichord's sound, recorded over USB) and loses the speaker's input terminal, volume feature
#     unit and output terminal, and the speaker's streaming interface goes
# and usb_desc_pick(speaker), which main.cpp's startup_middle_hook calls before USB starts, copies the
# speakerless one in when it's wanted. And a copy of the core's usb.c that sends a configuration
# descriptor as long as its own wTotalLength says (the core sends the table's whole length, which the
# speakerless one is shorter than). The core's own files are left as they are (they're shared by every
# project on the machine): the copies are made in the build directory and compiled in their place. A
# core whose files don't read as expected stops the build rather than being half patched.
import os, re
Import("env")

CORE = os.path.join(env.PioPlatform().get_package_dir("framework-arduinoteensy"), "cores", "teensy4")
BUILD = env.subst("$BUILD_DIR")
# what the speaker takes: a byte of the AC header's interface list, its input terminal (12), feature
# unit (10) and output terminal (9), and its streaming interface (9+9+7+11+9+7) with the sync endpoint (9)
SPEAKER_BYTES = "(1 + 12+10+9 + 9+9+7+11+9+7+9)"


def fail(path, why):
    raise SystemExit(f"usb_audio_choice.py: {why} in {path}; the Teensy core has changed, so check the patch")


def write(name, text):
    path = os.path.join(BUILD, name)
    os.makedirs(BUILD, exist_ok=True)
    if not os.path.exists(path) or open(path).read() != text:
        open(path, "w").write(text)
    return path


def without_speaker(block, path):
    """one speed's audio function, the speaker taken out"""
    def sub(pattern, repl, what):
        nonlocal block
        block, n = re.subn(pattern, repl, block, count=1)
        if n != 1:
            fail(path, f"no {what}")
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
        fail(path, "second input terminal")
    line_start = block.rfind("\n", 0, second_it) + 1
    rest = block[line_start:]
    as_at = rest.find("// Standard AS Interface Descriptor")
    if as_at < 0 or "0x31" not in rest[:as_at]:
        fail(path, "speaker terminals before the first streaming interface")
    block = block[:line_start] + rest[rest.rfind("\n", 0, as_at) + 1:]
    # the speaker's streaming interface, to the end of the function
    first_as = block.find("// Standard AS Interface Descriptor")
    second_as = block.find("// Standard AS Interface Descriptor", first_as + 1)
    if second_as < 0 or "AUDIO_INTERFACE+2" not in block[second_as:] or "AUDIO_SYNC_ENDPOINT" not in block[second_as:]:
        fail(path, "speaker streaming interface")
    block = block[:block.rfind("\n", 0, second_as) + 1]
    if "AUDIO_INTERFACE+2" in block or "AUDIO_RX_ENDPOINT" in block:
        fail(path, "speaker left over after the cut")
    return block


def patch_usb_desc():
    path = os.path.join(CORE, "usb_desc.c")
    src = open(path).read()
    for speed in ("480", "12"):
        head = f"PROGMEM const uint8_t usb_config_descriptor_{speed}[CONFIG_DESC_SIZE] = {{\n"
        start = src.find(head)
        end = src.find("\n};\n", start)
        if start < 0 or end < 0:
            fail(path, f"usb_config_descriptor_{speed}")
        table = src[start + len(head):end + 4]
        # the speakerless table: its own length and one interface fewer, and its audio function cut
        m = re.search(r"#ifdef AUDIO_INTERFACE\n\t// configuration for [^\n]*\n", table)
        stop = table.find("\n#endif", m.end()) if m else -1
        if not m or stop < 0:
            fail(path, f"the audio function in usb_config_descriptor_{speed}")
        bare = table[:m.end()] + without_speaker(table[m.end():stop], path) + table[stop:]
        for old, new, what in (
            ("LSB(CONFIG_DESC_SIZE),                 // wTotalLength\n        MSB(CONFIG_DESC_SIZE),",
             "LSB(USB_NOSPEAKER_DESC_SIZE),          // wTotalLength\n        MSB(USB_NOSPEAKER_DESC_SIZE),", "wTotalLength"),
            ("NUM_INTERFACE,                          // bNumInterfaces",
             "NUM_INTERFACE-1,                        // bNumInterfaces", "bNumInterfaces")):
            if bare.count(old) != 1:
                fail(path, f"{what} in usb_config_descriptor_{speed}")
            bare = bare.replace(old, new)
        both = (f"// usb_audio_choice.py: in RAM, so usb_desc_pick can put the speakerless one in its place\n"
                f"uint8_t usb_config_descriptor_{speed}[CONFIG_DESC_SIZE] = {{\n" + table +
                f"\nstatic PROGMEM const uint8_t usb_config_descriptor_{speed}_nospeaker[USB_NOSPEAKER_DESC_SIZE] = {{\n" + bare +
                f"_Static_assert(sizeof(usb_config_descriptor_{speed}_nospeaker) == USB_NOSPEAKER_DESC_SIZE, \"the speakerless table's length\");\n")
        src = src[:start] + both + src[end + 4:]
    first = src.find("// usb_audio_choice.py: in RAM")
    src = (src[:first] + "#include <string.h>\n#define USB_NOSPEAKER_DESC_SIZE (CONFIG_DESC_SIZE - " + SPEAKER_BYTES + ")\n\n" + src[first:] +
           "\n\n// usb_audio_choice.py: the host offered a USB speaker or not, chosen before USB starts\n"
           "void usb_desc_pick(int speaker)\n{\n\tif (speaker) return;\n"
           "\tmemcpy(usb_config_descriptor_480, usb_config_descriptor_480_nospeaker, USB_NOSPEAKER_DESC_SIZE);\n"
           "\tmemset(usb_config_descriptor_480 + USB_NOSPEAKER_DESC_SIZE, 0, CONFIG_DESC_SIZE - USB_NOSPEAKER_DESC_SIZE);\n"
           "\tmemcpy(usb_config_descriptor_12, usb_config_descriptor_12_nospeaker, USB_NOSPEAKER_DESC_SIZE);\n"
           "\tmemset(usb_config_descriptor_12 + USB_NOSPEAKER_DESC_SIZE, 0, CONFIG_DESC_SIZE - USB_NOSPEAKER_DESC_SIZE);\n}\n")
    return write("usb_desc_choice.c", src)


def patch_usb():
    path = os.path.join(CORE, "usb.c")
    src = open(path).read()
    for old, new, what in (
        ("extern const uint8_t usb_config_descriptor_480[];\nextern const uint8_t usb_config_descriptor_12[];",
         "extern uint8_t usb_config_descriptor_480[];   // usb_audio_choice.py: in RAM\nextern uint8_t usb_config_descriptor_12[];", "the descriptors' declarations"),
        ("\t\t\t\t\tdatalen = list->length;\n",
         "\t\t\t\t\tdatalen = list->length;\n"
         "\t\t\t\t\t// usb_audio_choice.py: a configuration as long as it says it is\n"
         "\t\t\t\t\tif ((setup.wValue >> 8) == 2 || (setup.wValue >> 8) == 7) datalen = list->addr[2] | (list->addr[3] << 8);\n",
         "the descriptor length")):
        if src.count(old) != 1:
            fail(path, what)
        src = src.replace(old, new)
    return write("usb_choice.c", src)


DESC, USB = patch_usb_desc(), patch_usb()
env.AddBuildMiddleware(lambda env, node: env.Object(os.path.join("$BUILD_DIR", "usb_desc_choice.o"), DESC), "*cores/teensy4/usb_desc.c")
env.AddBuildMiddleware(lambda env, node: env.Object(os.path.join("$BUILD_DIR", "usb_choice.o"), USB), "*cores/teensy4/usb.c")
