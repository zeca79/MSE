library IEEE;
use IEEE.std_logic_1164.all;
use ieee.numeric_std.all;
use std.textio.all;
use IEEE.math_real.all; -- Função log2

entity testbench is
end testbench;

architecture tb of testbench is
    constant passo : time := 20 ns;
	 
    constant B : POSITIVE := 8; -- Número de bits por amostra
    constant O : POSITIVE := 8; -- Número de amostras por coluna do bloco (em potência de 2)
    constant P : POSITIVE := 8; -- Número de amostras por linha do bloco (em potência de 2)
    
    -- Função auxiliar para poder usar log2 para o address genérico
    function calculate_Z(O : POSITIVE; P : POSITIVE) return REAL is
    begin
        return REAL(O * P);  -- Realiza a multiplicação na função
    end function calculate_Z;
    
    constant Z : REAL := calculate_Z(O, P);
    
    constant x : INTEGER := integer(log2(Z));
	 
    signal clk : STD_LOGIC;  -- Clock
    signal enable : STD_LOGIC;  -- Iniciar
    signal reset : STD_LOGIC;  -- Reset
    signal sample_ori : STD_LOGIC_VECTOR (B-1 DOWNTO 0);  -- Mem_A[end]
    signal sample_can : STD_LOGIC_VECTOR (B-1 DOWNTO 0);  -- Mem_B[end]
    signal read_mem : STD_LOGIC;  -- Read
    signal address : STD_LOGIC_VECTOR (x-1 DOWNTO 0);  -- Endereço
    signal resto : STD_LOGIC_VECTOR ((2*B)-1 DOWNTO 0);  -- Sobra fracionada do truncamento
    signal mse_value : STD_LOGIC_VECTOR ((2*B)-1 DOWNTO 0);  -- MSE (resultado truncado)
    signal done : STD_LOGIC;
		
    signal comparavel : std_LOGIC_VECTOR ((2*B)-1 downto 0);	
begin
    -- Geração do processo de clock
    clk_process : process
    begin
        while true loop
            clk <= '0';
            wait for passo / 2;
            clk <= '1';
            wait for passo / 2;
        end loop;
    end process clk_process;
	 
    -- Instanciação do DUT (Design Under Test)
    DUV: entity work.mse
        port map (
            clk => clk,
            enable => enable,
            reset => reset,
            sample_ori => sample_ori,
            sample_can => sample_can,
            read_mem => read_mem,
            address => address,
            resto => resto,
            mse_value => mse_value,
            done => done
        );

    -- Processo de estímulos
    stim_process : process
        file arquivo_de_estimulos : text open read_mode is "../../estimulos.dat";
        variable linha_de_estimulos : line;
        variable espaco : character;
        variable valor_de_entrada : bit_vector(B-1 downto 0);
        variable valor_de_entrada2 : bit_vector(B-1 downto 0);
        variable result_comp : bit_vector((2*B)-1 downto 0);
    begin
        enable <= '1';
        wait for passo;
        enable <= '0';

        while not endfile(arquivo_de_estimulos) loop
            -- Aguarda read_mem ser '1'
            wait until read_mem = '1';

            -- Leitura de inputs
            readline(arquivo_de_estimulos, linha_de_estimulos);
				
            -- Leitura do primeiro input
            read(linha_de_estimulos, valor_de_entrada);
            sample_ori <= to_stdlogicvector(valor_de_entrada);
            read(linha_de_estimulos, espaco);
            
            -- Leitura do segundo input
            read(linha_de_estimulos, valor_de_entrada2);
            sample_can <= to_stdlogicvector(valor_de_entrada2);
            read(linha_de_estimulos, espaco);
				
            -- Leitura do valor esperado
            read(linha_de_estimulos, result_comp);
            comparavel <= to_stdlogicvector(result_comp);

            wait for passo;
            
            -- Teste do output somente se done = '1'
            if done = '1' then
                assert mse_value = comparavel
                    report "Falha na simulação."
                    severity error;
            end if;
        end loop;
		  
        wait for passo;
        assert false
            report "Teste concluído."
            severity note;
        wait;
    end process stim_process;

end tb;
