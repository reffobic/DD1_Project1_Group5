module circuit_1(a, b, c, out);
    input a;
    input b;
    input c;
    output out;

    wire w1;
    wire w2;

    and #(5) g0 (w1, a, b);
    or #(5) g1 (w2, w1, c);
    not #(2) g2 (out, w2);
endmodule
