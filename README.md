## Contract

```

grepe json_line \
    -t 'time=%Y-%m-%d %H-%M-%S' \
    -g 'response.status' \
    -c '400|201'


```
