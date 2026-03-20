# Inverti argomenti
# Scrivi uno script inverti.sh che stampi gli argomenti in modo inverso.
# Per esempio ./inverti.sh uno due tre stamperà tre due uno.

#!/bin/bash

args=()

while [[ $1 != "" ]]; do
    args=( "$1" "${args[@]}" );
    shift;
done

echo "${args[@]}"