find -name '*.h' -or -name '*.cpp' -print0 | xargs -r0 sed -e 's/[[:blank:]]\+$//' -i
