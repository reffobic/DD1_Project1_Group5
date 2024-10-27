module circuit_2(a, b, s, out);
    input a;
    input b;
    input s;
    output out;

    wire w1;
    wire w2;
    wire s_not;

    not #(2) g0 (s_not, s);
    and #(5) g1 (w1, a, s_not);
    and #(5) g2 (w2, b, s);
    or #(5) g3 (out, w1, w2);
endmodule
