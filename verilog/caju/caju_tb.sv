module caju_tb();

    logic [3:0] [31:0] state;
    logic [31:0] cv;

    initial begin
        $dumpfile("test.vcd");
        $dumpvars();

        state = 0;
        cv = 0;

        #10;

        state[0] = 32'h01234567;
        state[1] = 32'h89abcdef;
        state[2] = 32'h12345678;
        state[3] = 32'h9abcdef0;
        cv = 32'h78_56_34_12;

        #10;
    end


    logic [3:0] [31:0] state_out;
    logic [31:0] cv_out;
    caju dut (state, cv, state_out, cv_out);



endmodule
