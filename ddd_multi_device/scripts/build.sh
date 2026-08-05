#!/usr/bin/env bash
# One-shot build (configure + compile) then optionally run.
# Usage: ./scripts/build.sh [run]
set -euo pipefail
cd "$(dirname "$0")/.."

cmake -S . -B build
cmake --build build

if [[ "${1:-}" == "run" ]]; then
    ./build/device_ddd
fi