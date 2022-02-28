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
	always @(posedge clk)begin
		if (!srst_n)
			d_ff <= 1'b0;
		else
			d_ff <= din;
	end

	always_ff @(posedge clk) begin

		if (posedge_trigger == 1'b0 && negedge_trigger == 1'b0)begin
			edge_p <= 1'b0;
		end
		else if (posedge_trigger == 1'b1 && negedge_trigger == 1'b0) begin 
			edge_p <= din & ~d_ff;
		end
		else if (posedge_trigger == 1'b0 && negedge_trigger == 1'b1) begin 
			edge_p <=(~din & d_ff);
		end
		else
			edge_p <= (din & ~d_ff) | (~din & d_ff);


  	end
  
	
endmodule
