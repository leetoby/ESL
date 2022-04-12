EE6470 Electronic System Level Design and Synthesis

Homework 2

Name: LEE YU-CHOU	ID: 107061144

- Repository URL: <https://github.com/leetoby/ESL/tree/main/hw3>
- Problems: High level synthesis of Gaussian Blur

The areas and run time of different versions of synthesized circuits are shown below:


|**BASIC**|
| :-: | :-: | :-: |
||Area|Run Time|
|NORMAL|963.8|43909110 ns|
|PIPELINE (II=1)|632.7|20316150 ns|
|PIPELINE (II=2)|662.8|24248310 ns|
|UNROLLING (NUM=9)|1528.1|43253750 ns|


|**DPA**|
| :-: | :-: | :-: |
||Area|Run Time|
|NORMAL|926.1|43909110 ns|
|PIPELINE (II=1)|625.2|20316150 ns|
|PIPELINE (II=2)|655.3|24248310 ns|
|UNROLLING (NUM=9)|1430.2|24248310 ns|

- Timing annotation of Gaussian Blur module

In TLM version, we can add some buffer to split the convolution into some stages, also add some adders and multipliers to accelerate convolution.

- The result of blur is shown below:

|Before blur|After blur|
| :-: | :-: |
|![](Aspose.Words.2eba1d47-6302-45ca-9109-d68a1ea8aa03.001.png)|![](Aspose.Words.2eba1d47-6302-45ca-9109-d68a1ea8aa03.001.png)|
|![](Aspose.Words.2eba1d47-6302-45ca-9109-d68a1ea8aa03.002.png)|![](Aspose.Words.2eba1d47-6302-45ca-9109-d68a1ea8aa03.003.png)|

