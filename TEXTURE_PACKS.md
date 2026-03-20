# 360sweeper — Texture Packs

Info about texturepacks for this game.

---

## How to Load a Texture Pack

1. Launch 360sweeper
2. Go to **Menu → Options → Settings**
3. Navigate to the **second page**
4. In **Texture pack:** select your desired texture pack by going left or right.
5. Press **A** on your pack, then wait for a second and it will be loaded.

---

## Getting Started

Base your texture pack on the default **`Content\`** folder. Your pack should mirror the same file names and subfolder structure. Any file you don't include will break the texture of the element so include all.

---

## Textures

### Understanding the canvas vs the sprite

Every texture in 360sweeper has two things you need to care about:

1. **The PNG canvas** — the full size of the image file (e.g. 64×64)
2. **The sprite** — the actual visible pixel art drawn inside that canvas (e.g. 43×54, sitting in the bottom-right corner)

The empty/transparent space around the sprite is **not padding you can throw away** — the game uses the full canvas size to position the texture on screen. If you crop the PNG down to just the sprite, the texture will appear in the wrong place and the texture will break in game.

**Example:** An original texture has a 64×64 PNG canvas, with a 43×54 sprite drawn in the bottom-right corner. The top-left area is fully transparent. Your replacement must also be a 64×64 PNG (or 128×128 / 32×32 if scaling), with your new sprite drawn in the same bottom-right corner — **not** a 43×54 PNG of just the sprite.

### Size rules

Your replacement PNG must be either **double or half** the original's canvas size. Scale both dimensions equally:

- **Double** — multiply width and height by 2 (e.g. 64×64 → 128×128)
- **Half** — divide width and height by 2 (e.g. 64×64 → 32×32)

You can keep doubling or halving as many times as you want, as long as both dimensions remain whole numbers.

**The sprite inside the canvas must scale by the same factor.** Using the example above — if the canvas goes from 64×64 to 128×128 (doubled), your sprite must go from 43×54 to 86×108 (also doubled), still sitting in the bottom-right corner. The ratio of sprite-to-canvas must stay the same.

> **Never change the aspect ratio.** A 64×64 canvas must stay square. A 128×64 canvas must stay 2:1. Don't stretch or squish either the canvas or the sprite inside it.

### Why?

This is all because the xbox 360 doesn't like resolutions that aren't power of 2 x power of 2 - it does weird stuff if they arent.

### Anchor corner

Most textures have their sprite in the **bottom-right corner** of the PNG canvas, with transparent space filling the top-left area. Your replacement sprite must sit in that same corner.

**Exceptions** — these are anchored differently:
- Arrow textures
- Font character PNGs
- Some border textures

When in doubt, open the original from `Content\` in an image editor and check where the sprite actually sits. Your version must match that same position within the canvas.

---

## Sounds

All sound files must be in **`.wav`** format.

Keep sounds **short** — long audio files will cause issues.

---

## Font

The font is loaded from **individual PNG files**, one per character.

### Generating font PNGs

1. Download the **`.bat`** file and **Python script** from the `font script/` folder in this repo
2. **Drag a `.ttf` font file onto the `.bat`** 
   - Enter the size of the file that each character png will have, the value must be a **power of 2** (e.g. 16, 32, 64)
4. Place the generated PNGs into your pack's **`Font/`** subfolder

### Adjusting font settings — `packprops.txt`

When you load a texture pack in-game, a **`packprops.txt`** file is automatically generated inside your pack's folder. This file controls how the font is displayed on screen. You can edit it manually after it's been created.

| Key | What it does |
|---|---|
| `spacing` | Width of a space character, in screen pixels (out of 1280 horizontal) |
| `letterWidth` | Distance in screen pixels from the left edge of one character to the left edge of the next |
| `fontScale` | Global scale multiplier for all text — increase to make text bigger, decrease to shrink it |
| `bgColor` | Background color of the game, as `R,G,B` values (0–255 each) |

**Example:**
```
spacing=10
letterWidth=20
fontScale=1
bgColor=118,118,118
```

Tweak `spacing` and `letterWidth` to fix gaps or overlapping between characters. `fontScale` is a good first knob to turn if your font looks too small or too large overall.

---

## Submission requirements

Before submitting or sharing your pack, go through these:
If something doesnt work, you have questions feel free to contact me on discord
nickname: rutinoscorbin

- [ ] All pngs are included with the same ratio of width and height as originals and are power of 2s
- [ ] Sprite content is anchored to the correct corner (bottom-right for most, check exceptions)
- [ ] All sounds are `.wav` and kept short
- [ ] Font PNGs are generated correctly
- [ ] `packprops.txt` values have been adjusted to match your font

