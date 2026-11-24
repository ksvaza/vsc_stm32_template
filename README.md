# vsc_stm32_template

[English](README_en.md)

VS Code + CMake sagatave **STM32F103C8T6 "Blue Pill"** plates programmēšanai.
Projekts veidots ar STM32CubeMX un ST oficiālo VS Code paplašinājumu, un tiek ierakstīts un atkļūdots ar **ST-Link V2** (der arī klons).

| Iestatījums  | Vērtība                                                |
|--------------|--------------------------------------------------------|
| MCU          | STM32F103C8Tx (LQFP48, 64 KB flash, 20 KB RAM)         |
| Takts        | 8 MHz HSE kvarcs × PLL 9 = **72 MHz** (APB1 36, APB2 72, USB 48) |
| Atkļūdošana  | SWD (PA13 SWDIO, PA14 SWCLK)                           |
| Kompilēšana  | CMake + Ninja + arm-none-eabi-gcc (no ST paplašinājuma) |
| HAL          | STM32Cube FW_F1 V1.8.x, nokopēti tikai izmantotie faili |
| Piemērs      | PC13 gaismas diode (`LEDBUILTIN`, active-low) mirgo ar 1 Hz |

## Struktūra

```
App/              <- TAVS kods. CubeMX šo mapi nekad neaizskar.
  Inc/app.h
  Src/app.c       App_Init() + App_Loop() + App_Error()
  Inc/taimeris.h  programmatūras taimeri (Timer_t, CheckTimer, ...)
  Src/taimeris.c
Core/             <- ģenerē CubeMX (main.c, HAL konfigurācija, pārtraukumi)
Drivers/          <- ģenerēts: HAL + CMSIS (ir git, tāpēc klons kompilējas uzreiz)
cmake/            <- ģenerēts: rīkkopas fails + CubeMX failu saraksts
CMakeLists.txt    <- ģenerēts vienreiz, tālāk tavs (šeit pievienota App/)
CMakePresets.json <- Debug / Release iestatījumi
*.ioc             <- atver CubeMX, lai mainītu pinus un perifērijas
```

Noteikums: `Core/` mapē raksti tikai starp `/* USER CODE BEGIN */ ... /* USER CODE END */`.
Viss pārējais ir `App/` mapē. Jauni moduļi: `App/Inc/<nosaukums>.h` + `App/Src/<nosaukums>.c`,
katru `.c` failu pievieno `CMakeLists.txt` sarakstam `target_sources`.

Galvenā cikla princips (sk. `App/Src/app.c`): katram periodiskam darbam savs `Timer_t`,
`App_Loop()` iekšā `if (CheckTimer(&TAIMERIS, currentTime))`.

Ja palaišana neizdodas (piemēram, nestartē kvarcs), `Error_Handler()` izsauc `App_Error()`,
un PC13 bezgalīgi ātri mirgo (~5 Hz).

Formatējums: tabi + CRLF, to nosaka `.editorconfig` un `.vscode/settings.json`.

## Datora sagatavošana (vienreiz, Windows)

1. **VS Code** ar paplašinājumu **STM32Cube for Visual Studio Code**
   (`stmicroelectronics.stm32-vscode-extension`, VS Code to piedāvā, atverot šo mapi).
2. **Rīkkopa**: pirmajā reizē ST paplašinājums pats lejupielādē GCC, CMake, Ninja un ST-Link rīkus
   uz `%LOCALAPPDATA%\stm32cube\bundles`. Piekrīti tā piedāvājumiem. STM32CubeCLT atsevišķi nav vajadzīgs.
3. **STM32CubeMX** (6.17 vai jaunāks), vajadzīgs tikai, mainot `.ioc`.
4. **ST-Link USB draiveris** (STSW-LINK009).
5. **EditorConfig** paplašinājums (tiek piedāvāts automātiski), lai jaunie faili ir ar tabiem + CRLF.

## ST-Link V2 pieslēgšana

| ST-Link | Blue Pill (4 pinu kontakti galā) |
|---------|------------------------------|
| 3.3V    | 3.3                          |
| SWDIO   | DIO (PA13)                   |
| SWCLK   | CLK (PA14)                   |
| GND     | GND                          |

Abus dzeltenos džemperus (BOOT0 un BOOT1) atstāj pozīcijā **0**.

## Ikdienas darbs (tikai VS Code)

| Darbība | Kā |
|---|---|
| Kompilēt | `F7` vai *Build* statusa joslā |
| Kompilēt + ierakstīt + atkļūdot (apstājas pie `main`) | `F5` |
| Kompilēt + ierakstīt + palaist, bez atkļūdotāja | `Ctrl+F5` |
| Mainīt pinus/perifērijas | atver `.ioc` CubeMX → **Generate Code**, tad `F7` |

Atkļūdošanas konfigurācija ir `.vscode/launch.json` ("Blue Pill: build + flash + debug").
Salīdzinot ar CubeIDE: Debug ≈ `F5`, Run ≈ `Ctrl+F5`, un CubeProgrammer solis notiek automātiski.

**BOOT0 jābūt 0** (abi dzeltenie džemperi uz 0). Ja BOOT0 = 1, mikrokontrolieris palaiž iebūvēto
seriālo ielādētāju, un tava programma nestartē, lai gan ierakstīšana caur SWD izdodas.

## Jauns projekts no šīs sagataves

1. Nokopē mapi (vai GitHub izmanto "Use this template").
2. Pārdēvē `vsc_stm32_template.ioc` par `<nosaukums>.ioc`, atver to CubeMX, iestati
   *Project Manager → Project Name* uz `<nosaukums>` un spied **Generate Code**.
3. `CMakeLists.txt` failā iestati `CMAKE_PROJECT_NAME` uz `<nosaukums>` (ja CubeMX to neizdarīja).

## Problēmu risināšana

- **Klonēts mikrokontrolieris (CKS32 / CS32 / APM32 / GD32)**: parasti strādā ar ST-Link V2.
  Ja ST GDB serveris atsakās ("not an STM32 device" vai nepareizs IDCODE),
  izmanto OpenOCD ar `-c "set CPUTAPID 0"` un Cortex-Debug paplašinājumu.
- **"No target found" pēc vecas programmas, kurā SWD bija izslēgts**: turi RESET, sāc ierakstīšanu,
  atlaid RESET ~0,5 s vēlāk. Vai arī: BOOT0 uz 1, atslēdz un pieslēdz barošanu, ieraksti, BOOT0 atpakaļ uz 0.
  (Šajā sagatavē SWD ir ieslēgts: *SYS → Debug = Serial Wire*. Nekad neliec "No Debug".)
- **Gaismas diode šķiet otrādi**: PC13 ir active-low, `GPIO_PIN_RESET` to **ieslēdz**.
- **PC13 ātri mirgo**: kaut kas inicializācijā izsauca `Error_Handler()`. Visbiežāk nestartē kvarcs:
  CubeMX nomaini PLL avotu uz HSI/2 × 16 = 64 MHz.
