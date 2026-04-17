module update_key
(
    // input[31:0] k0_in,
    // input[31:0] k1_in,
    // input[31:0] k2_in,
    // input[31:0] k3_in,

    // output[31:0] k0_out,
    // output[31:0] k1_out,
    // output[31:0] k2_out,
    // output[31:0] k3_out,

    input [3:0] [31:0] k_in,
    output [3:0] [31:0] k_out
);

    // wire k01_xor = k0_in ^ k1_in;

    // assign k2_out = {k01_xor[12:0], k01_xor[31:13]} ^ k3_in;    //Rotate left 19
    // assign k3_out = k1_in ^ k1_in;
    // assign k0_out = k1_in;
    // assign k1_out = k2_in;

    wire [31:0] k01_xor = k_in[0] ^ k_in[1];

    assign k_out[2] = {k01_xor[12:0], k01_xor[31:13]} ^ k_in[3];    //Rotate left 19
    assign k_out[3] = k_in[0] ^ k_in[1];
    assign k_out[0] = k_in[1];
    assign k_out[1] = k_in[2];


endmodule
