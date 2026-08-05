#!/usr/bin/env bash
set -euo pipefail

cd "$(dirname "$0")/.."

if ! command -v clang-format >/dev/null 2>&1; then
  echo "error: clang-format not found" >&2
  exit 1
fi

FILES=$(find include src tests -type f \( -name '*.h' -o -name '*.hpp' -o -name '*.cpp' \) | sort)

if [[ "${1:-}" == "--fix" ]]; then
  # shellcheck disable=SC2086
  clang-format -i $FILES
  echo "formatted files"
  exit 0
fi

if ! clang-format --dry-run --Werror $FILES; then
  echo "format check failed; run: scripts/check_format.sh --fix" >&2
  exit 1
fi
echo "format check passed"
