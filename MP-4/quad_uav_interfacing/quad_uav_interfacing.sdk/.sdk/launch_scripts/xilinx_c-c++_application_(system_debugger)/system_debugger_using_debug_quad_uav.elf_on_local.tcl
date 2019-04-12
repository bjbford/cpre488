connect -url tcp:127.0.0.1:3121
source U:/cpre488/Labs/MP-4/quad_uav_interfacing/quad_uav_interfacing.sdk/quad_uav_interfacing_wrapper_hw_platform_0/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Platform Cable USB II 000018cd581101"} -index 0
loadhw -hw U:/cpre488/Labs/MP-4/quad_uav_interfacing/quad_uav_interfacing.sdk/quad_uav_interfacing_wrapper_hw_platform_0/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Platform Cable USB II 000018cd581101"} -index 0
stop
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Platform Cable USB II 000018cd581101"} -index 0
rst -processor
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Platform Cable USB II 000018cd581101"} -index 0
dow U:/cpre488/Labs/MP-4/quad_uav_interfacing/quad_uav_interfacing.sdk/quad_uav/Debug/quad_uav.elf
configparams force-mem-access 0
bpadd -addr &main
