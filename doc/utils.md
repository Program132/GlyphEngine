# Utils Module

This module provides utility functions for terminal display and console management.

# Functions

| Name               | Type | Arguments | Description                                                                                          |
|--------------------|------|-----------|------------------------------------------------------------------------------------------------------|
| clearConsoleScreen | void | void      | Resets the cursor position to the top-left corner of the terminal using ANSI escape codes (`\033[H`).  |
| hideCursor         | void | void      | Hides the terminal text cursor (`\033[?25l`) for clean character-based graphic rendering.            |
| showCursor         | void | void      | Restores the terminal text cursor (`\033[?25h`) when stopping or exiting the engine.                |
