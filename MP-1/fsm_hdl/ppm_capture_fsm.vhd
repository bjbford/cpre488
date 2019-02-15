library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

-- entity
entity ppm_capture_fsm is
port (ppm_input, CLK : in std_logic;
	ppm_cap_resetn : in std_logic;
	write_en : out std_logic;
	cycle_count : out std_logic_vector(31 downto 0);
	write_addr : out std_logic_vector(2 downto 0);
	end_of_frame : out std_logic);
end ppm_capture_fsm;

-- architecture
architecture ppm_capture_fsm of ppm_capture_fsm is
	type state_type is (idle, gap, channel);
	-- should we initialize unknowns here (instead of on hard reset)
	signal PS, NS : state_type;
	signal state_resetn : std_logic := '1';
	signal count_en_sig, channel_count_sig, end_of_frame_sig, debounce_success : std_logic := '0';
	signal write_addr_sig : std_logic_vector(2 downto 0) := "000";
	signal cycle_count_sig : std_logic_vector(31 downto 0) := x"00000000";
	signal debounce_sig : std_logic := '1';
	signal debounce_cnt : unsigned (3 downto 0) := "0000";
	
	begin
	cycle_count <= cycle_count_sig;
	write_addr <= write_addr_sig;
	end_of_frame <= end_of_frame_sig;
	
	sync_proc: process(CLK)
		begin
		if(rising_edge(CLK)) then
            if (ppm_cap_resetn = '0') then PS <= idle;
            else PS <= NS;
            end if;
		end if;
	end process sync_proc;
	
	debounce_proc: process(CLK)
		begin
		if(rising_edge(CLK)) then
			debounce_sig <= ppm_input;
			
			if(debounce_cnt = "1111") then 
				debounce_cnt <= "0000";
				debounce_success <= '1';
			elsif(debounce_sig = ppm_input) then
				debounce_cnt <= debounce_cnt + 1;
				debounce_success <= '0';
			else
				debounce_cnt <= "0000";
				debounce_success <= '0';
			end if;

		end if;
	end process debounce_proc;
	
	comb_proc: process(PS, ppm_input, state_resetn, debounce_success)
		begin
		--default value assignments
		NS <= idle;
		channel_count_sig <= '0';
		count_en_sig <= '0';
		write_en <= '0';
		case PS is
			--idle state
			when idle =>
				count_en_sig <= '0';
				if (ppm_input='0') then
					if(debounce_success = '1') then
						NS<=gap; channel_count_sig <= '0'; write_en <= '0';
					else NS <= idle; channel_count_sig <= '0'; write_en <= '0';
					end if;
				else NS <= idle; channel_count_sig <= '0'; write_en <= '0';
				end if;
			--gap state
			when gap =>
				count_en_sig <= '0'; 
				if (ppm_input='1') then
					if(debounce_success = '1') then 
						NS <= channel; channel_count_sig <= '1'; write_en <= '0';
					else NS<=gap; channel_count_sig <= '0'; write_en <= '0';
					end if;
				else NS<=gap; channel_count_sig <= '0'; write_en <= '0';
				end if;
			--channel state
			when channel =>
				count_en_sig <= '1';
				if (ppm_input='0') then
					if(debounce_success = '1') then 
						NS<=gap; channel_count_sig <= '0'; write_en <= '1';
					else NS <= channel;  channel_count_sig <= '0'; write_en <= '0';
					end if;
				elsif (state_resetn = '0') then NS <= idle; channel_count_sig <= '0'; write_en <= '0';
				else NS <= channel;  channel_count_sig <= '0'; write_en <= '0';
				end if;
			when others =>
				channel_count_sig <= '0'; count_en_sig <= '0'; write_en <= '0'; NS <= idle;
		end case;
	end process comb_proc;
	
	--clock process
	--counting cycles of high input
	--MIGHT NEED TO ADD 1, MOST LIKELY DOESN'T MATTER
	cycle_proc: process(CLK)
		begin
		if(rising_edge(CLK)) then
			if(count_en_sig = '1') then cycle_count_sig <= std_logic_vector(unsigned(cycle_count_sig) + 1);
			else cycle_count_sig <= (others => '0');
			end if;
		end if;
	end process cycle_proc;
	
	--clock process
	--counting channels - counts when channel_count_sig goes high between state channel and gap
	channel_proc: process(CLK)
		begin
		if(rising_edge(CLK)) then
			if(ppm_cap_resetn = '0') then
				write_addr_sig <= (others => '0'); end_of_frame_sig <= '0';	state_resetn <= '1'; 
			else
				if(write_addr_sig = "111") then
					write_addr_sig <= (others => '0');
					end_of_frame_sig <= '1';
					state_resetn <= '0';
				elsif(channel_count_sig = '1') then write_addr_sig <= std_logic_vector(unsigned(write_addr_sig) + 1); end_of_frame_sig <= '0'; state_resetn <= '1';
				else end_of_frame_sig <= '0'; state_resetn <= '1';
				end if;
			end if;
		end if;
	end process channel_proc;
	
end ppm_capture_fsm;