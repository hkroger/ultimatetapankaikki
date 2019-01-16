from PIL import Image
import argparse
import io
import os
import struct


def read_fnt(fp, palette) -> Image:
    """
    Read and rearrange the .fnt file from the stream `fp`
    into a palettized image.

    Since the image can't contain the width/height metadata
    the .fnt file can, it's simply rearranged into a rectangle
    of exactly (16 * (w + 1), 16 * (h + 1)) pixels, which makes
    it possible to reconstruct the original width and height
    of each glyph from the image's size.
    """
    w, h = struct.unpack('<BB', fp.read(2))
    fp.seek(256 * 2)
    glyphs = []
    for i in range(256):
        data = fp.read(w * h)
        assert len(data) == w * h
        glyphs.append(Image.frombytes(mode='P', size=(w, h), data=data))

    w_p = w + 1
    h_p = h + 1
    img = Image.new('P', (16 * w_p, 16 * h_p))
    img.paste(255, (0, 0, img.size[0], img.size[1]))  # Fill with marker color
    
    for i, glyph in enumerate(glyphs):
        y, x = divmod(i, 16)
        img.paste(glyph, (x * w_p, y * h_p))

    if palette:
        img.putpalette(palette)
        
    return img


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('file', nargs='*')
    ap.add_argument('--palette-from')
    args = ap.parse_args()
    
    if args.palette_from:
        palette_img = Image.open(args.palette_from)
        palette = palette_img.getpalette()
    else:
        palette = None

    for filename in args.file:
        with open(filename, 'rb') as infp:
            print(filename, end=' ')
            img = read_fnt(infp, palette=palette)
        out_filename = os.path.splitext(filename)[0] + '.png'
        print('->', out_filename)
        img.save(out_filename)


if __name__ == "__main__":
    main()