module byte_enabled_simple_dual_port_ram //from Intel Recommended HDL Coding Styles
(
	 input we, clk, 
	 input [ADDRESS_WIDTH-1:0] waddr, raddr,// address width = 12
	 input [NUM_BYTES-1:0] be, // 4 bytes per word
	 input [(BYTE_WIDTH * NUM_BYTES -1):0] wdata, // byte width = 8, 4 bytes per word
	 output reg [(BYTE_WIDTH * NUM_BYTES -1):0] q // byte width = 8, 4 bytes per word
);
	 parameter ADDRESS_WIDTH = 12;
	 parameter DEPTH = 1200;
	 parameter BYTE_WIDTH = 8;
	 parameter NUM_BYTES = 4;
	 // use a multi-dimensional packed array
	 //to model individual bytes within the word
	 logic [NUM_BYTES-1:0][BYTE_WIDTH-1:0] ram[0:DEPTH-1];
	 // # words = 1 << address width
	 // port A
	 always@(posedge clk)
	 begin
		if(we) begin
			 for (int i = 0; i < NUM_BYTES; i = i + 1) begin
				if(be[i]) ram[waddr][i] <= wdata[i*BYTE_WIDTH +: BYTE_WIDTH];
			 end
		 end
		q <= ram[raddr];
	 end
endmodule