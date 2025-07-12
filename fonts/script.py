import json
import string
import freetype

def create_font_dictionary(font_path, font_size=16, output_file="font_data.json"):

    # Load the font
    try:
        face = freetype.Face(font_path)
        face.set_char_size(font_size * 64)
    except Exception as e:
        pass

    font_data = {}

    for letter in string.ascii_uppercase:
        face.load_char(letter)

        glyph = face.glyph
        bitmap = glyph.bitmap

        # Skip if no bitmap (shouldn't happen for normal letters)
        if bitmap.width == 0 or bitmap.rows == 0:
            continue

        grid = []
        for y in range(bitmap.rows):
            row = []
            for x in range(bitmap.width):
                pixel_index = y * bitmap.width + x

                if pixel_index < len(bitmap.buffer):
                    pixel_value = bitmap.buffer[pixel_index]
                    row.append(pixel_value > 127)
                else:
                    row.append(False)
            grid.append(row)

        font_data[letter] = grid

    try:
        with open(output_file, 'w') as f:
            json.dump(font_data, f, indent=2)
    except Exception as e:
        return None

    return font_data

create_font_dictionary("font.ttf", 16)