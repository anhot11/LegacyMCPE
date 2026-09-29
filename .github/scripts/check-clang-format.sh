#!/usr/bin/env bash

set -euo pipefail

formatter="${CLANG_FORMAT_BIN:-clang-format-19}"
base_ref="${1:-}"
head_ref="${2:-HEAD}"

if [[ -z "$base_ref" ]]; then
    if git rev-parse --verify HEAD^ >/dev/null 2>&1; then
        base_ref="$(git rev-parse HEAD^)"
    else
        echo "No comparison base available; skipping clang-format check."
        exit 0
    fi
fi

diff_output="$(
    git diff --name-only --diff-filter=ACMR "$base_ref" "$head_ref" -- \
        '*.c' '*.cc' '*.cpp' '*.cxx' '*.h' '*.hh' '*.hpp' '*.hxx' '*.inl'
)"

if [[ -z "$diff_output" ]]; then
    echo "No changed C/C++ files to check."
    exit 0
fi

mapfile -t files <<<"$diff_output"

echo "Checking formatting for changed files:"
printf '  %s\n' "${files[@]}"

"$formatter" --version
"$formatter" --dry-run --Werror "${files[@]}"