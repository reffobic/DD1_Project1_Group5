module dec4to16(a, b, c, d, out0, out1, out2, out3, out4, out5, out6, out7, out8, out9, out10, out11, out12, out13, out14, out15);
    input a;
    input b;
    input c;
    input d;
    output out0;
    output out1;
    output out2;
    output out3;
    output out4;
    output out5;
    output out6;
    output out7;
    output out8;
    output out9;
    output out10;
    output out11;
    output out12;
    output out13;
    output out14;
    output out15;

    wire a_not;
    wire b_not;
    wire c_not;
    wire d_not;

    not #(2) g0 (a_not, a);
    not #(2) g1 (b_not, b);
    not #(2) g2 (c_not, c);
    not #(2) g3 (d_not, d);

    and #(5) g4 (out0, a_not, b_not, c_not, d_not);
    and #(5) g5 (out1, a_not, b_not, c_not, d);
    and #(5) g6 (out2, a_not, b_not, c, d_not);
    and #(5) g7 (out3, a_not, b_not, c, d);
    and #(5) g8 (out4, a_not, b, c_not, d_not);
    and #(5) g9 (out5, a_not, b, c_not, d);
    and #(5) g10 (out6, a_not, b, c, d_not);
    and #(5) g11 (out7, a_not, b, c, d);
    and #(5) g12 (out8, a, b_not, c_not, d_not);
    and #(5) g13 (out9, a, b_not, c_not, d);
    and #(5) g14 (out10, a, b_not, c, d_not);
    and #(5) g15 (out11, a, b_not, c, d);
    and #(5) g16 (out12, a, b, c_not, d_not);
    and #(5) g17 (out13, a, b, c_not, d);
    and #(5) g18 (out14, a, b, c, d_not);
    and #(5) g19 (out15, a, b, c, d);
endmodule
