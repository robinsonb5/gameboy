
derive_clock_uncertainty;

set sysclk ${topmodule}pll|altpll_component|auto_generated|pll1|clk[0]
create_generated_clock -name ramclk -source [get_pins ${topmodule}pll|altpll_component|auto_generated|pll1|clk[1]] [get_ports $RAM_CLK]

create_generated_clock -name ceclk -source [get_pins $sysclk] -divide_by 2 [get_registers ${topmodule}ce_cpu]

set_input_delay -clock ramclk -max 6.4 [get_ports ${RAM_IN}]
set_input_delay -clock ramclk -min 3.2 [get_ports ${RAM_IN}]

set_output_delay -clock ramclk -max 1.5 [get_ports ${RAM_OUT}]
set_output_delay -clock ramclk -min -0.8 [get_ports ${RAM_OUT}]

set_output_delay -clock [get_clocks $sysclk] -max 0.5 [get_ports ${VGA_OUT}]
set_output_delay -clock [get_clocks $sysclk] -min 0 [get_ports ${VGA_OUT}]

set_multicycle_path -to ${VGA_OUT} -setup 2
set_multicycle_path -to ${VGA_OUT} -hold 1

set_clock_groups -asynchronous -group spiclk -group [get_clocks $sysclk]
set_multicycle_path -from ramclk -to [get_clocks $sysclk] -setup 2
set_multicycle_path -from ramclk -to ceclk -setup 2

set_false_path -to [get_ports ${FALSE_OUT}]
set_false_path -from [get_ports ${FALSE_IN}]

