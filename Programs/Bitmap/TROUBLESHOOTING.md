# BMP Viewer - Troubleshooting Guide

## Issue: Image still displays incorrectly

### Symptom: White/colored line at top, rest black

**Probable Cause**: BMP orientation still inverted

**Debug Steps**:
```c
// Add to main.c after drawBMP24():
char debugBuf[50];
sprintf(debugBuf, "H:%d W:%d", infoHeader.biHeight, infoHeader.biWidth);
lcdPrintS(debugBuf);
HAL_Delay(3000);
```

**Expected Output**: 
- `H:480 W:640` (positive height = bottom-up)
- Image should NOT be upside-down

**If Inverted**: 
- Check that `isBottomUp` variable is true
- Verify `screenY = height - 1 - rowIdx` calculation is used
- Add print statement: `if (rowIdx == 0) sprintf(...); lcdPrintS(...);`

---

## Issue: "File transfer process already running" error

### Root Cause: Incomplete BMP data consumption

**Symptoms**:
1. First image loads (but maybe incorrectly)
2. Second image fails with error
3. UART seems stuck

**Debug Steps**:
```c
// Add counter in drawBMP24():
int totalBytesRead = 0;
for (int rowIdx = 0; rowIdx < height; rowIdx++) {
    for (int x = 0; x < width; x++) {
        if (COMread((char *)&pixel, sizeof(RGBTRIPLE), 1) != 1) {
            return;
        }
        totalBytesRead += 3;
    }
    for (int i = 0; i < padding; i++) {
        COMread(&dummy, 1, 1);
        totalBytesRead++;
    }
}
// Expected: width * height * 3 + (padding * height)
// For 640x480: 640*480*3 + 0 = 921,600 bytes
```

**Expected Total**: 921,600 bytes
- If less: You're not reading all rows
- If correct: Proceed to check completion signal

**Fix**: Ensure BOTH loops read completely:
```c
// Must have:
for (int rowIdx = 0; rowIdx < height; rowIdx++) {  // ALL rows
    for (int x = 0; x < width; x++) {              // ALL pixels
        COMread(...)
    }
    for (int i = 0; i < padding; i++) {            // ALL padding
        COMread(...)
    }
}
```

---

## Issue: Completion signal not sent

### Symptom: Python GUI waits indefinitely after image

**Debug Steps**:
1. Add print in main.c:
```c
drawBMP24();
lcdPrintS("Sending signal...");
HAL_Delay(1000);
sendCompletionSignal();
lcdPrintS("Signal sent!");
HAL_Delay(1000);
```

2. Verify function exists:
```bash
grep -n "sendCompletionSignal" Programs/Bitmap/Inc/input.h
grep -n "void sendCompletionSignal" Programs/Bitmap/Src/input.c
```

**Expected Output**: Both lines should be found

**If Not Found**: 
- Rebuild: `cbuild --build Bitmap`
- Check files were saved with changes

---

## Issue: Image partially visible / coordinates wrong

### Symptom: Only top part of image visible, or pixels in wrong positions

**Probable Cause**: `GUI_drawPoint()` coordinate mapping issue

**Debug Steps**:
1. Draw test pattern first:
```c
// In main.c, before drawBMP24():
for (int y = 0; y < 240; y++) {
    GUI_drawPoint({0, y}, RED, DOT_PIXEL_1X1, true);
}
// Should draw red line on left edge
HAL_Delay(2000);
GUI_clear(BLACK);
```

2. If red line appears in wrong place:
   - Check `Coordinate` struct definition in LCD_GUI.h
   - Verify: `{x, y}` order (not `{y, x}`)
   - Check LCD resolution: Must be 320×240

3. Add debug output:
```c
// In drawBMP24() inside loop:
if (rowIdx == 0 && x == 0) {
    char dbg[30];
    sprintf(dbg, "Y:%d SY:%d", rowIdx, screenY);
    lcdPrintS(dbg);
    HAL_Delay(500);
}
```

---

## Issue: Build fails with undefined references

### Symptom: 
```
undefined reference to `sendCompletionSignal'
```

**Fix Steps**:
1. Verify include path:
```c
// In main.c, should have:
#include "input.h"
```

2. Verify function declaration in input.h:
```bash
grep "sendCompletionSignal" Programs/Bitmap/Inc/input.h
```

3. Verify function implementation in input.c:
```bash
grep -A2 "void sendCompletionSignal" Programs/Bitmap/Src/input.c
```

4. Clean rebuild:
```bash
cd Programs/Bitmap
rm -rf build/
cbuild --build Bitmap.cproject.yml
```

---

## Issue: Black screen with no image

### Symptom: Screen clears to black but no image appears

**Debug Checklist**:

1. **Is `drawBMP24()` being called?**
```c
// Before drawBMP24(), add:
GUI_drawPoint({10, 10}, WHITE, DOT_PIXEL_1X1, true);
drawBMP24();
// If white dot appears, drawBMP24() is reached
```

2. **Is BMP being read correctly?**
```c
// After COMread in drawBMP24(), add periodically:
if (rowIdx % 48 == 0) {  // Every 48 rows
    uint16_t testColor = convert(255, 0, 0);  // Red
    GUI_drawPoint({rowIdx / 2, 0}, testColor, DOT_PIXEL_1X1, true);
}
// Should see red dots along top if data is flowing
```

3. **Is `convert()` function working?**
```c
// In main.c before drawBMP24():
uint16_t red = convert(255, 0, 0);
uint16_t green = convert(0, 255, 0);
uint16_t blue = convert(0, 0, 255);
GUI_clear(red);    // Screen should be red
HAL_Delay(1000);
GUI_clear(green);  // Should turn green
HAL_Delay(1000);
GUI_clear(blue);   // Should turn blue
HAL_Delay(1000);
GUI_clear(BLACK);
drawBMP24();
```

4. **Is LCD initialized correctly?**
```bash
# Check in init.c or LCD_GUI.c that GUI_init() is called
grep -n "GUI_init" Programs/Bitmap/Src/main.c
# Should see: GUI_init(100);
```

---

## Common Mistakes Checklist

- ❌ `writeChar()` called instead of `sendCompletionSignal()` (writeChar is static!)
- ❌ `height - 1 - rowIdx` not used for Y-coordinate
- ❌ Padding not read in all cases
- ❌ Loop condition is `y < maxHeight` instead of `y < height`
- ❌ Forgot to include `#include "input.h"` in main.c
- ❌ `GUI_drawPoint()` called with wrong coordinate order
- ❌ Screen larger than 320×240 assumed (LCD is fixed size!)
- ❌ BMP header not checked for validity (biHeight must be != 0)

---

## Python GUI Debugging

### If Python GUI shows errors:

1. **"Connection refused"**
   - STM32 board not powered on
   - COM port incorrect in Python settings
   - UART cable not connected

2. **"File transfer process is already running"**
   - STM32 didn't send completion signal (0x00 byte)
   - `sendCompletionSignal()` not being called
   - Previous transfer incomplete (UART not synced)

3. **"Invalid BMP file received"**
   - Python received corrupted data
   - UART sync lost (bytes shifted)
   - BMP file < 1054 bytes (smaller than headers)

4. **Python receives 0x00 but doesn't recognize it**
   - Add logging in Python GUI to show received bytes:
   ```python
   data = serial_port.read()
   print(f"Received byte: {data[0]:02x}")  # Should show "00"
   ```

---

## Performance Notes

- **Image render time**: ~200-500ms for 320×240 pixels (depends on LCD_GUI implementation)
- **UART transfer time**: ~2-5 seconds for 640×480 BMP at 115200 baud
- **Total per image**: ~5-10 seconds (including protocol handshake)

If taking much longer:
- Check UART baud rate (should be 115200)
- Check if LCD_drawPoint() is blocking (should be fast)
- Profile code to find bottleneck

---

## Success Indicators

✅ **Working correctly when**:
1. Image displays with correct orientation (not upside-down)
2. No white line artifact at top
3. Python GUI doesn't error after 1st image
4. Can load 2+ images without "transfer running" error
5. LCD shows image within 1-2 seconds of file selection
6. No UART sync errors in Python serial output

---

## Next Steps if Still Issues

1. Enable UART debugging: Add `printf()` statements
2. Use logic analyzer to capture UART protocol
3. Compare received bytes with expected BMP file structure
4. Add CRC checksum to verify data integrity
5. Implement timeout detection for protocol errors
