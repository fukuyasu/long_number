# long_number

## ビルド
```
$ make
```

## PI
```
$ ./pi
pi:
      3.1415 9265 3589 7932 3846 2643 3832 7950 2884 1971 (   1-  40)
        6939 9375 1058 2097 4944 5923 0781 6406 2862 0899 (  41-  80)
        8628 0348 2534 2117 0679 8214 8086 5132 8230 6647 (  81- 120)
        0938 4460 9550 5822 3172 5359 4081 2848 1117 4502 ( 121- 160)
        8410 2701 9385 2110 5559 6446 2294 8954 9303 8196 ( 161- 200)
        4428 8109 7566 5933 4461 2847 5648 2337 8678 3165 ( 201- 240)
        2712 0190 9145 6485 6692 3460 3486 1045 4326 6482 ( 241- 280)
        1339 3607 2602 4914 1273 7245 8700 6606 3155 8817 ( 281- 320)
        4881 5209 2096 2829 2540 9171 5364 3678 9259 0360 ( 321- 360)
        0113 3053 0548 8204 6652 1384 1469 5194 1511 6094 ( 361- 400)
        3305 7270 3657 5959 1953 0921 8611 7381 9326 1179 ( 401- 440)
        3105 1185 4807 4462 3799 6274 9567 3518 8575 2724 ( 441- 480)
        8912 2793 8183 0119 4912 9833 6733 6244 0656 6430 ( 481- 520)
        8602 1394 9463 9522 4737 1907 0217 9860 9437 0277 ( 521- 560)
        0539 2171 7629 3176 7523 8467 4818 4676 6940 5132 ( 561- 600)
        0005 6812 7145 2635 6082 7785 7713 4275 7789 6091 ( 601- 640)
        7363 7178 7214 6844 0901 2249 5343 0146 5495 8537 ( 641- 680)
        1050 7922 7968 9258 9235 4201 9956 1121 2902 1960 ( 681- 720)
        8640 3441 8159 8136 2977 4771 3099 6051 8707 2113 ( 721- 760)
        4999 9998 3729 7804 9951 0597 3173 2816 0963 1859 ( 761- 800)
        5024 4594 5534 6908 3026 4252 2308 2533 4468 5035 ( 801- 840)
        2619 3118 8171 0100 0313 7838 7528 8658 7533 2083 ( 841- 880)
        8142 0617 1776 6914 7303 5982 5349 0428 7554 6873 ( 881- 920)
        1159 5628 6388 2353 7875 9375 1957 7818 5778 0532 ( 921- 960)
        1712 2680 6613 0019 2787 6611 1959 0921 6420 1989 ( 961-1000)

1000 digits: 0.009 seconds, 717 iterations.
```

## Benchmark

| Hardware                                           | 1k digits | 2k    | 5k    | 10k    | 100k     |
| :------------------------------------------------- | --------: | ----: | ----: | -----: | -------: |
| MSYS2 / i7-1065G7 / GCC 10.2.0                     | 0.008 sec | 0.029 | 0.161 |  0.682 |   66.561 |
| Ubuntu 20.04 on WSL2 / i7-1065G7 / GCC 9.3.0       | 0.008 sec | 0.027 | 0.177 |  0.686 |   66.804 |
| Ubuntu 20.04 on WSL1 / i7-1065G7 / GCC 9.3.0       | 0.008 sec | 0.028 | 0.173 |  0.682 |   70.648 |
| Darwin 18.7.0 / i7 2.8GHz / Clang 1001.0.46.4      | 0.010 sec | 0.038 | 0.209 |  0.814 |   76.935 |
| MSYS2 / Xeon W-2225 / GCC 11.2.0                   | 0.008 sec | 0.036 | 0.206 |  0.845 |   85.886 |
| Cygwin / Xeon W-2123 / GCC 7.3.0                   | 0.009 sec | 0.035 | 0.221 |  0.883 |   89.453 |
| FreeBSD 12.0 on XenServer / E3-1225 / Clang 6.0.1  | 0.010 sec | 0.039 | 0.246 |  0.983 |   98.760 |
| FreeBSD 11.1 on XenServer / i7-4770K / Clang 4.0.0 | 0.010 sec | 0.041 | 0.256 |  1.023 |  102.948 |
| MSYS2 / Xeon W-2123 / GCC 10.2.0                   | 0.010 sec | 0.040 | 0.252 |  1.003 |  100.709 |
| MSYS2 / Xeon W-2123 / GCC 7.4.0                    | 0.010 sec | 0.046 | 0.255 |  1.091 |  103.829 |
| MinGW64 / Xeon W-2123 / GCC 8.2.1                  | 0.011 sec | 0.045 | 0.252 |  1.015 |  103.935 |
| MinGW64 / Xeon W-2123 / GCC 8.1.0                  | 0.010 sec | 0.042 | 0.255 |  1.017 |  104.410 |
| Ubuntu 18.04 on WSL / Xeon W-2123 / GCC 7.3.0      | 0.010 sec | 0.041 | 0.271 |  1.011 |  109.510 |
| Ubuntu 16.04 on WSL / Xeon W-2123 / GCC 5.4.0      | 0.011 sec | 0.045 | 0.284 |  1.145 |  115.053 |
| MSYS2 / i7-8500Y / GCC 7.4.0                       | 0.010 sec | 0.040 | 0.236 |  0.944 |  112.389 |
| MinGW64 / i7-8500Y / GCC 9.1.0                     | 0.010 sec | 0.039 | 0.233 |  0.947 |  112.643 |
| Ubuntu 18.04 on WSL / i7-8500Y / GCC 7.4.0         | 0.012 sec | 0.042 | 0.245 |  0.943 |  110.994 |
| MinGW64 / Pentium 4415Y / GCC 8.1.0                | 0.015 sec | 0.093 | 0.593 |  2.358 |  234.600 |
| Ubuntu 18.04 on WSL / Pentium 4415Y / GCC 7.3.0    | 0.023 sec | 0.094 | 0.589 |  2.358 |  236.027 |
| Raspbian / Raspberry Pi 3 Model B / GCC 4.9.2      | 0.120 sec | 0.478 | 2.982 | 11.932 | 1193.150 |
