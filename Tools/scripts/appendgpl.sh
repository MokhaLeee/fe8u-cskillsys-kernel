#!/bin/bash

spdx_header="// SPDX-License-Identifier: GPL-2.0-only"

for file in $(find . -type f -name "*.c"); do
    if [[ -f "$file" ]]; then
        if ! grep -q "$spdx_header" "$file"; then
            echo -e "$spdx_header\n\n$(cat "$file")" > "$file"
            echo "Added SPDX header to $file"
        else
            echo "SPDX header already exists in $file"
        fi
    fi
done
