class CoatTail
	def initialize(fname)
		@fname = fname
	end

	def each_line(num_lines: 0, wait_indefinitely: true, &blk)
		each_line_ext(num_lines, wait_indefinitely, blk)
	end
end

require "coattail/coattail"
