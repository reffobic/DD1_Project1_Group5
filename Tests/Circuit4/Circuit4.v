module dec4to16(a, b, c, d, out);
    input a;
    input b;
    input c;
    input d;
    output out;

    wire w1;
    wire w2;
    wire w3;
    wire a_not;
    wire c_not;
    wire d_not;

    not #(2) g0 (a_not, a);
    not #(2) g1 (c_not, c);
    not #(2) g2 (d_not, d);
    and #(5) g3 (w1, a_not, c, d_not);
    and #(5) g4 (w2, a, c_not, d_not);
    and #(5) g5 (w3, b, c, d_not);
    or #(5) g6 (out, w1, w2, w3);
endmodule
