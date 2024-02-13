#!/bin/bash


./build/grepe --time-pattern "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}" \
      --time-format "%d/%b/%Y:%H:%M:%S" \
      --start "2024-02-11 22:17:00" \
      --end "2024-02-11 22:18:00" \
      --step 10 \
      --group-by '200' \
      --group-by '400' \
      --path mylog
