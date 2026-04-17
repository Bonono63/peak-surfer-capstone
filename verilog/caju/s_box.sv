module s_box
(
    input [31:0] cv,
    output [31:0] cv_out
);

    // could make this actual wires instead of the unreadable mess below lmfao
    // cv[7:0] = input(Z)
    // cv[15:8] = input(Y)
    // cv[23:16] = input(X)
    // cv[31:24] = input(W)

    wire [7:0] x = cv[23:16] ^ (cv[31:24] & cv[15:8]);
    wire [7:0] z = cv[7:0] ^ (x & cv[15:8]);
    wire [7:0] y = cv[15:8] ^ (cv[31:24] & z);
    wire [7:0] w = cv[31:24] ^ (x & z);

    // could shrnk this even more by directly mapping outputs
    // but that gets undreadable.
    assign cv_out = {w, x, y, z};

endmodule
