# PPM Input = PMODA pin 1
set_property PACKAGE_PIN Y11  [get_ports {PPM_Input}];  # "JA1"
# PPM Output = PMODB pin 1
set_property PACKAGE_PIN W12 [get_ports {PPM_Output}];  # "JB1"

# Note that the bank voltage for IO Bank 13 is fixed to 3.3V on ZedBoard. 
set_property IOSTANDARD LVCMOS33 [get_ports -of_objects [get_iobanks 13]];

# Set the bank voltage for IO Bank 34 to 1.8V by default.
set_property IOSTANDARD LVCMOS18 [get_ports -of_objects [get_iobanks 34]];