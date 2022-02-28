module edge_det (
	clk,
	srst_n,
	din,
	posedge_trigger,
	negedge_trigger,
	edge_p
);
	input clk;
	input srst_n;
	input din;
	input posedge_trigger;
	input negedge_trigger;
	output reg edge_p;
	reg d_ff;
	always @(posedge clk or negedge srst_n)begin
		if (!srst_n)
			d_ff <= 1'b0;
		else
			d_ff <= din;
	end
	always @(posedge clk) begin
		if (posedge_trigger == 1'b1) begin 
			edge_p <= din & ~d_ff;
		end
		if (negedge_trigger == 1'b1) begin 
			edge_p <= ~din & d_ff;
		end
	end
	
endmodule
