#!/usr/bin/env python3
"""Run and record the Issue 548 firmware test over the ESP32 console."""

from __future__ import annotations

import argparse
from datetime import datetime
from pathlib import Path
import sys
import threading
import time

import serial


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Record the Issue 548 T-SIM7600X test")
    parser.add_argument("--port", default="COM227")
    parser.add_argument("--cycles", type=int, default=1, help="Number of light-sleep cycles")
    parser.add_argument("--log", type=Path, help="Log path; defaults to a timestamped file")
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if args.cycles < 1:
        raise SystemExit("--cycles must be at least 1")

    log_path = args.log or Path(__file__).resolve().parent / f"issue548_serial_{datetime.now():%Y%m%d_%H%M%S}.log"
    log_path.parent.mkdir(parents=True, exist_ok=True)
    stop = threading.Event()

    with serial.Serial(args.port, 115200, timeout=0.1, write_timeout=2) as port, log_path.open(
        "w", encoding="utf-8", errors="replace"
    ) as log:
        started = datetime.now().isoformat(timespec="seconds")
        header = f"# Issue 548 serial run\n# port={args.port} baud=115200 cycles={args.cycles}\n# started={started}\n"
        print(header, end="")
        log.write(header)
        log.flush()

        def reader() -> None:
            while not stop.is_set():
                data = port.read(port.in_waiting or 1)
                if not data:
                    continue
                text = data.decode("utf-8", errors="replace")
                sys.stdout.write(text)
                sys.stdout.flush()
                log.write(text)
                log.flush()

        thread = threading.Thread(target=reader, daemon=True)
        thread.start()

        # CH9102 RTS is connected to the ESP32 auto-reset circuit on this board.
        port.dtr = False
        port.rts = True
        time.sleep(0.25)
        port.rts = False
        time.sleep(5.0)

        def command(value: str, wait: float) -> None:
            print(f"\n# SEND {value!r}; wait={wait:.1f}s")
            port.write(value.encode("ascii"))
            port.flush()
            time.sleep(wait)

        command("r\n", 5.0)
        command("b\n", 12.0)
        for cycle in range(1, args.cycles + 1):
            print(f"\n# LIGHT_SLEEP_CYCLE {cycle}/{args.cycles}")
            command("l\n", 50.0)

        stop.set()
        thread.join(timeout=1.0)
        footer = f"\n# finished={datetime.now().isoformat(timespec='seconds')}\n# log={log_path}\n"
        print(footer, end="")
        log.write(footer)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
