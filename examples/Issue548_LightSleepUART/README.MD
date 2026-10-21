# Issue 548: T-SIM7600X UART Light-Sleep Test

This example verifies large `AT+CFTRANTX` responses on the `T-SIM7600X` using the standard SIMCom UART sleep/wake sequence. It is intentionally a reproducible test case, not an OTA application.

## Board and pins

Select `T-SIM7600X` in `platformio.ini`. The example uses the existing `../ATdebug/utilities.h` board definition:

| Signal | ESP32 GPIO |
| --- | ---: |
| Modem RX (ESP32 receives TXD) | 26 |
| Modem TX (ESP32 sends RXD) | 27 |
| DTR | 32 |
| RI | 33 |

The CH9102 console is the ESP32 control port. A SIM7600 USB AT port such as `COM190` is a separate USB interface and is not the GPIO26/27 UART tested here.

## Build and upload

Temporarily select this example in the repository `platformio.ini`:

```ini
default_envs = T-SIM7600X
src_dir = examples/Issue548_LightSleepUART
```

Then build/upload in the normal way. The example can also be opened directly in Arduino IDE after copying the repository `lib` libraries.

## Test commands

Open the ESP32 console at 115200 baud:

- `r`: create `E:/issue548.bin`, a deterministic 4096-byte file;
- `b`: baseline, with DTR low and `AT+CSCLK=0`;
- `l`: standard light-sleep test.

The `l` sequence is:

1. send `AT+CSCLK=1` and pull DTR high;
2. wait 25 seconds for the modem sleep condition;
3. enter ESP32 light sleep;
4. after wake, rebuild `Serial1`, pull DTR low, and wait;
5. require `AT` -> `OK`, set `AT+CATR=1`, then send `AT+CSCLK=0`;
6. read four 1024-byte windows with `AT+CFTRANTX` and verify every byte.

If DTR is not pulled low, or the application starts the large transfer before a clean `AT`/`OK` wake confirmation, it has not followed the SIMCom wake procedure and the result must be treated as an application-flow error.

For automatic logging from the repository root:

```powershell
python examples/Issue548_LightSleepUART/issue548_serial_runner.py --port COM227 --cycles 1
```

The runner toggles RTS to reset the ESP32, sends `r`, `b`, and `l`, and stores a timestamped log next to the runner.
