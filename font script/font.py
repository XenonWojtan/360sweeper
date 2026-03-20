import os
from PIL import Image, ImageDraw, ImageFont

import sys

# --- Settings ---
if len(sys.argv) > 1:
    FONT_PATH = sys.argv[1]
    print(f"Using font: {FONT_PATH}")
else:
    FONT_PATH = input("Enter TTF filename (e.g. PressStart2P-Regular.ttf): ").strip()
VALID_SIZES = [2 ** i for i in range(1, 12)]  # 2, 4, 8, ..., 2048
while True:
    try:
        size_input = int(input(f"Enter image size (power of 2, 2–2048): ").strip())
        if size_input in VALID_SIZES:
            IMAGE_SIZE = (size_input, size_input)
            break
        else:
            print(f"Invalid size. Choose from: {VALID_SIZES}")
    except ValueError:
        print("Please enter a whole number.")
FONT_SIZE = int(IMAGE_SIZE[0] * (80 / 128))
font_name = os.path.splitext(os.path.basename(FONT_PATH))[0]
OUTPUT_DIR = f"{font_name}_exported"

symbol_names = {
    '!': 'exclamation', '#': 'hash', '$': 'dollar', '%': 'percent',
    '&': 'ampersand', '(': 'paren_open', ')': 'paren_close', '*': 'asterisk',
    '+': 'plus', ',': 'comma', '-': 'minus', '.': 'period', '/': 'slash',
    ':': 'colon', ';': 'semicolon', '<': 'less_than', '=': 'equals',
    '>': 'greater_than', '?': 'question', '@': 'at', '[': 'bracket_open',
    '\\': 'backslash', ']': 'bracket_close', '^': 'caret', '_': 'underscore',
    '{': 'brace_open', '|': 'pipe', '}': 'brace_close', '~': 'tilde',
    ' ': 'space', '"': 'quote', "'": 'apostrophe'
}

if not os.path.exists(OUTPUT_DIR):
    os.makedirs(OUTPUT_DIR)

chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{}|;:,.<>?/\\"

try:
    font = ImageFont.truetype(FONT_PATH, FONT_SIZE)
except IOError:
    print(f"Error: Could not find '{FONT_PATH}'")
    exit()

for char in chars:
    img = Image.new("RGBA", IMAGE_SIZE, (255, 255, 255, 0))
    draw = ImageDraw.Draw(img)
    draw.text((IMAGE_SIZE[0]/2, IMAGE_SIZE[1]/2), char, font=font, fill="black", anchor="mm")

    if char.isalnum():
        name = f"upper_{char}" if char.isupper() else char
    else:
        name = symbol_names.get(char, str(ord(char)))

    img.save(os.path.join(OUTPUT_DIR, f"{name}.png"))

print(f"Finished! Check the '{OUTPUT_DIR}' folder.")