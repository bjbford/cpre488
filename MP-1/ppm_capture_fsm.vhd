-- entity state_reset_counter is 
-- port (inc, resetn, cnt_clk : in std_logic;
	-- channel_resetn, eof : out std_logic;
	-- count : out std_logic_vector (2 downto 0));
-- end state_reset_counter;

-- architecture reset_counter of state_reset_counter is
-- signal sel : std_logic_vector(1 downto 0);
-- signal count_sig : std_logic_vector(2 downto 0);
-- signal debug : std_logic;
-- begin
    -- process(cnt_clk, resetn, inc)
    -- begin
	-- count <= count_sig;
	-- count_sig <= "000";
	-- channel_resetn <= '1';
	-- debug <= '0';
	-- sel <= resetn&inc;
	-- if(rising_edge(cnt_clk)) then
		-- eof <= '0';
		-- case sel is
			-- when "00" | "01" =>
				-- count_sig <= "000";
			-- when "11" =>
				-- count_sig <= std_logic_vector(unsigned(count_sig) + 1);
				-- debug <= '1';
			-- when "10" =>
				-- channel_resetn <= '1';
			-- when others =>
				-- channel_resetn <= '0';
		-- end case;
	-- end if;
	-- if(count_sig = "111") then
		-- count_sig <= "000";
		-- channel_resetn <= '0';
		-- eof <= '1';
	-- else channel_resetn <= '1';
	-- end if;
    -- end process;
-- end reset_counter;

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

-- entity
entity ppm_capture is
port (ppm_input, CLK : in std_logic;
	ppm_cap_resetn : in std_logic;
	write_en : out std_logic;
	cycle_count : out std_logic_vector(31 downto 0);
	write_addr : out std_logic_vector(2 downto 0);
	end_of_frame : out std_logic);
end ppm_capture;

-- architecture
architecture ppm_capture of ppm_capture is
	type state_type is (a, b, c);
	-- should we initialize unknowns here (instead of on hard reset)
	signal PS, NS : state_type;
	signal state_resetn : std_logic;
	signal count_en_sig, channel_count_sig : std_logic;
	signal write_addr_sig : std_logic_vector(2 downto 0);
	signal cycle_count_sig : std_logic_vector(31 downto 0);
	
	begin
	cycle_count <= cycle_count_sig;
	write_addr <= write_addr_sig;
	
	sync_proc: process(CLK)
		begin
		if (ppm_cap_resetn = '0')then	PS <= a;
		elsif (rising_edge(CLK)) then PS <= NS;
		end if;
	end process sync_proc;
	
	comb_proc: process(PS, ppm_input, state_resetn)
		begin
		--default value assignments
		NS <= a;
		channel_count_sig <= '0';
		count_en_sig <= '0';
		write_en <= '0';
		case PS is
			when a =>
				count_en_sig <= '0';
				if (ppm_input='0') then NS<=b; channel_count_sig <= '0'; write_en <= '0';
				else NS <= a; channel_count_sig <= '0'; write_en <= '0';
				end if;
			when b =>
				count_en_sig <= '0'; 
				if (ppm_input='0') then NS<=b; channel_count_sig <= '0'; write_en <= '0';
				else NS <= c; channel_count_sig <= '1'; write_en <= '0';
				end if;
			when c =>
				count_en_sig <= '1';
				if (ppm_input='0') then NS<=b; channel_count_sig <= '0'; write_en <= '1';
				elsif (state_resetn = '0') then NS <= a; channel_count_sig <= '0'; write_en <= '0';
				else NS <= c;  channel_count_sig <= '0'; write_en <= '0';
				end if;
			when others =>
				channel_count_sig <= '0'; count_en_sig <= '0'; write_en <= '0'; NS <= a;
		end case;
	end process comb_proc;
	
	cycle_proc: process(CLK)
		begin
		if(rising_edge(CLK)) then
			if(count_en_sig = '1') then cycle_count_sig <= std_logic_vector(unsigned(cycle_count_sig) + 1);
			else cycle_count_sig <= (others => '0');
			end if;
		end if;
	end process cycle_proc;
	
	channel_proc: process(CLK)
		begin
		if(rising_edge(CLK)) then
			if(ppm_cap_resetn = '0') then
				write_addr_sig <= (others => '0'); end_of_frame <= '0';	state_resetn <= '1'; 
			else
				if(write_addr_sig = "111") then
					write_addr_sig <= (others => '0');
					end_of_frame <= '1';
					state_resetn <= '0';
				elsif(channel_count_sig = '1') then write_addr_sig <= std_logic_vector(unsigned(write_addr_sig) + 1); end_of_frame <= '0'; state_resetn <= '1';
				else end_of_frame <= '0'; state_resetn <= '1';
				end if;
			end if;
		end if;
	end process channel_proc;
	
end ppm_capture;


