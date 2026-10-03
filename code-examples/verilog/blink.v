// ============================================================================
// File: blink.v
// Module: blink
// Description: Synthesizable digital clock divider and 1 Hz LED blinker.
// Input Clock: 50 MHz (e.g. standard FPGA oscillator)
// ============================================================================

module blink (
    input  wire clk,    // 50 MHz clock
    input  wire rst_n,  // Active-low asynchronous reset
    output reg  led     // User LED output
);

    // 50 MHz / 2 = 25,000,000 cycles for 0.5s toggle (1 Hz full blink period)
    localparam COUNT_MAX = 25_000_000 - 1;

    reg [24:0] counter;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            counter <= 25'd0;
            led     <= 1'b0;
        end else begin
            if (counter >= COUNT_MAX) begin
                counter <= 25'd0;
                led     <= ~led; // Toggle LED
            end else begin
                counter <= counter + 1'b1;
            end
        end
    end

endmodule
