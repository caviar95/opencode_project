#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/.."

if [[ "${1:-}" == "--fix" ]]; then
  clang-tidy src/calculator.cpp tests/test_calculator.cpp src/main.cpp \
    -p build/dev --fix --fix-errors 2>/dev/null || true
  exit 0
fi

EXTRA_CMAKE_FLAGS=()
if [[ "$(uname -s)" == "Darwin" ]]; then
  EXTRA_CMAKE_FLAGS+=("-DCMAKE_CXX_FLAGS=-isysroot $(xcrun --show-sdk-path)")
fi

cmake -S . -B build/tidy --fresh -DAUTO_BUILD_TESTS=ON -DAUTO_BUILD_EXAMPLES=ON \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON "${EXTRA_CMAKE_FLAGS[@]}" >/dev/null

run_tidy() {
  clang-tidy "$1" -p build/tidy 2>/dev/null
}

status=0
for f in src/calculator.cpp src/main.cpp tests/test_calculator.cpp; do
  echo "== linting $f =="
  run_tidy "$f" || status=1
done
exit "$status"
