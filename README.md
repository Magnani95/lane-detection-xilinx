# lane-detection-xilinx
Lane detection system as key building block for Autonomous Driving (AD) on F1Tenth cars. The lane detection is accelerated in FPGA using AMD Vitis Vision libraries.
The project is structured as follows:

- hw/hls: contains the HLS code of the FPGA-accelerated computer vision kernels;
- hw/vivado: contains the .tcl to build the hardware design for AMD Kria KV260 and the exported hardware (.xsa and .bit);
- sw: the Linux software application for lane detection that uses FPGA acceleration. This can be compiled directly on the target (AMD Kria KV260).

## How to run the demo
1) use a AMD Kria KV260 running a Linux distribution (use for example PetaLinux or pre-compiled Linux images);
2) program the board using the provided .bit file using xsct (target the board, then xsct> fpga hw/vivado/lane_detection_fpga.bit);
3) transfer the application code to the target platform (the content of sw/);
4) compile the application:

```console
$ cd sw
$ mkdir build
$ cd build
$ cmake ..
$ make -j4
```

Make sure to use the SSH flags to forward the graphical interface on the board (ssh -CX root@<ip.of.the.kria>)

```console
$ ./main ../data/00-prj.webm
```

## How to build the hardware
The steps to build the hardware are:

1) create a Vitis HLS project for each computer vision kernel contained in hw/hls;
2) create the vivado block design using the provided .tcl script (i.e. source the environment, then vivado -source hw/vivado/lane_detection_fpga.tcl);
3) launch the implementation and after that export the hardware design;