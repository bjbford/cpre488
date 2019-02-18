library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;

entity debounce_button is
  generic(
    counter_size  :  integer := 20); --counter size (20 bits gives 10.5ms with 100MHz clock)
  port(
    clk     : in  std_logic;  --input clock
    button  : in  std_logic;  --input signal to be debounced
    result  : out std_logic); --debounced signal
end debounce_button;

architecture logic of debounce_button is
  --input flip flops
  signal flipflops   : std_logic_vector(1 downto 0);
  --sync reset to zero
  signal counter_set : std_logic;
  --counter output
  signal counter_out : std_logic_vector(counter_size downto 0) := (others => '0');
begin

  --determine when to start/reset counter
  counter_set <= flipflops(0) xor flipflops(1);

  process(clk)
  begin
    if(clk'event and clk = '1') then
      flipflops(0) <= button;
      flipflops(1) <= flipflops(0);
      --reset counter because input is changing
      if(counter_set = '1') then
        counter_out <= (others => '0');
      --stable input time is not yet met
      elsif(counter_out(counter_size) = '0') then
        counter_out <= counter_out + 1;
      --stable input time is met
      else
        result <= flipflops(1);
      end if;    
    end if;
  end process;
end logic;
