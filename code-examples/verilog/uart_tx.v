// ============================================================================
// File: uart_tx.v
// Module: uart_tx
// Description: Fully synthesizable 8-N-1 UART serial transmitter.
// Clocks per bit: CLKS_PER_BIT = (Clock Frequency / Baud Rate)
// Example: 50 MHz clock / 115,200 baud = 434 clock ticks per bit.
// ============================================================================

module uart_tx #(
    parameter CLKS_PER_BIT = 434
)(
    input  wire       clk,        // System Clock
    input  wire       rst_n,      // Active-low Reset
    input  wire       tx_start,   // Pulse high to initiate transmission
    input  wire [7:0] tx_data,    // 8-bit byte to transmit
    output reg        tx_active,  // High while transmission in progress
    output reg        tx_serial,  // Physical serial TX line
    output reg        tx_done     // Pulse high for 1 cycle when completed
);

    localparam STATE_IDLE  = 3'b000;
    localparam STATE_START = 3'b001;
    localparam STATE_DATA  = 3'b010;
    localparam STATE_STOP  = 3'b011;
    localparam STATE_CLEAN = 3'b100;

    reg [2:0]  state;
    reg [15:0] clk_count;
    reg [2:0]  bit_index;
    reg [7:0]  tx_byte;

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            state      <= STATE_IDLE;
            tx_serial  <= 1'b1; // Idle line is HIGH
            tx_active  <= 1'b0;
            tx_done    <= 1'b0;
            clk_count  <= 16'd0;
            bit_index  <= 3'd0;
            tx_byte    <= 8'd0;
        end else begin
            case (state)
                STATE_IDLE: begin
                    tx_serial <= 1'b1;
                    tx_done   <= 1'b0;
                    clk_count <= 16'd0;
                    bit_index <= 3'd0;
                    if (tx_start) begin
                        tx_active <= 1'b1;
                        tx_byte   <= tx_data;
                        state     <= STATE_START;
                    end else begin
                        tx_active <= 1'b0;
                    end
                end

                // Transmit Start Bit (0)
                STATE_START: begin
                    tx_serial <= 1'b0;
                    if (clk_count < (CLKS_PER_BIT - 1)) begin
                        clk_count <= clk_count + 1'b1;
                    end else begin
                        clk_count <= 16'd0;
                        state     <= STATE_DATA;
                    end
                end

                // Transmit 8 Data Bits (LSB first)
                STATE_DATA: begin
                    tx_serial <= tx_byte[bit_index];
                    if (clk_count < (CLKS_PER_BIT - 1)) begin
                        clk_count <= clk_count + 1'b1;
                    end else begin
                        clk_count <= 16'd0;
                        if (bit_index < 7) begin
                            bit_index <= bit_index + 1'b1;
                        end else begin
                            bit_index <= 3'd0;
                            state     <= STATE_STOP;
                        end
                    end
                end

                // Transmit Stop Bit (1)
                STATE_STOP: begin
                    tx_serial <= 1'b1;
                    if (clk_count < (CLKS_PER_BIT - 1)) begin
                        clk_count <= clk_count + 1'b1;
                    end else begin
                        clk_count <= 16'd0;
                        tx_done   <= 1'b1;
                        state     <= STATE_CLEAN;
                    end
                end

                STATE_CLEAN: begin
                    tx_done   <= 1'b0;
                    tx_active <= 1'b0;
                    state     <= STATE_IDLE;
                end

                default: state <= STATE_IDLE;
            endcase
        end
    end

endmodule
