EE6470 Electronic System Level Design and Synthesis

Homework 1

Name: LEE YU-CHOU ID: 107061144

-   Repository URL: <https://github.com/leetoby/ESL/tree/main/hw1>

Problems:

1.  Gaussian Blur reusing the same code base from Sobel Filter with FIFO
    channels in lab 2.

-   Rewrite original FIFO o_result to o_result_r, o_result_g, and o_result_b.

-   The factor should be 1/16 since the sum of the filter is 16. Note the we can
    use left shift to replace division.

1.  Data movement with row buffers.

-   I use 4 rows to save the RBG values. Three rows are used to calculate the
    convolution and the fourth row is upgraded to be the new value receiving
    from the testbench.

-   Note that we cannot do convolution in the first two rows. Therefore, we need
    to wait the first two rows and begin to do convolution.

| Before blur                                                                                        | After blur                                                                                         |
|----------------------------------------------------------------------------------------------------|----------------------------------------------------------------------------------------------------|
| ![一張含有 女性, 服飾, 髮, 個人 的圖片 自動產生的描述](media/d872d38bd03a9fc64cd538af771a7526.png) | ![](media/dff1acffe1d3b0bb749d3476a11e7dc5.png)                                                    |
| ![一張含有 女性, 服飾, 髮, 個人 的圖片 自動產生的描述](media/d872d38bd03a9fc64cd538af771a7526.png) | ![一張含有 服飾, 女性, 髮, 靠近 的圖片 自動產生的描述](media/dff1acffe1d3b0bb749d3476a11e7dc5.png) |
