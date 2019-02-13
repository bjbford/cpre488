

-- library IEEE;
-- use IEEE.std_logic_1164.all;
-- use IEEE.numeric_std.all;  

-- entity cycle_decrementer is 
-- port(resetn, clk, enable, load : in std_logic;
	-- start_val : in std_logic_vector (31 downto 0);
	-- count : out std_logic_vector (31 downto 0)
-- );
-- end cycle_decrementer;

-- architecture decrementer of cycle_decrementer is
-- signal sel: std_logic_vector(1 downto 0);
-- signal count_sig : std_logic_vector(31 downto 0);
-- begin
    -- process(clk, resetn, enable, load)
    -- begin
		-- sel <= resetn&enable;
		-- count <= count_sig;
		-- if(rising_edge(clk)) then
			-- if(load = '1') then count <= start_val;
			-- else
				-- case sel is
					-- when "00" | "01" =>
						-- --reset to clock cycles in gap
						-- count_sig <= x"FFFFFFFF";
					-- when "11" =>
						-- if(count_sig = x"00000000") then count_sig <= x"00000000";
						-- else count_sig <= std_logic_vector(unsigned(count_sig) - 1);
						-- end if;
					-- when "10" =>
						-- count_sig <= count_sig;
					-- when others =>
						-- count_sig <= x"00000000";
				-- end case;
			-- end if;
		-- end if;
    -- end process;
-- end decrementer;

-- library declaration
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

-- entity
entity ppm_gen is
port (CLK, gen_en : in std_logic;
	ppm_gen_resetn : in std_logic;
	inc_cycle_count : in std_logic_vector(31 downto 0);
	read_addr : out std_logic_vector(2 downto 0);
	ppm_output : out std_logic);
end ppm_gen;

-- architecture
architecture ppm_gen of ppm_gen is
	type state_type is (idle, gap, chan);
	signal PS, NS : state_type;
	signal gen_resetn_sig, decrement_en, decrement_resetn, gap_en, gap_resetn, channel_count_en, frame_start : std_logic;
	signal gap_done, channel_done, frame_running : std_logic := '0';
	signal frame_done : std_logic := '1';
	signal addr_sig :  std_logic_vector(2 downto 0) := "001";
	signal gap_val : unsigned(15 downto 0) := x"9C40";
	signal decrement_val : unsigned(31 downto 0) := x"FFFFFFFF";
	signal frame_val : unsigned(31 downto 0) := x"001E8480";

	begin
	gen_resetn_sig <= ppm_gen_resetn;
	read_addr <= addr_sig;
	frame_start <= gen_en and (not frame_running);
	
	sync_proc: process(CLK, NS, ppm_gen_resetn, gen_resetn_sig)
		begin
		--block should receive reset when channel is idle
		if (gen_resetn_sig = '0') then PS <= idle;
		elsif (rising_edge(CLK)) then PS <= NS;
		end if;
	end process sync_proc;
	
	comb_proc: process(PS, gen_en, gap_done, channel_done, frame_done)
		begin
		ppm_output <= '0';
		decrement_en <= '0';
		decrement_resetn <= '1';
		gap_en <= '0';
		gap_resetn <= '1';
		channel_count_en <= '0';
		case PS is
			--idle (no enable)
			when idle =>
				ppm_output <= '1';
				decrement_en <= '0';
				gap_en <= '0';
				gap_resetn <= '0'; 
				decrement_resetn <= '0';
				if (gen_en = '0') then NS<=idle; channel_count_en <= '0';
				elsif (frame_done = '1') then  NS <= gap; channel_count_en <= '0'; 
				end if;
			--gap state
			when gap =>
				ppm_output <= '0';
				decrement_en <= '0';
				gap_en <= '1';
				gap_resetn <= '1'; 
				decrement_resetn <= '0';
				if (gap_done = '1') then 
					if (addr_sig = "111") then NS<=idle; channel_count_en <= '0';
					else NS<=chan; channel_count_en <= '0';
					end if;
				else NS <= gap; channel_count_en <= '0';
				end if;
			--channel state
			when chan =>
				ppm_output <= '1';
				decrement_en <= '1';
				gap_en <= '0';
				gap_resetn <= '0';
				decrement_resetn <= '1';
				if (channel_done = '1') then NS<=gap; channel_count_en <= '1';
				else NS <= chan; channel_count_en <= '0';
				end if;
			when others =>
				ppm_output <= '1'; decrement_en <= '0'; gap_en <= '0'; gap_resetn <= '1'; decrement_resetn <= '1'; NS<=idle;
		end case;
	end process comb_proc;
	
	--clocked process
	--counting which channel to read cycle counts from
	--incremented when state chan to state gap
	addr_proc: process(CLK)
		begin
		if(rising_edge(CLK)) then
			if(ppm_gen_resetn = '0') then addr_sig <= "001";
			elsif(frame_done = '1') then  addr_sig <= "001";
			elsif(channel_count_en = '1') then addr_sig <= std_logic_vector(unsigned(addr_sig) + 1);
			else addr_sig <= addr_sig;
			end if;
		end if;
	end process addr_proc;
	
	--clocked process
	--counting gaps low for ppm_output to be low
	--9c40 is 400 us of cycles
	gap_proc: process(CLK)
	begin
		if(rising_edge(CLK)) then
			if(ppm_gen_resetn = '0') then
				gap_done <= '0';
			else
				if(gap_resetn = '0') then gap_val <= x"9C40"; gap_done <= '0';
				elsif(gap_val = x"0000") then gap_done <= '1';
				elsif(gap_en = '1') then gap_val <= gap_val - 1;
				else gap_done <= '0'; gap_val <= x"9C40";
				end if;
			end if;
		end if;
	end process gap_proc;
	
	--clocked process
	--counting channel high for ppm_output to be high
	--reading from read_addr/addr_sig to pull initial register value from inc_cycle_count into decrement_val
	channel_proc: process(CLK)
	begin
		if(rising_edge(CLK)) then
			if(ppm_gen_resetn = '0') then
				channel_done <= '0';
			else
				if(decrement_resetn = '0') then decrement_val <= unsigned(inc_cycle_count); channel_done <= '0';
				elsif(decrement_val = x"00000000") then channel_done <= '1';
				elsif(decrement_en = '1') then decrement_val <= decrement_val - 1;
				else channel_done <= '0';
				end if;
			end if;
		end if;
	end process channel_proc;
	
	--clocked process
	--counting 20ms period (will have a buffer of one frame doing nothing at the beginning)
	period_proc: process(CLK)
	begin	
		if(rising_edge(CLK)) then
			--frame_start is (gen_en AND ~frame_running)
			if(frame_start = '1') then frame_running <= '1'; frame_val <= x"001E8480"; frame_done <= '0';
			end if;
			--when done counting, pulse frame_done bit to move out of idle
			--otherwise, keep counting
			if(frame_val = x"00000000") then frame_done <= '1'; frame_running <= '0';
			elsif(frame_running = '1') then frame_val <= frame_val - 1;
			else frame_running <= '0'; frame_val <= x"001E8480"; frame_done <= '0';
			end if;
		end if;
	end process period_proc;
	
end ppm_gen;