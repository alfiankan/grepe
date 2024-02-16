#!/bin/bash


grepe --group-by '200' '400'\
      --time-pattern "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}" \
      --time-format "%d/%b/%Y:%H:%M:%S" \
      --start "2020-02-11 22:17:00" \
      --end "2029-02-11 22:18:00" \
      --step 86400 \
      --path mylog
