module key_expansion
    #(parameter int ROUNDS = 68)
(
    input [127:0] key_in,

    output [67:0] [63:0] round_key
);

    genvar round;
    generate
        for (round=2; round<ROUNDS; round++) begin : gen_simon_round
            round_key [round] = 
        end
    endgenerate




endmodule
