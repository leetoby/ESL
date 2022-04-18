EE6470 Electronic System Level Design and Synthesis

Midterm Project

Name: LEE YU-CHOU	ID: 107061144

- Repository URL: <https://github.com/leetoby/ESL/tree/main/midterm>

- Algorithm: quadratic formula
- Goal:
  - Given a, b, and c, solve the equation a\*x^2+b\*x+c=0. There exists two solutions x1 and x2 if b^2-4\*a\*c>=0.
- Input:
  - a <7:0>, b <7:0>, c<7:0>
- Output:
  - x1 <12:0>, x2 <12:0>
- Problem in this project:
  - Need a loop to calculate the square root.
  - Choose appropriate bit size for each variable in the process.
- Testbench task:
  - Get x1 and x2 to find whether |ϵ|=|a\*x1^2+b\*x1+c| is less than 2.
- Results using HLS using different directives

||**Run Time**|**Area**|
| :- | :- | :- |
|BASIC|136314870 ns|6129.2|
|BASIC CONSTRAIN LATENCY 1|47185910 ns|6118.3|
|BASIC CONSTRAIN LATENCY 2|76021750 ns|6849.3|
|BASIC CONSTRAIN LATENCY 3|107479030 ns|6126.5|
|BASIC PIPELINED 1|2621490 ns (Max throughput)|15187.0|
|BASIC UNROLL “SQRT LOOP”|81264630 ns|6264.0|
|DPA|70778870 ns|7140.2|
|DPA CONSTRAIN LATENCY 1|44564470 ns|6516.1|
|DPA CONSTRAIN LATENCY 2|73400310 ns|6722.7|
|DPA CONSTRAIN LATENCY 3|73400310 ns|6722.7|
|DPA PIPELINED 1|2621480 ns (Max throughput)|13607.4|
|DPA UNROLL “SQRT LOOP”|34078710 ns|7629.0|

