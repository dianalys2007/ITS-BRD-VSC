#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

if [ -z "$1" ]; then
    read -p "Bitte geben Sie einen ADC Projektnamen ein: " project_name
else
    project_name=$1
fi

"$SCRIPT_DIR/createGenericProject.sh" --adc "$project_name"
