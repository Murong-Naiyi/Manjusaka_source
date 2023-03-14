find / \
  \( -path "/mnt" \
  -o -path "/data" \
  -o -path "/proc" \
  -o -path "/dev" \
  -o -path "/storage" \
  -o -path "/data_mirror" \
  -o -path "/apex" \
  -o -path "/sys" \
  -o -path "/acct" \) \
  -prune \
  -o -iname "*thermal*" \
  -type f \
  ! -name "*.so" \
  ! -name "*@*" \
  ! -name "*hardware*" \
  -print0 \
  | while IFS= read -r -d '' Manjusaka; do
    case "$Manjusaka" in
      *.so|*@|*hardware)
        continue
        ;;
    esac
    wc -c "$Manjusaka"
  done