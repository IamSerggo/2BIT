-- cpu.vhd: Simple 8-bit CPU (BrainF*ck interpreter)
-- Copyright (C) 2018 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Radovan Babic (xbabic09)
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru

   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti

   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='1') / zapis do pameti (DATA_RDWR='0')
   DATA_EN    : out std_logic;                    -- povoleni cinnosti

   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
   IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice

   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 -- zde dopiste potrebne deklarace signalu

type state_of_proc is(
INIT,
DECODE,
INCPTR,
DECPTR,
INCVAL,
DECVAL,
INCVAL2,
DECVAL2,
VALREAD1,
VALREAD2,
VALPRINT1,
VALPRINT2,
INWHILE1,
INWHILE2,
INWHILE3,
INWHILE4,
OUTWHILE1,
OUTWHILE2,
OUTWHILE3,
OUTWHILE4,
OUTWHILE5,
COMMENT,
COMMENT2,
NUM,
LET,
OTHER,
ENDE
);

signal curr_state: state_of_proc;
signal next_state: state_of_proc;

signal ptrAdr: std_logic_vector(9 downto 0); --adresa dat v RAM
signal ptrInc: std_logic;
signal ptrDec: std_logic;

signal cntAdr: std_logic_vector(7 downto 0); --zaciatok while cyklu
signal cntInc: std_logic;
signal cntDec: std_logic;

signal pcAdr: std_logic_vector(11 downto 0); --adresa instrukcie
signal pcInc: std_logic;
signal pcDec: std_logic;

signal mux: std_logic_vector(1 downto 0); --multiplexor
signal temp_var: std_logic_vector(7 downto 0); --pomocna premenna na ulozenie dat na vypis

begin

 -- zde dopiste vlastni VHDL kod dle blokoveho schema

 -- inspirujte se kodem procesoru ze cviceni

 -------- Program Counter --------
 process(CLK, RESET)
	begin
    --if EN = '1' then
  		if RESET = '1' then
  			pcAdr <= ("000000000000");
  		elsif rising_edge(CLK) then
  			if pcInc = '1' then
  				pcAdr <= pcAdr + 1;
  			elsif pcDec = '1' then
  				pcAdr <= pcAdr - 1;
  			end if;
  		end if;
    --end if;
	end process;

  CODE_ADDR <= pcAdr;
 --------------------------------

 -------- Current Data Pointer --------
 process(CLK, RESET)
	begin
    --if EN = '1' then
  		if RESET = '1' then
  			ptrAdr <= ("0000000000");
  		elsif rising_edge(CLK) then
  			if ptrInc = '1' then
  				ptrAdr <= ptrAdr + 1;
  			elsif ptrDec = '1' then
  				ptrAdr <= ptrAdr - 1;
  			end if;
  		end if;
    --end if;
	end process;

  DATA_ADDR <= ptrAdr;
 --------------------------------

 -------- Loop Counter --------
 process(CLK, RESET)
	begin
    --if EN = '1' then
  		if RESET = '1' then
  			cntAdr <= ("00000000");
  		elsif rising_edge(CLK) then
  			if cntInc = '1' then
  				cntAdr <= cntAdr + 1;
  			elsif cntDec = '1' then
  				cntAdr <= cntAdr - 1;
  			end if;
  		end if;
    --end if;
	end process;
 --------------------------------

 -------- Multiplexor --------
 process(DATA_RDATA, IN_DATA, mux)
	begin
    --if EN = '1' then
  		case mux is
  			when "11" => DATA_WDATA <= temp_var;
        when "00" => DATA_WDATA <= IN_DATA;
  			when "01" => DATA_WDATA <= DATA_RDATA+1;
        when others => DATA_WDATA <= DATA_RDATA-1;
  		end case;
    --end if;
	end process;
 --------------------------------

 -------- State transition --------
 process(CLK, RESET, EN)
	begin
    --if EN = '1' then
  		if RESET = '1' then
  			curr_state <= INIT;
  		elsif (rising_edge(CLK) and EN = '1') then
  			curr_state <= next_state;
  		end if;
    --end if;
	end process;
 --------------------------------

 -------- Final State Machine --------
 process(CLK, RESET, EN, DATA_RDATA, CODE_DATA, IN_DATA, IN_VLD, OUT_BUSY)
	begin
		CODE_EN <= '1';
    DATA_EN <= '0';
		DATA_RDWR <= '0';
		IN_REQ <= '0';
		OUT_WE <= '0';

		ptrInc <= '0';
		ptrDec <= '0';

    cntInc <= '0';
		cntDec <= '0';

    pcInc <= '0';
		pcDec <= '0';

		mux <= "00";

    next_state <= INIT;

		case curr_state is
			when INIT =>
				CODE_EN <= '1';
				next_state <= DECODE;

			when DECODE =>
				case CODE_DATA is
					when X"3E" => next_state <= INCPTR;
					when X"3C" => next_state <= DECPTR;
					when X"2B" => next_state <= INCVAL;
					when X"2D" => next_state <= DECVAL;
					when X"5B" => next_state <= INWHILE1;
					when X"5D" => next_state <= OUTWHILE1;
					when X"2E" => next_state <= VALPRINT1;
					when X"2C" => next_state <= VALREAD1;
					when X"23" => next_state <= COMMENT;
					when X"30" => next_state <= NUM;
					when X"31" => next_state <= NUM;
					when X"32" => next_state <= NUM;
					when X"33" => next_state <= NUM;
					when X"34" => next_state <= NUM;
					when X"35" => next_state <= NUM;
					when X"36" => next_state <= NUM;
					when X"37" => next_state <= NUM;
					when X"38" => next_state <= NUM;
					when X"39" => next_state <= NUM;
					when X"41" => next_state <= LET;
					when X"42" => next_state <= LET;
					when X"43" => next_state <= LET;
					when X"44" => next_state <= LET;
					when X"45" => next_state <= LET;
					when X"46" => next_state <= LET;
					when X"00" => next_state <= ENDE;
					when others => next_state <= OTHER;
				end case;

			when INCPTR =>
				pcInc <= '1';
				ptrInc <= '1';
				next_state <= INIT;

			when DECPTR =>
				pcInc <= '1';
				ptrDec <= '1';
				next_state <= INIT;

			when INCVAL =>
				DATA_RDWR <= '1';
				DATA_EN <= '1';
				next_state <= INCVAL2;

			when INCVAL2 =>
				DATA_RDWR <= '0';
				DATA_EN <= '1';
				pcInc <= '1';
				mux <= "01";
				next_state <= INIT;

			when DECVAL =>
				DATA_RDWR <= '1';
				DATA_EN <= '1';
				next_state <= DECVAL2;

			when DECVAL2 =>
				DATA_RDWR <= '0';
				DATA_EN <= '1';
				pcInc <= '1';
				mux <= "10";
				next_state <= INIT;

			when VALPRINT1 =>
				if OUT_BUSY = '0' then
					DATA_RDWR <= '1';
					DATA_EN <= '1';
					next_state <= VALPRINT2;
				else next_state <= VALPRINT1;
				end if;

			when VALPRINT2 =>
				OUT_DATA <= DATA_RDATA;
				OUT_WE <= '1';
				pcInc <= '1';
				next_state <= INIT;

			when VALREAD1 =>
				IN_REQ <= '1';

				if IN_VLD = '1' then
					DATA_RDWR <= '0';
					DATA_EN <= '1';
					pcInc <= '1';
					mux <= "00";
					next_state <= INIT;
				else next_state <= VALREAD1;
				end if;

			when COMMENT =>
				pcInc <= '1';
        CODE_EN <= '1';
				next_state <= COMMENT2;

			when COMMENT2 =>
				if CODE_DATA = X"23" then
					pcInc <= '1';
					next_state <= INIT;
				else next_state <= COMMENT;
				end if;

      when LET =>
				DATA_EN <= '1';
				pcInc <= '1';
				mux <= "11";
				temp_var <= (CODE_DATA(3 downto 0) + std_logic_vector(conv_unsigned(9, temp_var'LENGTH))) & "0000";
				next_state <= INIT;

			when NUM =>
				DATA_EN <= '1';
				pcInc <= '1';
				mux <= "11";
				temp_var <= CODE_DATA(3 downto 0) & "0000";
				next_state <= INIT;

			when INWHILE1 =>
				DATA_RDWR <= '1';
				DATA_EN <= '1';
				pcInc <= '1';
				next_state <= INWHILE2;

			when INWHILE2 =>
				if DATA_RDATA = "00000000" then
					cntInc <= '1';
					next_state <= INWHILE3;
				else next_state <= INIT;
				end if;

			when INWHILE3 =>
				if cntAdr = "00000000" then
					--cntInc <= '1';
					next_state <= INIT;
				else
					CODE_EN <= '1';
					next_state <= INWHILE4;
				end if;

			when INWHILE4 =>
				if CODE_DATA = X"5D" then
					cntDec <= '1';
				elsif CODE_DATA = X"5B" then
					cntInc <= '1';
				end if;
				pcInc <= '1';
				next_state <= INWHILE3;

			when OUTWHILE1 =>
				DATA_RDWR <= '1';
				DATA_EN <= '1';
				next_state <= OUTWHILE2;

			when OUTWHILE2 =>
				if DATA_RDATA = "00000000" then
					pcInc <= '1';
					next_state <= INIT;
				else
					pcDec <= '1';
					cntInc <= '1';
					next_state <= OUTWHILE3;
				end if;

			when OUTWHILE3 =>
				if cntAdr =  "00000000" then
					next_state <= INIT;
				else
					CODE_EN <= '1';
					next_state <= OUTWHILE4;
				end if;

			when OUTWHILE4 =>
				if CODE_DATA = X"5B" then
					cntDec <= '1';
				elsif CODE_DATA = X"5D" then
					cntInc <= '1';
				end if;
				next_state <= OUTWHILE5;

			when OUTWHILE5 =>
				if cntAdr =  "00000000" then
					pcInc <= '1';
				else
					pcDec <= '1';
				end if;
				next_state <= OUTWHILE3;

			when OTHER =>
				pcInc <= '1';
				next_state <= INIT;

			when ENDE =>
				next_state <= ENDE;

			when others =>
        next_state <= INIT;

		end case;
	end process;
end behavioral;
