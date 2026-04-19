module topmodule();

    logic [3:0] [31:0] state;
    logic [31:0] cv;



    assign state[0] = 32'h01234567;
    assign state[1] = 32'h89abcdef;
    assign state[2] = 32'h12345678;
    assign state[3] = 32'h9abcdef0;
    assign cv = 32'h78_56_34_12;



    logic [3:0] [31:0] state_out;
    logic [31:0] cv_out;
    caju dut (state, cv, state_out, cv_out);



endmodule
