# Inputs Module

This module manages keyboard user input. It is designed to work cross-platform (Windows and Linux/macOS) in a non-blocking asynchronous manner.

## Types

### `KeyCode` (Enum)
The `KeyCode` enum defined in [inputs.h](file:///c:/Users/cleme/CLionProjects/GlyphEngine/src/inputs/inputs.h) defines integer codes corresponding to common keyboard keys.

* **Standard keys (0 to 127)**: Map directly to standard ASCII values (e.g. `KEY_A`, `KEY_Z`, `KEY_SPACE`, `KEY_1`, `KEY_EXCLAMATION`, `KEY_A_UPPER`).
* **Control and navigation keys (> 255)**: Arrow keys (`KEY_UP`, `KEY_DOWN`, `KEY_LEFT`, `KEY_RIGHT`), navigation (`KEY_HOME`, `KEY_END`), and function keys (`KEY_F1` to `KEY_F12`).
* **Special value**: `KEY_UNKNOWN` (-1) is returned when a key cannot be recognized or when no key is pressed.

## Functions

| Name              | Type    | Arguments   | Description                                                                                                                              |
|-------------------|---------|-------------|------------------------------------------------------------------------------------------------------------------------------------------|
| input_get_key     | KeyCode | void        | Reads the standard terminal input buffer non-blockingly. Returns the pressed key as `KeyCode`, or `KEY_UNKNOWN` if no input is buffered. |
| input_is_key_down | int     | KeyCode key | Polls real-time key state. Returns `1` if the specified key is currently held down, `0` otherwise. Recommended for smooth game controls. |
| input_flush       | void    | void        | Flushes any queued keys in the input buffer to prevent accumulated keystrokes.                                                           |