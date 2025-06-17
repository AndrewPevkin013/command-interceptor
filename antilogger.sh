#!/bin/bash

echo "[antilogger] Clean logger..."

ps aux | grep bash | grep liblogger.so | awk '{print $2}' | xargs -r kill -9
env -u LD_PRELOAD rm -f /tmp/execve_log.txt

echo "[antilogger] DONE"
