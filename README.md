## GREPE - In Terminal Time Series Log Decoder

"Grepe" is a playful nod to "Grep," a robust pattern matching tool in GNU, with the added twist of incorporating "e" for "eseries." This tool serves as a valuable resource when traditional time-series logging tools such as Loki, Splunk, or Datadog aren't available due to cost constraints or other limitations.

Designed to be used directly in the terminal, Grepe offers a simple yet effective solution for visualizing time-series patterns extracted from log data. By leveraging regular expressions and a hash map, it efficiently generates bar charts directly within the terminal interface.

In practical terms, Grepe proves particularly useful for scenarios where monitoring for specific events, such as requests with a status code of "400," is necessary but conventional time-series logging tools aren't feasible. With Grepe, users can gain insights into these patterns in their log data without the need for additional, potentially costly observability solutions.

## How to Install
1. Downloaf from release page https://github.com/alfiankan/grepe/releases/tag/0.1.0-beta
2. there is 2 binaries, `grepe` is main cli and `grepe-test` is to try using by generating demo data and quick demo.

## Spoiler
```bash
~/workspace/grepe on master *5 !4 ?22                                                                                                                      took 48s node system
> grepe --group-by '200' '400'\
      --time-pattern "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}" \
      --time-format "%d/%b/%Y:%H:%M:%S" \
      --start "2020-02-11 22:17:00" \
      --end "2029-02-11 22:18:00" \
      --step 86400 \
      --path mylog
TOTAL PAGES SIZE: 4915200
Processing [100.00%] 104857633/104857632 bytes
================================================================================================================================================================================
  grepe v0.1.0-beta
  TIME RANGE  : 2023-11-04 12:30:00 ~ 2023-11-04 13:00:00
  PROCESSED   : 514007 line
  MAX         : 178
  MIN         : 119

  Legend      :
  [A] 200
  [B] 400
================================================================================================================================================================================

2023-11-04 12:30:00 |  A ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ (139)
2023-11-04 12:30:00 |  B ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ (122)

2023-11-04 12:35:00 |  A ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ (130)
2023-11-04 12:35:00 |  B ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ (150)

2023-11-04 12:40:00 |  A ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ (154)
2023-11-04 12:40:00 |  B ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ (145)

2023-11-04 12:45:00 |  A ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ (124)
2023-11-04 12:45:00 |  B ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ (158)

2023-11-04 12:50:00 |  A ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ (151)
2023-11-04 12:50:00 |  B ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ (154)

2023-11-04 12:55:00 |  A ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ (119)
2023-11-04 12:55:00 |  B ▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓ (178)
```



## How to use
```bash

grepe --group-by '200' '400'\
      --time-pattern "[[0-9]{2}/[A-Za-z]+/[0-9]{4}:[0-9]{2}:[0-9]{2}:[0-9]{2}" \
      --time-format "%d/%b/%Y:%H:%M:%S" \
      --start "2020-02-11 22:17:00" \
      --end "2029-02-11 22:18:00" \
      --step 86400 \
      --path mylog

```

or if you just want to see the demo use

```bash
grepe-test
```

## Build From Sourse
1. Usig cmake and build tool GNU `mkdir build && cd build && cmake .. && make && sudo make install && cd ..`
2. Using Zig toolchain `make build-all-cross-platform`
