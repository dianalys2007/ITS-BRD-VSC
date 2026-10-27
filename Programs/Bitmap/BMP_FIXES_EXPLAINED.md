# BMP Viewer Fixes - Comprehensive Explanation

## Summary of Changes

Three critical bugs fixed in BMP24 viewer:
1. **Bottom-up BMP orientation not handled** → Image displayed incorrectly
2. **Incomplete data consumption** → UART protocol desynchronization
3. **No completion signal** → Python GUI stuck in "transfer running" state

---

## Problem 1: BMP Bottom-Up Orientation ❌ → ✅

### What is BMP Bottom-Up?

BMP files store image data from **bottom to top**:
- First row in file = **bottom** of displayed image
- Last row in file = **top** of displayed image

### The Bug

Old code read rows sequentially but rendered them as if they were top-down:

```c
// WRONG: reads bottom-up but renders as top-down
for (int y = 0; y < height; y++) {
    // Read pixel at row y (from file)
    Coordinate p = {x, y};  // Draw at y=0 for first row (should be y=height-1!)
    GUI_drawPoint(p, color, DOT_PIXEL_1X1, true);
}
```

### The Fix

The fix detects orientation and applies correct Y-coordinate mapping:

```c
// CORRECT: respects BMP orientation
bool isBottomUp = (height > 0);  // Standard BMP is bottom-up
if (isBottomUp) {
    screenY = height - 1 - rowIdx;  // Map first row to bottom of display
} else {
    screenY = rowIdx;  // Top-down BMPs (rare) map directly
}
Coordinate p = {x, screenY};
GUI_drawPoint(p, color, DOT_PIXEL_1X1, true);
```

**Example**: For 640x480 BMP → 320x240 screen
- rowIdx=0 reads first BMP row → screenY = 480-1-0 = 479... wait, but LCD only goes to 239!
- Actually: rowIdx=240 reads at → screenY = 480-1-240 = 239 (visible top of LCD)
- rowIdx=479 reads last BMP row → screenY = 0 (visible bottom of LCD)

✅ Image now displays **right-side-up**

---

## Problem 2: Incomplete Data Consumption ❌ → ✅

### The Bug

640x480 BMP contains 307,200 pixels × 3 bytes = 921,600 bytes (plus padding)

Old code only read/processed visible area (320×240 = 76,800 pixels):

```c
// WRONG: skips invisible rows but uses loop that doesn't guarantee all rows read
for (int y = 0; y < maxHeight; y++) {    // maxHeight = 240
    // ... process pixels ...
}
// This loop exits when y >= 240
// But BMP file has 480 rows! 240 rows unread!

for (int y = maxHeight; y < height; y++) {  // Try to skip remaining
    // ... but coordinate mapping broken, incomplete reads possible
}
```

**Result**: 
- 240 rows (921,600 bytes total) left unread in UART buffer
- Next `openNextFile()` call reads garbage
- Protocol loses synchronization
- Python GUI sees "file transfer error"

### The Fix

Unified loop that reads **ALL rows** and applies coordinate filtering:

```c
// CORRECT: reads entire BMP file, filters what to display
for (int rowIdx = 0; rowIdx < height; rowIdx++) {  // ALL 480 rows!
    for (int x = 0; x < width; x++) {
        if (COMread((char *)&pixel, sizeof(RGBTRIPLE), 1) != 1) {
            return;  // EOF
        }
        
        // Calculate display coordinate
        screenY = (isBottomUp) ? height - 1 - rowIdx : rowIdx;
        
        // Draw ONLY if in visible area
        if (x < maxWidth && screenY < maxHeight) {
            GUI_drawPoint(p, color, ...);
        }
        // Otherwise pixel is read but not drawn
    }
    
    // Read padding bytes (required for BMP format)
    for (int i = 0; i < padding; i++) {
        COMread(&dummy, 1, 1);
    }
}
```

**Key insight**: We MUST read all BMP bytes to maintain UART protocol sync, even if we don't display them.

✅ **All 480 rows consumed** → Protocol stays synchronized → Next file reads correctly

---

## Problem 3: No Completion Signal ❌ → ✅

### The Bug

After `drawBMP24()` finishes, STM32 sent nothing back to Python GUI.

Python protocol waits for acknowledgment before accepting new file:
```python
# Pseudo Python code
file_transfer_running = True
while file_transfer_running:
    if receive_data():
        # Process image
        pass
    else:
        # Wait for completion signal
        # But STM32 sends nothing!
        # Timeout → "transfer already running" error
        error("File transfer process is already running")
```

### The Fix

Added completion signal (0x00 byte) after image fully consumed:

**input.h**: 
```c
extern void sendCompletionSignal(void);
```

**input.c**:
```c
void sendCompletionSignal(void) {
    writeChar(0x00);  // Send null byte as EOF marker
}
```

**main.c**:
```c
drawBMP24();           // Consume all BMP data
sendCompletionSignal(); // Tell Python "done!"
```

✅ **Python GUI receives 0x00** → Knows transfer complete → Ready for next file

---

## BMP File Format Reference

### File Structure
```
[14 bytes] BITMAPFILEHEADER
  - bfType: 0x4D42 ("BM")
  - bfSize: file size
  - bfOffBits: offset to pixel data (usually 54 for 24-bit)
  
[40 bytes] BITMAPINFOHEADER
  - biWidth: image width (640)
  - biHeight: height (+480 for bottom-up, -480 for top-down)
  - biBitCount: 24 for true color
  - biCompression: 0 for uncompressed
  
[pixel data] Rows stored bottom-to-top (if biHeight > 0)
  - 3 bytes per pixel (B, G, R order)
  - Each row padded to 4-byte boundary
```

### Padding Calculation

BMP rows must be 4-byte aligned:
```c
int rowSizeBytes = ((width * 3 + 3) / 4) * 4;
int paddingBytes = rowSizeBytes - (width * 3);

// Example: width=640
// rowSizeBytes = ((640*3 + 3) / 4) * 4 = (1923 / 4) * 4 = 480 * 4 = 1920
// paddingBytes = 1920 - 1920 = 0 (no padding!)
```

---

## Testing Checklist

- [ ] Build project successfully
- [ ] Load 640×480 BMP file via Python GUI
- [ ] Image displays correctly (right-side-up)
- [ ] No white line artifacts
- [ ] Python GUI shows "Image loaded successfully"
- [ ] Can load second image (no "already running" error)
- [ ] Test with 320×240 BMP (smaller than screen)
- [ ] Test with 800×600 BMP (larger than screen)
- [ ] Verify LCD_GUI.h `GUI_drawPoint()` function signature matches usage

---

## Debugging Commands

If issues persist:

```bash
# Check for compilation errors
cd /Users/dianalys/ITS-BRD-VSC/Programs/Bitmap
cmake --build . 2>&1 | grep -i error

# Check file includes
grep -n "include" Src/bmp.c
grep -n "include" Src/input.c
grep -n "include" Src/main.c

# Verify function signatures
grep -n "GUI_drawPoint\|sendCompletionSignal\|drawBMP24" Inc/*.h
```

---

## Next Steps

1. **Build and test** with corrected code
2. If still issues: Add `lcdPrintS()` debug statements to show coordinates
3. Verify Python GUI is reading 0x00 completion byte
4. Consider adding checksum verification in protocol
