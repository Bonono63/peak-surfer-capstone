module simon_round
(
    input [31:0] round,

    input [63:0] round_key, //TODO: remove later

    input [63:0] x_in,
    input [63:0] y_in,

    output [63:0] x_out,
    output [63:0] y_in
);

    assign x_out = y_in;

    // Y xor (rotateLeft(x,1) and rotateLeft(x,7))
    wire [63:0] yxor1 = y_in ^ ({x_in[62:0], x_in[63]} & {x_in[55:0], x_in[63:56]});

    // new(Y) xor (rotateLeft(x,2) xor roundkey)
    assign x_out = round_key ^ (yxor1 ^ {x_in[61:0], x_in[63:62]});

endmodule
