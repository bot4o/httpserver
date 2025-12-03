#!/bin/bash
echo "[*] Building backend and generating compile_commands.json..."
bear -- make

echo "[*] Done! clangd should now work correctly."

