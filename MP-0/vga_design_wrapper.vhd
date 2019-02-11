--Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2018.3 (win64) Build 2405991 Thu Dec  6 23:38:27 MST 2018
--Date        : Mon Feb  4 12:52:21 2019
--Host        : CO2041-11 running 64-bit major release  (build 9200)
--Command     : generate_target vga_design_wrapper.bd
--Design      : vga_design_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity vga_design_wrapper is
  port (
    DDR_addr : inout STD_LOGIC_VECTOR ( 14 downto 0 );
    DDR_ba : inout STD_LOGIC_VECTOR ( 2 downto 0 );
    DDR_cas_n : inout STD_LOGIC;
    DDR_ck_n : inout STD_LOGIC;
    DDR_ck_p : inout STD_LOGIC;
    DDR_cke : inout STD_LOGIC;
    DDR_cs_n : inout STD_LOGIC;
    DDR_dm : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dq : inout STD_LOGIC_VECTOR ( 31 downto 0 );
    DDR_dqs_n : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dqs_p : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_odt : inout STD_LOGIC;
    DDR_ras_n : inout STD_LOGIC;
    DDR_reset_n : inout STD_LOGIC;
    DDR_we_n : inout STD_LOGIC;
    FIXED_IO_ddr_vrn : inout STD_LOGIC;
    FIXED_IO_ddr_vrp : inout STD_LOGIC;
    FIXED_IO_mio : inout STD_LOGIC_VECTOR ( 53 downto 0 );
    FIXED_IO_ps_clk : inout STD_LOGIC;
    FIXED_IO_ps_porb : inout STD_LOGIC;
    FIXED_IO_ps_srstb : inout STD_LOGIC;
    active_video_out : out STD_LOGIC;
    axi_lite_read : out STD_LOGIC_VECTOR ( 0 to 0 );
    axi_lite_write : out STD_LOGIC_VECTOR ( 0 to 0 );
    btns_5bits_tri_i : in STD_LOGIC_VECTOR ( 4 downto 0 );
    clk_100Mhz : out STD_LOGIC;
    clk_25Mhz : out STD_LOGIC;
    eol : out STD_LOGIC;
    fclk_reset0_n : out STD_LOGIC;
    hblank_out : out STD_LOGIC;
    hsync_out : out STD_LOGIC;
    interconnect_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 );
    locked : out STD_LOGIC;
    overflow : out STD_LOGIC;
    peripherial_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 );
    sof : out STD_LOGIC_VECTOR ( 0 to 0 );
    sws_8bits_tri_i : in STD_LOGIC_VECTOR ( 7 downto 0 );
    underflow : out STD_LOGIC;
    vblank_out : out STD_LOGIC;
    vid_data : out STD_LOGIC_VECTOR ( 11 downto 0 );
    vid_hsync : out STD_LOGIC;
    vid_hsync1 : out STD_LOGIC;
    vid_vsync : out STD_LOGIC;
    vid_vsync1 : out STD_LOGIC;
    vsync_out : out STD_LOGIC;
    vtc_resetn : out STD_LOGIC_VECTOR ( 0 to 0 );
    vtg_ce : out STD_LOGIC
  );
end vga_design_wrapper;

architecture STRUCTURE of vga_design_wrapper is
  component vga_design is
  port (
    axi_lite_write : out STD_LOGIC_VECTOR ( 0 to 0 );
    axi_lite_read : out STD_LOGIC_VECTOR ( 0 to 0 );
    clk_100Mhz : out STD_LOGIC;
    clk_25Mhz : out STD_LOGIC;
    fclk_reset0_n : out STD_LOGIC;
    active_video_out : out STD_LOGIC;
    hblank_out : out STD_LOGIC;
    hsync_out : out STD_LOGIC;
    vblank_out : out STD_LOGIC;
    vsync_out : out STD_LOGIC;
    interconnect_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 );
    peripherial_aresetn : out STD_LOGIC_VECTOR ( 0 to 0 );
    locked : out STD_LOGIC;
    overflow : out STD_LOGIC;
    underflow : out STD_LOGIC;
    vid_data : out STD_LOGIC_VECTOR ( 11 downto 0 );
    vid_hsync : out STD_LOGIC;
    vid_vsync : out STD_LOGIC;
    vtg_ce : out STD_LOGIC;
    sof : out STD_LOGIC_VECTOR ( 0 to 0 );
    vtc_resetn : out STD_LOGIC_VECTOR ( 0 to 0 );
    vid_vsync1 : out STD_LOGIC;
    vid_hsync1 : out STD_LOGIC;
    eol : out STD_LOGIC;
    DDR_cas_n : inout STD_LOGIC;
    DDR_cke : inout STD_LOGIC;
    DDR_ck_n : inout STD_LOGIC;
    DDR_ck_p : inout STD_LOGIC;
    DDR_cs_n : inout STD_LOGIC;
    DDR_reset_n : inout STD_LOGIC;
    DDR_odt : inout STD_LOGIC;
    DDR_ras_n : inout STD_LOGIC;
    DDR_we_n : inout STD_LOGIC;
    DDR_ba : inout STD_LOGIC_VECTOR ( 2 downto 0 );
    DDR_addr : inout STD_LOGIC_VECTOR ( 14 downto 0 );
    DDR_dm : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dq : inout STD_LOGIC_VECTOR ( 31 downto 0 );
    DDR_dqs_n : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dqs_p : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    FIXED_IO_mio : inout STD_LOGIC_VECTOR ( 53 downto 0 );
    FIXED_IO_ddr_vrn : inout STD_LOGIC;
    FIXED_IO_ddr_vrp : inout STD_LOGIC;
    FIXED_IO_ps_srstb : inout STD_LOGIC;
    FIXED_IO_ps_clk : inout STD_LOGIC;
    FIXED_IO_ps_porb : inout STD_LOGIC;
    btns_5bits_tri_i : in STD_LOGIC_VECTOR ( 4 downto 0 );
    sws_8bits_tri_i : in STD_LOGIC_VECTOR ( 7 downto 0 )
  );
  end component vga_design;
begin
vga_design_i: component vga_design
     port map (
      DDR_addr(14 downto 0) => DDR_addr(14 downto 0),
      DDR_ba(2 downto 0) => DDR_ba(2 downto 0),
      DDR_cas_n => DDR_cas_n,
      DDR_ck_n => DDR_ck_n,
      DDR_ck_p => DDR_ck_p,
      DDR_cke => DDR_cke,
      DDR_cs_n => DDR_cs_n,
      DDR_dm(3 downto 0) => DDR_dm(3 downto 0),
      DDR_dq(31 downto 0) => DDR_dq(31 downto 0),
      DDR_dqs_n(3 downto 0) => DDR_dqs_n(3 downto 0),
      DDR_dqs_p(3 downto 0) => DDR_dqs_p(3 downto 0),
      DDR_odt => DDR_odt,
      DDR_ras_n => DDR_ras_n,
      DDR_reset_n => DDR_reset_n,
      DDR_we_n => DDR_we_n,
      FIXED_IO_ddr_vrn => FIXED_IO_ddr_vrn,
      FIXED_IO_ddr_vrp => FIXED_IO_ddr_vrp,
      FIXED_IO_mio(53 downto 0) => FIXED_IO_mio(53 downto 0),
      FIXED_IO_ps_clk => FIXED_IO_ps_clk,
      FIXED_IO_ps_porb => FIXED_IO_ps_porb,
      FIXED_IO_ps_srstb => FIXED_IO_ps_srstb,
      active_video_out => active_video_out,
      axi_lite_read(0) => axi_lite_read(0),
      axi_lite_write(0) => axi_lite_write(0),
      btns_5bits_tri_i(4 downto 0) => btns_5bits_tri_i(4 downto 0),
      clk_100Mhz => clk_100Mhz,
      clk_25Mhz => clk_25Mhz,
      eol => eol,
      fclk_reset0_n => fclk_reset0_n,
      hblank_out => hblank_out,
      hsync_out => hsync_out,
      interconnect_aresetn(0) => interconnect_aresetn(0),
      locked => locked,
      overflow => overflow,
      peripherial_aresetn(0) => peripherial_aresetn(0),
      sof(0) => sof(0),
      sws_8bits_tri_i(7 downto 0) => sws_8bits_tri_i(7 downto 0),
      underflow => underflow,
      vblank_out => vblank_out,
      vid_data(11 downto 0) => vid_data(11 downto 0),
      vid_hsync => vid_hsync,
      vid_hsync1 => vid_hsync1,
      vid_vsync => vid_vsync,
      vid_vsync1 => vid_vsync1,
      vsync_out => vsync_out,
      vtc_resetn(0) => vtc_resetn(0),
      vtg_ce => vtg_ce
    );
end STRUCTURE;
