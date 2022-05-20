#!/usr/bin/env python3

import argparse
from PIL import Image, ImageSequence
parser = argparse.ArgumentParser(
    description='Converts GIFs to RGB332 for playback on microcontrollers')
parser.add_argument('input_file', type=str,
                    help='filename of the GIF to convert')
parser.add_argument(
    '-p', '--progmem', help='output a C-style array instead of a .bin file', action="store_true")
parser.add_argument(
    '-r', '--rgb565', help='select RGB565 instead of RGB332', action="store_true")
args = parser.parse_args()

gif = Image.open(args.input_file)
width, height = gif.size
frames = 0
rgbdata = bytearray()

for frame in ImageSequence.Iterator(gif):
    frames += 1
    for pixel in frame.getdata():
        if frame.mode == 'RGB':
            (r, g, b) = pixel
        else:
            r = frame.palette.palette[pixel * 3 + 0]
            g = frame.palette.palette[pixel * 3 + 1]
            b = frame.palette.palette[pixel * 3 + 2]
        if args.rgb565:
            rgb565 = (r & 0xf8) | ((g & 0xe0) >> 5)
            rgbdata.append(rgb565)
            rgb565 = ((g & 0x1c) << 3) | ((b & 0xf8) >> 3)
            rgbdata.append(rgb565)
        else:
            rgbdata.append((r & 0xe0) | ((g & 0xe0) >> 3) | ((b & 0xc0) >> 6))

if args.progmem:
    filetext = ('PROGMEM const byte Data[] { ' + str(width) +
                ', ' + str(height) + ', ' + str(frames) + ',')
    modulus = width * height
    if args.rgb565:
        modulus *= 2
    for i, byte in enumerate(rgbdata):
        if i % modulus == 0:
            filetext += ('\n  // frame ' + str(i // modulus) + '\n  ')
        elif i % width == 0:
            filetext += ('\n  ')
        filetext += ('0x%02x, ' % (byte))
    filetext += ("\n};\n")
    with open(args.input_file + '.h', 'w') as output:
        output.write(filetext)

else:
    if args.rgb565:
        with open(args.input_file + '.565', 'wb') as output:
            header = bytearray()
            header.append(width)
            header.append(height)
            header.append(frames)
            output.write(header)
            output.write(rgbdata)
    else:
        with open(args.input_file + '.332', 'wb') as output:
            header = bytearray()
            header.append(width)
            header.append(height)
            header.append(frames)
            output.write(header)
            output.write(rgbdata)

print('saved %d frames' % (frames))
