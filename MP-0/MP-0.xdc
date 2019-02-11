set_property PACKAGE_PIN AA19 [get_ports {vid_hsync}];  # VGA-HSync
set_property PACKAGE_PIN Y19  [get_ports {vid_vsync}];  # VGA-VSync
set_property PACKAGE_PIN V20  [get_ports {vid_data[0]}];  # VGA-R1
set_property PACKAGE_PIN U20  [get_ports {vid_data[1]}];  # VGA-R2
set_property PACKAGE_PIN V19  [get_ports {vid_data[2]}];  # VGA-R3
set_property PACKAGE_PIN V18  [get_ports {vid_data[3]}];  # VGA-R4
set_property PACKAGE_PIN AB22 [get_ports {vid_data[4]}];  # VGA-G1
set_property PACKAGE_PIN AA22 [get_ports {vid_data[5]}];  # VGA-G2
set_property PACKAGE_PIN AB21 [get_ports {vid_data[6]}];  # VGA-G3
set_property PACKAGE_PIN AA21 [get_ports {vid_data[7]}];  # VGA-G4
set_property PACKAGE_PIN Y21  [get_ports {vid_data[8]}];  # VGA-B1
set_property PACKAGE_PIN Y20  [get_ports {vid_data[9]}];  # VGA-B2
set_property PACKAGE_PIN AB20 [get_ports {vid_data[10]}];  # VGA-B3
set_property PACKAGE_PIN AB19 [get_ports {vid_data[11]}];  # VGA-B4

# DEBUGGING
set_property PACKAGE_PIN T22 [get_ports {axi_lite_write[0]}];  # "LD0"
set_property PACKAGE_PIN T21 [get_ports {axi_lite_read[0]}];  # "LD1"
set_property PACKAGE_PIN U22 [get_ports {locked}];  # "LD2"
set_property PACKAGE_PIN U21 [get_ports {overflow}];  # "LD3"
set_property PACKAGE_PIN V22 [get_ports {underflow}];  # "LD4"

set_property PACKAGE_PIN Y11  [get_ports {clk_100Mhz}];  # "JA1"
set_property PACKAGE_PIN AA11 [get_ports {fclk_reset0_n}];  # "JA2"
set_property PACKAGE_PIN Y10  [get_ports {peripherial_aresetn[0]}];  # "JA3"
set_property PACKAGE_PIN AA9  [get_ports {clk_25Mhz}];  # "JA4"
set_property PACKAGE_PIN AB11 [get_ports {interconnect_aresetn[0]}];  # "JA7"
set_property PACKAGE_PIN AB10 [get_ports {vid_vsync1}];  # "JA8"
set_property PACKAGE_PIN AB9  [get_ports {vid_hsync1}];  # "JA9"
set_property PACKAGE_PIN AA8  [get_ports {eol}];  # "JA10"

set_property PACKAGE_PIN W12 [get_ports {active_video_out}];  # "JB1"
set_property PACKAGE_PIN W11 [get_ports {hblank_out}];  # "JB2"
set_property PACKAGE_PIN V10 [get_ports {hsync_out}];  # "JB3"
set_property PACKAGE_PIN W8 [get_ports {vblank_out}];  # "JB4"
set_property PACKAGE_PIN V12 [get_ports {vsync_out}];  # "JB7"
set_property PACKAGE_PIN W10 [get_ports {vtg_ce}];  # "JB8"
set_property PACKAGE_PIN V9 [get_ports {sof}];  # "JB9"
set_property PACKAGE_PIN V8 [get_ports {vtc_resetn}];  # "JB10"

# ----------------------------------------------------------------------------
# IOSTANDARD Constraints
#
# Note that these IOSTANDARD constraints are applied to all IOs currently
# assigned within an I/O bank.  If these IOSTANDARD constraints are 
# evaluated prior to other PACKAGE_PIN constraints being applied, then 
# the IOSTANDARD specified will likely not be applied properly to those 
# pins.  Therefore, bank wide IOSTANDARD constraints should be placed 
# within the XDC file in a location that is evaluated AFTER all 
# PACKAGE_PIN constraints within the target bank have been evaluated.
#
# Un-comment one or more of the following IOSTANDARD constraints according to
# the bank pin assignments that are required within a design.
# ---------------------------------------------------------------------------- 

# Note that the bank voltage for IO Bank 33 is fixed to 3.3V on ZedBoard. 
set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 33]];

# Note that the bank voltage for IO Bank 13 is fixed to 3.3V on ZedBoard. 
set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 13]];