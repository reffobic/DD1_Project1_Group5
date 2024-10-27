module circuit_3(a, b, out);
    input a;
    input b;
    output out;

    xor #(5) g0 (out, a, b);
endmodule
