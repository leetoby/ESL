# **EE6470 Electronic System Level Design and Synthesis**
## **Final project**
### **Topic: Implement multiple accelerator PEs with a multi-core riscv-vp platform**
### **Implementation: Discrete Cosine Transform**
#### Algorithm:
Definition: The DCT-II is

![](Aspose.Words.30c7deea-5bbf-458c-b514-838938c7dc8e.001.png)

Where: X is the DCT output; x is the input; k is the index of the output coefficient being calculated, from 0 to 7; s is a scaling function, sy=1 except s0=12.


|<p>a1 = sqrt(0.5)</p><p>a2 = sqrt(2) \* cos(3/16 \* 2 \* pi)</p><p>a3 = sqrt(0.5)</p><p>a4 = sqrt(2) \* cos(1/16 \* 2 \* pi)</p><p>a5 = cos(3/16 \* 2 \* pi)</p><p></p><p>s0 = cos(0)           / ( 1      ) \* 2</p><p>s1 = cos(1.\*pi/16) / (-a5+a4+1) \* 2</p><p>s2 = cos(2.\*pi/16) / ( a1+1   ) \* 2</p><p>s3 = cos(3.\*pi/16) / ( a5+1   ) \* 2</p><p>s4 = cos(4.\*pi/16) / ( 1      ) \* 2</p><p>s5 = cos(5.\*pi/16) / ( 1-a5   ) \* 2</p><p>s6 = cos(6.\*pi/16) / ( 1-a1   ) \* 2</p><p>s7 = cos(7.\*pi/16) / ( a5-a4+1) \* 2</p><p></p><p>def DCT(i):</p><p>`    `# i is an input array with length=8</p><p>`    `b0 =  i[0] + i[7]</p><p>`    `b1 =  i[1] + i[6]</p><p>`    `b2 =  i[2] + i[5]</p><p>`    `b3 =  i[3] + i[4]</p><p>`    `b4 = -i[4] + i[3]</p><p>`    `b5 = -i[5] + i[2]</p><p>`    `b6 = -i[6] + i[1]</p><p>`    `b7 = -i[7] + i[0]</p><p></p><p>`    `c0 =  b0 + b3</p><p>`    `c1 =  b1 + b2</p><p>`    `c2 = -b2 + b1</p><p>`    `c3 = -b3 + b0</p><p>`    `c4 = -b4 - b5</p><p>`    `c5 =  b5 + b6</p><p>`    `c6 =  b6 + b7</p><p>`    `c7 =  b7</p><p></p><p>`    `d0 =  c0 + c1</p><p>`    `d1 = -c1 + c0</p><p>`    `d2 =  c2 + c3</p><p>`    `d3 =  c3</p><p>`    `d4 =  c4</p><p>`    `d5 =  c5</p><p>`    `d6 =  c6</p><p>`    `d7 =  c7</p><p>`    `d8 = (c4 + c6) \* a5</p><p></p><p>`    `e0 =  d0</p><p>`    `e1 =  d1</p><p>`    `e2 =  d2 \* a1</p><p>`    `e3 =  d3</p><p>`    `e4 = -d4 \* a2 - d8</p><p>`    `e5 =  d5 \* a3</p><p>`    `e6 =  d6 \* a4 - d8</p><p>`    `e7 =  d7</p><p></p><p>`    `f0 = e0</p><p>`    `f1 = e1</p><p>`    `f2 = e2 + e3</p><p>`    `f3 = e3 - e2</p><p>`    `f4 = e4</p><p>`    `f5 = e5 + e7</p><p>`    `f6 = e6</p><p>`    `f7 = e7 - e5</p><p></p><p>`    `g0 =  f0</p><p>`    `g1 =  f1</p><p>`    `g2 =  f2</p><p>`    `g3 =  f3</p><p>`    `g4 =  f4 + f7</p><p>`    `g5 =  f5 + f6</p><p>`    `g6 = -f6 + f5</p><p>`    `g7 =  f7 - f4</p><p></p><p>`    `o[0] = g0 \* s0</p><p>`    `o[4] = g1 \* s4</p><p>`    `o[2] = g2 \* s2</p><p>`    `o[6] = g3 \* s6</p><p>`    `o[5] = g4 \* s5</p><p>`    `o[1] = g5 \* s1</p><p>`    `o[7] = g6 \* s7</p><p>`    `o[3] = g7 \* s3</p><p></p><p>`    `return o</p>|
| :- |
#### Example for 8x8 DCT:
![](Aspose.Words.30c7deea-5bbf-458c-b514-838938c7dc8e.002.png)
#### Architecture:
![](Aspose.Words.30c7deea-5bbf-458c-b514-838938c7dc8e.003.png)
#### How to parallel
Input the 8 pixels to the first DCT module and get the DCT coefficient in even order. And input the 8 pixels to the second DCT module and get the DCT coefficient in odd order. And we can easily combine the result to get the full DCT coefficients.
#### RISCV result
- One core:
  - simulation time: 207802480 ns
  - num-instr: 6244028
- Two core:
  - Core 1:
    - simulation time: 195492990 ns
    - num-instr: 5993601
  - Core 2:
    - simulation time: 195492990 ns
    - num-instr: 5729575
#### Core 1 Stratus Syn. Result

||**Run time (ns)**|**Area**|
| :- | :- | :- |
|BASIC|286710|8981.9|
|BASIC\_PIPELINED 1|20500|17367.1|
|BASIC\_PIPELINED 2|40980|13109.2|
|BASIC\_PIPELINED 3|61450|12205.7|
|DPA|184310|9138.6|
|DPA\_PIPELINED 1|20500|10441.7|
|DPA\_PIPELINED 2|40970|10814.6|
|DPA\_PIPELINED 3|61450|11485.5|
#### Core 2 Stratus Syn. Result

||**Run time (ns)**|**Area**|
| :- | :- | :- |
|BASIC|286710|9616.3|
|BASIC\_PIPELINED 1|20500|27570.3|
|BASIC\_PIPELINED 2|40990|21073.2|
|BASIC\_PIPELINED 3|61470|18259.0|
|DPA|122870|13196.9|
|DPA\_PIPELINED 1|20500|14030.9|
|DPA\_PIPELINED 2|40980|16005.7|
|DPA\_PIPELINED 3|61460|15732.0|


