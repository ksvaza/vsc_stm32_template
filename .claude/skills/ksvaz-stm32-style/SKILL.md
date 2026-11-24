---
name: ksvaz-stm32-style
description: Write, refactor or review STM32 / embedded C for ksvaz (Kārlis) in his own style: Latvian module names and comments, Module_Verb functions, Timer_t main loops, tabs + CRLF.
---

# ksvaz (Kārlis) STM32 C style

Derived from his hand-written modules in Jaunaizmugure, Prieksa and Stuure (41 files, Apr-Jul 2026, STM32F1/F4, STM32CubeIDE + CubeMX HAL). Stuure is the newest and is the reference when the projects disagree. Code that looks pasted in (4-space indent, K&R braces, "Adjust pins as per your PCB" comments, for example `paterins.c` and Kuili `main.c`) is NOT his style; don't copy it.

## Analysis method (for updating this skill from new code)
1. Skip generated code: `Drivers/`, `Middlewares/`, `FATFS/`, `.metadata/`, and CubeMX files (`main.c` outside USER CODE, `*_it.c`, `*_hal_msp.c`, `*_hal_conf.h`, `system_*.c`, `syscalls.c`, `sysmem.c`).
2. Read his own modules (Latvian-named `.c/.h`) plus the USER CODE sections of each `main.c`.
3. Check indentation, line endings and encoding with tools (`cat -A`, `grep -c $'\r'`, `file -i`), not by eye.
4. Prefer patterns that hold across several projects; when projects disagree, the newest one wins.

## Files
- One module = `<nosaukums>.h` + `<nosaukums>.c`. The name is a lowercase Latvian noun, ASCII only, long vowels written doubled: `gaismas`, `kans` (CAN), `kludas` (errors), `taimeris`, `pedali`, `taure`, `ventilators`, `ekraans`, `akselerometrs`, `sd_karte`, `paterins`, `parveidotajs`, `motors`, `blink`. English names are fine for generic infrastructure (`logging`, `blink`, `app`).
- In CubeIDE projects, modules go in `Core/Inc` + `Core/Src`. In the VS Code template, they go in `App/Inc` + `App/Src`, and each `.c` is added to `target_sources` in `CMakeLists.txt`.
- Header block (CubeIDE format, exactly this spacing; date as `D Mon YYYY`):
```c
/*
 * kludas.h
 *
 *  Created on: 15 Jul 2026
 *      Author: ksvaz
 */
```
- Include guard `INC_<NAME>_H_` with closing comment: `#endif /* INC_KLUDAS_H_ */`.
- Header order: header block, guard, blank lines, `<stdint.h>` / std includes, blank, HAL include (`"stm32f1xx_hal.h"`) and project includes, then defines, types, externs and prototypes.
- In `.c` files: own header first, then `main.h` and other modules.
- **Tabs** for indentation (tab width 4), **CRLF** line endings, UTF-8 (Latvian diacritics in comments and strings are fine).

## Formatting
- Allman braces for functions, `if`, `for`, `while`, `switch`, `typedef struct`/`enum`.
- Short guard clauses on one line, without braces: `if (!blink) return;`, `if (value > 1.0f) value = 1.0f;`
- `switch` with `case` at the same indent as `switch`.
- Section separators in long files: `// ----------------------------------------------------------------`, or titled `// ------ Buffered Data logging ------ //`.
- Align `=` in enum/register tables with spaces: `LOGGING_LEVEL_ERROR   = 0x01U,`.
- Struct literals: `(Logging_Instance) { .name = "main", .loglevel = LOGGING_LEVEL_DEBUG }`, with fields one or two per line.
- Blank lines: 1 between functions (sometimes 0); 2-3 around include and prototype groups in headers.

## Naming
| Thing | Style | Examples |
|---|---|---|
| Public functions, newer code | `Module_Verb` / `MODULE_Verb` | `Blink_Setup`, `Lights_Brake`, `Logging_Start`, `KLUDA_InitCAN`, `CAN_SendFrame`, `App_Init` |
| Older / utility functions | `VerbNoun` | `CheckTimer`, `InitTimer`, `ReadTemperature`, `SetMotorDirection` |
| Peripheral handle setters | `Set<Thing>_Handler(X_HandleTypeDef* h)` | `SetCAN_Handler(&hcan)`, `SetINA229_Handler(&hspi1)`, `SetD_LEDPWM_Handler(&htim8)` |
| Chip-specific code | chip part number as prefix | `INA229_GetCurrent`, `MCP4725_SetValue` |
| Stored handle pointer | PascalCase / `Module_Handler` | `CAN_Handler`, `TempAnalogPort`, `D_LEDPWM_TimerHandle` |
| Module-level state | CAPS | `BLINK_BUFFER_I`, `INA229_EXISTS`, `GAS_UPDATE_TIMER`, `KLUDU_LOGERIS`, `APP_BLINKY_TIMER` |
| Status globals | `MODULE_Status_THING` | `KLUDA_Status_SDCARD`, `KLUDA_LastStatus_ACCEL` |
| Defines | `MODULE_THING`, unit in a trailing comment | `CAN_MOTORSPEED_SEND_PERIOD 100`, `APP_BLINK_PERIOD 500 // ms`, `INA229_VBUS_LSB 0.0001953125f // 195.3125 uV/LSB` |
| Pin defines | `<THING>_PORT` / `<THING>_PIN`, pin in a comment | `#define GA_BRM GPIO_PIN_10 // PA10` |
| Locals | camelCase, often English | `currentTime`, `blinkyTimer`, `adcValue`, `tempSendTimer` |
| Short locals | allowed | `d`, `tm`, `ret`, `err`, `res`, `cA`, `cB`, `i` |
| Latvian locals | OK for domain words | `dati` (CAN payload), `gazespedalis`, `bremzupedalis` |
| Structs | PascalCase, optional `_t`, `Module_Thing` | `Timer_t`, `Blink`, `Logging_Instance`, `Logging_DataPacket` |
| Enums | `MODULE_StatusTypeDef` (HAL-like) or `Module_Thing`; values `MODULE_GROUP_NAME = 0x..U` | `KLUDA_StatusTypeDef { KLUDA_VISSOK = 0x00, KLUDA_SDCARD_NOT_PRESENT = 0x02U }`, `Logging_Level` |
| Error codes | high nibble = subsystem, low nibble = reason | `0x1X` accel, `0x2X` CAN, `0x3X` screen |

## Types and C habits
- `uint8_t` / `uint16_t` / `uint32_t` / `float` with `f` suffix. `char` and `unsigned long` appear in `taimeris`; keep them there.
- Booleans: `uint8_t` or `char`, with `true` / `false` macros from `taimeris.h` (`#define true (0==0)`). Status returns: `0` = OK, non-zero = error (`uint8_t err`), or `HAL_StatusTypeDef`.
- Prototypes use empty parentheses `Func()` more often than `Func(void)`; both are accepted.
- Pointer star on the type: `Timer_t* timer`, `CAN_HandleTypeDef* hcan`.
- Hex literals uppercase with `U` in enums: `0x0FU`, `0xFF`.
- Clamp floats to 0.0f-1.0f before scaling to the DAC/PWM range: `(uint32_t)(value * (float)period + 0.5f)`.
- Big-endian ("motorola") byte packing with `// MSB` / `// LSB` comments on the first and last bytes.
- Feature switches with preprocessor flags: `#define TESTING_MODE false`, `#define KLUDA_SHOW_ERROR 1`, then indented `#if` blocks inside functions.
- `goto ret;` to one cleanup/return label is accepted in init functions (see `KLUDA_InitSDCard`).
- `extern` declarations in headers for shared globals.

## Architecture
- **main.c stays thin**: in USER CODE 2, call `SetX_Handler(&hxxx)` / `KLUDA_InitX(&hxxx)` for each peripheral, with a Latvian comment above each one (`// SD kartes inicializācija`).
- **Main loop = software timers.** One `Timer_t` per periodic job, created before the loop:
```c
Timer_t helloTimer;
InitTimer(&helloTimer, 314, 0, 0, 0);   // period, offset, enabled, oneTime
EnableTimer(&helloTimer, currentTime);
while (1)
{
	currentTime = HAL_GetTick();
	if (CheckTimer(&helloTimer, currentTime))
	{
		...
	}
}
```
  Don't add `HAL_Delay` in the loop. Pass `currentTime` into update functions (`Blink_UpdateAll(currentTime)`).
- **Errors are shown on LEDs**: blink codes (count = error nibble), fast blink = error state, `Error_Handler()` for unrecoverable HAL failures.
- **CAN**: one sender per message ID (`CAN_Send_Temp() // ID = 160`), one handler per ID (`CAN_HandleLights(...) // ID = 140`), and one dispatcher `CAN_HandleFrame` that `switch`es on the ID. Comment the ID at each prototype.
- Each driver keeps an `*_EXISTS` flag and returns `0` / `0.0f` when the device is missing.
- In the VS Code template, user code lives in `App/` (`App_Init()` after `MX_*_Init`, `App_Loop()` in `while (1)`, `App_Error()` from `Error_Handler()`). `main.c` only calls these from USER CODE blocks.

## Comments
- `//` comments, short, lowercase start is fine, no full stop needed. `/* */` only for file headers and the CubeMX code around them.
- **Latvian** for what code does and for section labels: `// datu ieguve`, `// datu saformatēšana`, `// sūtījums`, `// Taimeru inicializācija`, `// CAN sūtījumi`, `// CAN apstrādātāji`, `// kopā 4 testi`, `// vajag kļūdu sistēmu implementēt šeit`.
- **English** for hardware/register facts and units: `// PB11`, `// MSB`, `// motorola`, `// 12bit`, `// 7.8125 mC/LSB`, `// map amperes to 0.0f - 1.0f for DAC output matching.`
- Trailing comments on prototypes and defines carry IDs, pins and units.
- Casual or joking comments are normal (`// nuja`, `// error statee aah`, `// PPM btw`); don't remove them when refactoring his code, and don't add new ones on his behalf.
- Use Latvian diacritics correctly (ā, č, ē, ģ, ī, ķ, ļ, ņ, š, ū, ž) in comments and strings, but not in identifiers or file names.
- No Doxygen blocks.

## Docs
- `README.md` in Latvian with a link `[English](README_en.md)` at the top; `README_en.md` is the English copy, linking back with `[Latviski](README.md)`.

## When refactoring his code
- Change naming, comments and formatting to the rules above. Keep behaviour, timings and register values exactly.
- Leave commented-out experiments in place unless he asks to clean them up (he keeps them as notes).
- Never edit outside USER CODE blocks in CubeMX-generated files.
- Ask before renaming public functions that other modules call.
