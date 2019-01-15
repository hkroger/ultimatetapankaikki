from PIL import Image
import argparse
import io
import os
import struct


def read_efp(fp) -> Image:
    header = fp.read(6)
    assert header == b'EF pic'
    w, h = struct.unpack('<HH', fp.read(4))
    rle_stream = fp
    decoded_stream = io.BytesIO()
    while decoded_stream.tell() < w * h:
        buf = rle_stream.read(1)
        if not buf:
            break
        val, = struct.unpack('<B', buf)
        
        if val > 192:
            color_tup = struct.unpack('<B', rle_stream.read(1))
            decoded_stream.write(bytes(color_tup) * (val - 192))
        else:
            decoded_stream.write(bytes((val,)))
    assert decoded_stream.tell() == w * h
    img = Image.frombytes(mode='P', size=(w, h), data=decoded_stream.getvalue())

    fp.seek(-768, 2)
    palette_data = fp.read(768)
    assert len(palette_data) == 768
    palette_data = [c << 2 for c in palette_data]  # rescale VGA brightness 0..63 to 0..255
    img.putpalette(palette_data)

    return img
    


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('file', nargs='*')
    args = ap.parse_args()
    for filename in args.file:
        with open(filename, 'rb') as infp:
            print(filename, end=' ')
            img = read_efp(infp)
        out_filename = os.path.splitext(filename)[0] + '.png'
        print('->', out_filename)
        img.save(out_filename)


if __name__ == "__main__":
    main()