#!/bin/bash

set -e
set -o pipefail

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 file1.txt file2.bin file3.json ..."
    exit 1
fi

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
INSTALL_ROOT="$(realpath "$SCRIPT_DIR/..")"
BIN_DIR="$INSTALL_ROOT/bin"

PARSER_EXEC="$BIN_DIR/MiniParserVisualizer"
PYTHON_SCRIPT="$BIN_DIR/plot.py"

if [ ! -x "$PARSER_EXEC" ]; then
    echo "C++ parser executable not found or not executable at: $PARSER_EXEC"
    exit 1
fi

if [ ! -f "$PYTHON_SCRIPT" ]; then
    echo "Python script not found at: $PYTHON_SCRIPT"
    exit 1
fi

"$PARSER_EXEC" "$@" | python3 "$PYTHON_SCRIPT"
STATUS=$?

if [ "$STATUS" -ne 0 ]; then
    echo "Pipeline failed"
    exit $STATUS
fi

echo "Pipeline completed successfully"
exit 0
