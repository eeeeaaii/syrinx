


main
[[
	@f: { func1 att1="x" }
	@osc1: { osc level="0.2" freq="750.0"}
	@output: { fileout filename="out.raw" }

	@osc1 \ @f \ @output < <
]]



@@in | func1 | @@out
	att1=#str
[[
	> @@in / { filt a=#str } \ @@out <
]]
