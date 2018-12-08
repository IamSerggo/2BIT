library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port (
	RESET:in std_logic;
	SMCLK:in std_logic;
	ROW:out std_logic_vector (0 to 7);
	LED:out std_logic_vector (0 to 7)
);
end ledc8x8;

architecture main of ledc8x8 is
signal counter: std_logic_vector(7 downto 0);
signal counterino: std_logic_vector(20 downto 0);
signal my_signal: std_logic;
signal rows: std_logic_vector(7 downto 0);
signal column: std_logic_vector(7 downto 0);
signal mux: std_logic_vector(1 downto 0);


begin
------------------------------------------------------------------------------------
	signal_count: process(SMCLK, RESET)
	begin
		if RESET = '1' then
			counter <= (others => '0');
		elsif rising_edge(SMCLK) then
			counter <= counter + '1';
		end if;
	end process signal_count;

	my_signal <= '1' when counter = "11111111" else '0';
-------------------------------------------------------------------------------------
	delay_generate: process(RESET, SMCLK, mux)
	begin
		if RESET = '1' then
			counterino <= (others => '0');
		elsif rising_edge(SMCLK) then
			counterino <= counterino + '1';
			if counterino = "111000010000000000000" then
				counterino <= (others => '0');
				mux <= mux + '1';
			end if;
		end if;
	end process delay_generate;
--------------------------------------------------------------------------------------
	row_rotation: process(RESET, SMCLK, my_signal)
	begin
		if RESET = '1' then
			rows <= "10000000";
		elsif rising_edge(SMCLK) and my_signal = '1' then
			rows <= rows(0) & rows(7 downto 1);
		end if;
	end process row_rotation;
-------------------------------------------------------------------------------------
	print_initials: process(rows, mux)
	begin
		if mux = "00" then
			case rows is
				when "10000000" => column <= "00000111";
				when "01000000" => column <= "01110111";
				when "00100000" => column <= "00000111";
				when "00010000" => column <= "01011111";
				when "00001000" => column <= "01101111";
				when "00000100" => column <= "01110111";
				when others     => column <= "11111111";
				end case;
		elsif mux = "10" then
			case rows is
				when "10000000" => column <= "00001111";
				when "01000000" => column <= "01110111";
				when "00100000" => column <= "00001111";
				when "00010000" => column <= "01110111";
				when "00001000" => column <= "01110111";
				when "00000100" => column <= "00001111";
				when others     => column <= "11111111";
				end case;
		else
			column  <= "11111111";
		end if;
	end process print_initials;
	--------------------------------------------------------------------------------------
	ROW <= rows;
	LED <= column;
end main;
