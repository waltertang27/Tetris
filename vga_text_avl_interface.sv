/************************************************************************
Avalon-MM Interface VGA Text mode display

Register Map:
0x000-0x0257 : VRAM, 80x30 (2400 byte, 600 word) raster order (first column then row)
0x258        : control register

VRAM Format:
X->
[ 31  30-24][ 23  22-16][ 15  14-8 ][ 7    6-0 ]
[IV3][CODE3][IV2][CODE2][IV1][CODE1][IV0][CODE0]

IVn = Draw inverse glyph
CODEn = Glyph code from IBM codepage 437

Control Register Format:
[[31-25][24-21][20-17][16-13][ 12-9][ 8-5 ][ 4-1 ][   0    ] 
[[RSVD ][FGD_R][FGD_G][FGD_B][BKG_R][BKG_G][BKG_B][RESERVED]

VSYNC signal = bit which flips on every Vsync (time for new frame), used to synchronize software
BKG_R/G/B = Background color, flipped with foreground when IVn bit is set
FGD_R/G/B = Foreground color, flipped with background when Inv bit is set

************************************************************************/

module vga_text_avl_interface (
	// Avalon Clock Input, note this clock is also used for VGA, so this must be 50Mhz
	// We can put a clock divider here in the future to make this IP more generalizable
	input logic CLK,
	
	// Avalon Reset Input
	input logic RESET,
	
	// Avalon-MM Slave Signals
	input  logic AVL_READ,					// Avalon-MM Read
	input  logic AVL_WRITE,					// Avalon-MM Write
	input  logic AVL_CS,					// Avalon-MM Chip Select
	input  logic [3:0] AVL_BYTE_EN,			// Avalon-MM Byte Enable
	input  logic [11:0] AVL_ADDR,			// Avalon-MM Address
	input  logic [31:0] AVL_WRITEDATA,		// Avalon-MM Write Data
	output logic [31:0] AVL_READDATA,		// Avalon-MM Read Data
	
	// Exported Conduit (mapped to VGA port - make sure you export in Platform Designer)
	output logic [3:0]  red, green, blue,	// VGA color channels (mapped to output pins in top-level)
	output logic hs, vs						// VGA HS/VS
);

logic[31:0] LOCAL_REG [8];
//put other local variables here
integer i;
logic VGA_Clk, blank, sync, sprite_data, sprite_addr;
logic[9:0] DrawX, DrawY;
logic AVL_READ_SEL;
logic AVL_WRITE_SEL;
logic[10:0] AVL_ADDR_READ;
assign AVL_READ_SEL = AVL_READ & AVL_CS & ~(AVL_ADDR[11]);
assign AVL_WRITE_SEL = AVL_WRITE & AVL_CS & ~(AVL_ADDR[11]);
logic AVL_WRITE_REG;
logic AVL_READ_REG;
assign AVL_WRITE_REG = AVL_WRITE & AVL_CS & AVL_ADDR[11];
assign AVL_READ_REG = AVL_READ & AVL_CS & AVL_ADDR[11];
logic[31:0] VRAMreadData;
logic[31:0] REGreadData;
assign AVL_ADDR_READ = AVL_ADDR[10:0];

//Declare submodules..e.g. VGA controller, ROMS, etc
vga_controller vga (.*, .Clk(CLK), .Reset(RESET), .pixel_clk(VGA_Clk));
font_rom rom (.addr(font_addr), .data(font_data));

ram vram (.address_a(AVL_ADDR_READ), .byteena_a(AVL_BYTE_EN), .clock(CLK), .data_a(AVL_WRITEDATA), .rden_a(AVL_READ_SEL), .wren_a(AVL_WRITE_SEL), .q_a(VRAMreadData), 
			 .address_b(regIndex), .data_b(32'b0), .rden_b(1'b1), .wren_b(1'b0), .q_b(ramOut));

always_ff @(posedge CLK) begin
    if(RESET)
        begin
            //clears the values in LOCAL_REG to hex 0
				for(i = 0; i < 8; i++)
					LOCAL_REG[i] <= 32'b0;
        end
        
    else if(AVL_WRITE_REG) 
    begin
        case(AVL_BYTE_EN)
            4'b1111: LOCAL_REG[AVL_ADDR_READ] <= AVL_WRITEDATA;    //full 32 bits
            4'b1100: LOCAL_REG[AVL_ADDR_READ][31:16] <= AVL_WRITEDATA[31:16];    //two upper bytes
            4'b0011: LOCAL_REG[AVL_ADDR_READ][15:0] <= AVL_WRITEDATA[15:0];    //two lower bytes
            4'b1000: LOCAL_REG[AVL_ADDR_READ][31:24] <= AVL_WRITEDATA[31:24];    //byte 3 only
            4'b0100: LOCAL_REG[AVL_ADDR_READ][23:16] <= AVL_WRITEDATA[23:16];    //byte 2 only
            4'b0010: LOCAL_REG[AVL_ADDR_READ][15:8] <= AVL_WRITEDATA[15:8];    //byte 1 only
            4'b0001: LOCAL_REG[AVL_ADDR_READ][7:0] <= AVL_WRITEDATA[7:0];        //byte 0 only
            default:
                LOCAL_REG[AVL_ADDR] <= 32'h0;
			endcase
	 end
    else if(AVL_READ_REG)
	 begin
		REGreadData <= LOCAL_REG[AVL_ADDR_READ];
	 end

end

always_comb begin
	case(AVL_ADDR[11])
	1'b1: AVL_READDATA = REGreadData;
	1'b0: AVL_READDATA = VRAMreadData;
	endcase
end


//handle drawing (may either be combinational or sequential - or both).
logic[5:0] col;
logic[5:0] row;
logic[5:0] colIndex;
logic[10:0] regIndex;
logic charIndex;
logic[15:0] regVal;
logic[3:0] charRow;
logic invert;
logic[3:0] current;
logic[3:0] fgRed, fgGreen, fgBlue, bgRed, bgGreen, bgBlue;
logic[10:0] font_addr;
logic[15:0] font_data;
logic[31:0] ramOut;
logic[3:0] fgIdx;
logic[3:0] bgIdx;
logic[2:0] fgReg;
logic[2:0] bgReg;
logic bgInReg, fgInReg;
always_comb
begin:find_font
	col = DrawX[9:4]; //character column
	row = DrawY[9:4]; //character row
	colIndex = col[5:1];	//which register in the row is it?
	charIndex = col[0]; //which char in the register is it?
	charRow = DrawY[3:0]; //current row of the character
	current = DrawX[3:0]; //current pixel in the row
	regIndex = colIndex + (10'd20 * row); //register index number
	fgReg = fgIdx[3:1]; //index of color register
	bgReg = bgIdx[3:1]; //index of bg color register
	fgInReg = fgIdx[0]; //which color in the reg is it?
	bgInReg = bgIdx[0]; //which bg color in the reg is it?
	case(charIndex) //get char data from vram
		1'b1: regVal = ramOut[31:16];
		1'b0: regVal = ramOut[15:0];
		default: regVal = 8'b0;
	endcase 
	invert = regVal[15]; //character invert bit
	font_addr[10:4] = regVal[14:8]; //find font for correct char
	font_addr[3:0] = charRow; //find font for correct line of char
	if(~invert) //handle color invert & get color indexes
	begin
		fgIdx = regVal[7:4];
		bgIdx = regVal[3:0];
	end
	else begin
		fgIdx = regVal[3:0];
		bgIdx = regVal[7:4];
	end
end
always_comb
begin:find_color
	if(fgInReg)
	begin
		fgRed = LOCAL_REG[fgReg][24:21];
		fgGreen = LOCAL_REG[fgReg][20:17];
		fgBlue = LOCAL_REG[fgReg][16:13];
	end
	else
	begin
		fgRed = LOCAL_REG[fgReg][12:9];
		fgGreen = LOCAL_REG[fgReg][8:5];
		fgBlue = LOCAL_REG[fgReg][4:1];
	end
	if(bgInReg)
	begin
		bgRed = LOCAL_REG[bgReg][24:21];
		bgGreen = LOCAL_REG[bgReg][20:17];
		bgBlue = LOCAL_REG[bgReg][16:13];
	end
	else
	begin
		bgRed = LOCAL_REG[bgReg][12:9];
		bgGreen = LOCAL_REG[bgReg][8:5];
		bgBlue = LOCAL_REG[bgReg][4:1];
	end
end
always_ff @(posedge VGA_Clk) begin
	if(~blank)
	begin
		red = 4'b0;
		green = 4'b0;
		blue = 4'b0;
	end
	else
	begin
		if(font_data[~current] == 1'b1)
		begin
			red = fgRed;
			green = fgGreen;
			blue = fgBlue;
		end
		else
		begin
			red = bgRed;
			green = bgGreen;
			blue = bgBlue;
		end
	end
end
endmodule
