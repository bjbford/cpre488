-- library declaration
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;  
-- entity
entity ppm_read is
port ( X, CLK : in std_logic;
	ppm_read_reset : in std_logic;
	channel_count, count_enable : out std_logic;
	cycle_count : out std_logic_vector(31 downto 0);
	channel_number : out std_logic_vector(2 downto 0);
end ppm_read;

-- entity register32bit is 
-- port(reg_in   : in std_logic_vector(31 downto 0);
    -- load  : in std_logic;
    -- reset : in std_logic;
    -- clk : in std_logic;
    -- reg_val   : out std_logic_vector(31 downto 0)
-- );
-- end register32bit;

-- architecture reg32bit of register32bit is
-- begin
    -- process(clk, reset)
    -- begin
		-- reg_change <= reset&load;
        -- if rising_edge(clk) then
			-- case reg_change is
				-- when "10" | "11" then
					-- reg_val <= x"00000000";
				-- when "01" then
					-- reg_val <= reg_in;
				-- when "00" then
					-- reg_val <= reg_val;
				-- when others
					-- reg_val <= '1';
			-- end case
        -- end if;
    -- end process;
-- end reg32bit;

entity state_reset_counter is 
port(inc   : in std_logic;
    reset : in std_logic;
    clk : in std_logic;
    channel_reset   : out std_logic;
	count : out std_logic_vector (2 downto 0)
);
end state_reset_counter;

architecture reset_counter of state_reset_counter is
begin
    process(clk, reset, inc)
    begin
		channel_reset <= '1';
		sel <= reset&inc;
        case sel is
			when "00" | "01" =>
				count  <= "000";
			when "11" =>
				count <= count + 1;
			when "10" =>
				channel_reset <= '1';
			when others =>
				channel_reset <='0';
		end case
		if(count = "111") then
			count <= "000";
			channel_reset <= '0';
		else channel_reset <= '1';
		end if
    end process;
end reset_counter;

-- architecture
architecture ppm_read of ppm_read is
	type state_type is (a, b, c);
	signal PS, NS : state_type;
	signal count_temp : std_logic_vector(2 downto 0);
	signal chan_reset_loop : std_logic;
	
	begin
    channel_tracker : state_reset_counter port map(channel_count, chan_reset_loop, CLK, chan_reset_loop, count_temp);
	ppm_read_reset <= chan_reset_loop;
	channel_number <= count_temp;
	
	sync_proc: process(CLK, NS, ppm_read_reset)
		begin
		if (ppm_read_reset = '0') then PS <= a;
		elsif (rising_edge(CLK)) then PS <= NS;
		end if;
	end process sync_proc;
	comb_proc: process(PS, X)
		begin
		count_enable <= '0';
		case PS is
			when a =>
				count_enable <= '0';
				if (X='0') then NS<=b; channel_count <= '0';
				else NS <= a; channel_count <= '0';
				end if;
			when b =>
				count_enable <= '0'; 
				cycle_count <= x"00000000";
				if (X='0') then NS<=b; channel_count <= '0';
				else NS <= c; channel_count <= '1';
				end if;
			when c =>
				count_enable <= '1';
				if (X='0') then NS<=b; channel_count <= '0';
				else NS <= c;  channel_count <= '0';
				end if;
			when others =>
				channel_count <= '0'; count_enable <= '0'; NS<=a;
		end case;
		if (count_enable = '1') then
			cycle_count <= cycle_count + 1;
		end if
	end process comb_proc;
end ppm_read;