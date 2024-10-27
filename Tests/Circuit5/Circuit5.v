module circuit_5(a, b, c, out);
    input a;
    input b;
    input c;
    output out;

    wire w1;
    wire w2;
    wire ab_nand;

    nand #(5) g0 (ab_nand, a, b);
    and #(5) g2 (w2, b, c);
    or #(5) g1 (out, ab_nand, w2);
endmodule
