module caju
(
    input logic [3:0] [31:0] state_in,
    input logic [31:0] cv_in,

    output logic [3:0] [31:0] state_out,
    output logic [31:0] cv_out
);

    logic [11:0] [3:0] [31:0] state;
    logic [11:0] [31:0] cv;

    wire [31:0] cv_pre = cv_in ^ state_in[0];

    caju_round r0 (0, state_in, cv_pre, state[0], cv[0]);

    genvar round;
    generate
        for (round=1; round<12; round++) begin : gen_caju_rounds
            caju_round rx (round, state[round-1], cv[round-1], state[round], cv[round]);
        end
    endgenerate

    assign state_out = state[11];
    assign cv_out = cv[11];

endmodule
