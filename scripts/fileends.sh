find . src -maxdepth 1 -type f -not -name '*.gitignore' | sed 's/^.*\.//' | sort -u
