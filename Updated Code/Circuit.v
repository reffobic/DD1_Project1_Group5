module circuit_3(a, b, cin, sum, carry, xor_out);
    input a;
    input b;
    input cin;
    
    output sum;
    output carry;
    output xor_out;

    wire w1;
    wire w2;
    wire w3;

    xor #(5) g0 (w1, a, b);
    and #(3) g1 (w2, a, b);
    xor #(5) g2 (sum, w1, cin);
    and #(3) g3 (w3, w1, cin);
    or #(4) g4 (carry, w2, w3);
    xor #(5) g5 (xor_out, a, b, cin);
endmodule
