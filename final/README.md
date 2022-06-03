In the project, you can download these file to riscv platform and run the following instructions:

For single core:
  $ cd $EE6470/riscv-vp/vp/build

  $ make install

  $ cd $EE6470/riscv-vp/sw/basic-DCT-single

  $ make

  $ make sim
  
For multiple core:
  $ cd $EE6470/riscv-vp/vp/build

  $ make install

  $ cd $EE6470/riscv-vp/sw/basic-DCT

  $ make

  $ make sim
  
If you want to run the stratus, you can run the following instructions:
  $ cd /DCT_stratus/stratus
  
  $ make sim_B
  
  $ make sim_V_BASIC
  
  $ make sim_V_DPA
