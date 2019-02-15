library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use std.textio.all;

entity tb_ppm_capture_fsm is
port
(
  my_in : in std_logic -- input needed to keep modelsim from complainning???
);
end tb_ppm_capture_fsm;

architecture rtl of tb_ppm_capture_fsm is

----------------------------------------------
--       Component declarations             --
----------------------------------------------

-- Device under test

component ppm_capture_fsm
port
(
  ppm_input, CLK : in std_logic;
	ppm_cap_resetn : in std_logic;
	write_en : out std_logic;
	cycle_count : out std_logic_vector(31 downto 0);
	write_addr : out std_logic_vector(2 downto 0);
	end_of_frame : out std_logic
);
end component ppm_capture_fsm;

----------------------------------------------
--          Signal declarations             --
----------------------------------------------

type my_input_states is (START, IDLE, GAP, CHAN1, CHAN2, CHAN3, 
                        CHAN4, CHAN5, CHAN6, STOP_TEST);

signal input_state    : my_input_states;  -- Direct which input vector to use

signal clk : std_logic;
signal reset : std_logic;
signal ppm_input_sig : std_logic;

signal write_en_sig : std_logic;
signal end_of_frame_sig : std_logic;
signal write_addr_sig : std_logic_vector(2 downto 0);
signal cycle_count_sig : std_logic_vector(31 downto 0);

-- support for writing outputs to file

begin

-- Processes

-------------------------------------------
-------------------------------------------
-- Process Name: system_clk_gen          --
--                                       --
-- Description: Generat clock to run the --
-- simulation.                           --
--                                       --
--                                       --
-------------------------------------------
-------------------------------------------  
system_clk_gen : process   -- 100 MHz clock
begin
  clk <= '0';
  wait for 10 ns;
    loop
      wait for 5 ns;
      clk <= '1';
      wait for 5 ns;
      clk <= '0';
    end loop;
end process system_clk_gen;

-------------------------------------------
-------------------------------------------
-- Process Name: toggle_reset            --
--                                       --
-- Description: Toggle system reset.     --
-- used if DUT requires a reset signal   --
--                                       --
--                                       --
-------------------------------------------
-------------------------------------------  
toggle_reset : process
begin
  reset <= '0'; -- place circuit in reset
  wait for 95 ns;
  reset <= '1'; 
  wait;
end process toggle_reset;

------------------------------------------------------------
------------------------------------------------------------
--                                                        --
-- Process Name: DUT stimulus                             --
--                                                        --
-- Send inputs to dut. Holds inputs for HOLD_INPUT_reg    --
--  clk cycles                                            --
--                                                        --
------------------------------------------------------------
------------------------------------------------------------
DUT_stimulus : process
	file file_handler     : text open write_mode is "tb_ppm_cap_output.txt";
	variable row          : line;
begin
  --if (clk = '1' and clk'event) then

    -- Initialize the test
    if(reset = '0') then
      input_state <= START;
      reset <= '1';
    else

      -- Cycle thought DUT input stimulus patteren
      case input_state is
      -- start state with 0.1 IDLE
      when START =>
        ppm_input_sig <= '1';
        wait for 100000 ns; -- 0.1 ms
        -- gap
        ppm_input_sig <= '0';
        wait for 400000 ns; -- 0.4 ms
        input_state <= CHAN1;

      when IDLE =>
        ppm_input_sig <= '1';
        wait for 9300000 ns; -- 9.3 ms
        -- gap
        ppm_input_sig <= '0';
        -- verify outputs
        write(row, conv_integer(write_en_sig), right, 15);
        write(row, conv_integer(cycle_count_sig), right, 15);
        write(row, conv_integer(write_addr_sig), right, 15);
        write(row, conv_integer(end_of_frame_sig), right, 15);
        --Write line to the file
        writeline(file_handler ,row);
        wait for 400000 ns; -- 0.4 ms
        input_state <= CHAN1;

      when CHAN1 =>
        ppm_input_sig <= '1';
        wait for 1500000 ns; -- 1.5 ms
        -- fake gap
        ppm_input_sig <= '0';        
        -- verify outputs
        write(row, conv_integer(write_en_sig), right, 15);
        write(row, conv_integer(cycle_count_sig), right, 15);
        write(row, conv_integer(write_addr_sig), right, 15);
        write(row, conv_integer(end_of_frame_sig), right, 15);
        --Write line to the file
        writeline(file_handler ,row);
        wait for 400000 ns; -- 0.4 ms
        input_state <= CHAN2;

      when CHAN2 =>
        ppm_input_sig <= '1';
        wait for 800000 ns; -- 0.8 ms
        -- fake gap
        ppm_input_sig <= '0';
        -- verify outputs
        write(row, conv_integer(write_en_sig), right, 15);
        write(row, conv_integer(cycle_count_sig), right, 15);
        write(row, conv_integer(write_addr_sig), right, 15);
        write(row, conv_integer(end_of_frame_sig), right, 15);
        --Write line to the file
        writeline(file_handler ,row);
        wait for 400000 ns; -- 0.4 ms
        input_state <= CHAN3;

      when CHAN3 =>
        ppm_input_sig <= '1';
        wait for 1000000 ns; -- 1 ms
        -- fake gap
        ppm_input_sig <= '0';
        -- verify outputs
        write(row, conv_integer(write_en_sig), right, 15);
        write(row, conv_integer(cycle_count_sig), right, 15);
        write(row, conv_integer(write_addr_sig), right, 15);
        write(row, conv_integer(end_of_frame_sig), right, 15);
        --Write line to the file
        writeline(file_handler ,row);
        wait for 400000 ns; -- 0.4 ms
        input_state <= CHAN4;

      when CHAN4 =>
        ppm_input_sig <= '1';
        wait for 1250000 ns; -- 1.25 ms
        -- fake gap
        ppm_input_sig <= '0';
        -- verify outputs
        write(row, conv_integer(write_en_sig), right, 15);
        write(row, conv_integer(cycle_count_sig), right, 15);
        write(row, conv_integer(write_addr_sig), right, 15);
        write(row, conv_integer(end_of_frame_sig), right, 15);
        --Write line to the file
        writeline(file_handler ,row);
        wait for 400000 ns; -- 0.4 ms
        input_state <= CHAN5;

      when CHAN5 =>
        ppm_input_sig <= '1';
        wait for 2000000 ns; -- 2 ms
        -- fake gap
        ppm_input_sig <= '0';
        -- verify outputs
        write(row, conv_integer(write_en_sig), right, 15);
        write(row, conv_integer(cycle_count_sig), right, 15);
        write(row, conv_integer(write_addr_sig), right, 15);
        write(row, conv_integer(end_of_frame_sig), right, 15);
        --Write line to the file
        writeline(file_handler ,row);
        wait for 400000 ns; -- 0.4 ms
        input_state <= CHAN6;

      when CHAN6 =>
        ppm_input_sig <= '1';
        wait for 1750000 ns; -- 1.75 ms
        -- fake gap
        ppm_input_sig <= '0';
        -- verify outputs
        write(row, conv_integer(write_en_sig), right, 15);
        write(row, conv_integer(cycle_count_sig), right, 15);
        write(row, conv_integer(write_addr_sig), right, 15);
        write(row, conv_integer(end_of_frame_sig), right, 15);
        --Write line to the file
        writeline(file_handler ,row);
        wait for 400000 ns; -- 0.4 ms
        input_state <= IDLE;

      when STOP_TEST =>

        ppm_input_sig <= '0';
        input_state <= STOP_TEST;
         
      when OTHERS =>

        input_state <= STOP_TEST;
        
      end case;  
    end if;
 -- end if;
end process DUT_stimulus;



-- Combinational assignments

  -- none

-- Connect DUT (PPM Capture circuit) to the testbench

my_dut : ppm_capture_fsm
port map
(
  CLK => clk,
  ppm_input => ppm_input_sig,
  ppm_cap_resetn => reset,  
  write_en => write_en_sig,
  cycle_count => cycle_count_sig,
  write_addr => write_addr_sig,
  end_of_frame => end_of_frame_sig 
);

end rtl;