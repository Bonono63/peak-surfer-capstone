module linear_map
(
    input [31:0] cv,
    output [31:0] cv_out
);

    //just psuedonyms for the input cv words
    wire [7:0] in_z = cv[7:0];
    wire [7:0] in_y = cv[15:8];
    wire [7:0] in_x = cv[23:16];
    wire [7:0] in_w = cv[31:24];

    // Round 1
    wire [7:0] r1_w = {in_w[5:0], in_w[7:6]} ^ in_x;    //LCS=2
    wire [7:0] r1_x = {in_x[1:0], in_x[7:2]} ^ r1_w;    //LCS=6

    // Round 2
    wire [7:0] r2_w = {r1_w[6:0], r1_w[7]} ^ r1_x;  //LCS=1
    wire [7:0] r2_x = {r1_x[0], r1_x[7:1]} ^ r2_w;  //LCS=7

    // Scramble
    assign cv_out = {in_y, in_z, r2_w, r2_x};

endmodule
