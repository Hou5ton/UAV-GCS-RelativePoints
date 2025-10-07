#!/bin/bash

ignore_patterns=$(cat .clang-format-ignore)
patterns=$(cat .clang-format-dirs)

format_file() {
  #return
  file="${1}"
  #echo "Found ${file}..."
  [[ $file != *.c ]] && [[ $file != *.cpp ]] && [[ $file != *.h ]] && return
  [[ -f $file ]] || return 0
  for p in $patterns; do
    [[ $p == "" ]] && continue
    [[ $file =~ "$p" ]] || continue
    for ip in $ignore_patterns; do
      [[ $ip == "" ]] && continue
      [[ $file =~ "$ip" ]] && return 0
    done

    [[ $silent ]] || echo "Reformatting ${file}..."
    /usr/local/bin/clang-format -style=file -fallback-style=none -i ${file}
    git add "${file}"
  done
}


case "${1}" in
  --about )
    echo "Runs clang-format on source files"
    ;;
  --all )
    echo "Fixing codestyle for all files..."
    for file in `git ls-files` ; do
      format_file "${file}"
    done
    ;;
  * )
    echo "Fixing codestyle for modified files..."
    #silent=1
    for file in `git diff-index --cached --name-only HEAD` ; do
      format_file "${file}"
    done
    ;;
esac
