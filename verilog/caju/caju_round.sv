module caju_round
(
    //input clk,
    input [31:0] round,

    input logic [3:0] [31:0] state_in,
    input logic [31:0] cv_in,

    output logic [3:0] [31:0] state_out,
    output logic [31:0] cv_out
);

    logic [31:0] cv_subbox;
    logic [31:0] cv_linmap;


    s_box subbox (cv_in, cv_subbox);
    linear_map linmap (cv_subbox, cv_linmap);
    update_key upkey (state_in, state_out);

    assign cv_out = cv_linmap ^ (state_out[0] ^ round);

    // always_ff @(posedge clk) begin

    //     s_box subbox (cv_in, cv_subbox);
    //     linear_map linmap (cv_subbox, cv_linmap);
    //     update_key upkey (state_in, state_out);

    //     cv_out <= cv_linmap ^ (state_out[0] ^ round);
    // end


endmodule
