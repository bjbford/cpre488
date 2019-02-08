-- library declaration
library IEEE;
use IEEE.std_logic_1164.all;
-- entity
entity ppm_gen is
port ( X, CLK : in std_logic;
	RESET : in std_logic;
	Z : out std_logic;
end ppm_gen;
-- architecture
architecture ppm_gen of ppm_gen is
	type state_type is (a, b, c);
	signal PS, NS : state_type;
	begin
	sync_proc: process(CLK, NS, RESET)
		begin
		if (RESET = '0') then PS <= a;
		elsif (rising_edge(CLK)) then PS <= NS;
		end if;
	end process sync_proc;
	comb_proc: process(PS, X)
	begin
		case PS is
			when a =>
				if (X='0') then NS<=b; Z<='0';
				else NS <= a; Z <= '0';
				end if;
			when b =>
				if (X='0') then NS<=b; Z<='0';
				else NS <= c; Z <= '0';
				end if;
			when c =>
				if (X='0') then NS<=b; Z<='0';
				else NS <= c; Z <= '1';
				end if;
			when others =>
				Z <= '0'; NS<=a;
		end case;
	end process comb_proc;
end ppm_gen;