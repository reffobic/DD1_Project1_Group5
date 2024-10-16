`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 10/14/2024 06:34:24 PM
// Design Name: 
// Module Name: Circuit4
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module DEC4to16(input A, input B, input C, input D, output out);
    assign out = (~a&c&~d)+ (a&~c&~d) + (b&c&~d);
endmodule

