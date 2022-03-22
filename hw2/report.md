EE6470 Electronic System Level Design and Synthesis

Homework 2

Name: LEE YU-CHOU ID: 107061144

-   Repository URL:
    [https://github.com/leetoby/ESL/tree/main/hw2](https://github.com/leetoby/ESL/tree/main/hw2)

Problems: Gaussian Blur with a TLM-2.0 interconnect

-   Modify the filter in the header file *filter_def.h*

-   In *SobelFilter.cpp*, we modified the o_result to o_r, o_g, and o_b in the
    do_filter() function. Also, the buffer should be also modified to connect
    the three fifo port in blocking_transport() function.

-   In the testbench, we modified i_result to receive the three output from the
    filter and write into the image.

| Before blur                                                                                        | After blur                                      |
|----------------------------------------------------------------------------------------------------|-------------------------------------------------|
| ![一張含有 女性, 服飾, 髮, 個人 的圖片 自動產生的描述](media/d872d38bd03a9fc64cd538af771a7526.png) | ![](media/dff1acffe1d3b0bb749d3476a11e7dc5.png) |
| ![](media/72494dd719e30f58e1a036a9b57d7b20.png)                                                    | ![](media/c8aa6b1be2bf4ef1847db5a01494b172.png) |
