@@note @@trig @@velo | flute
[[
	@osc1: { osc level="0.2" freq=@@note }
	@output: { fileout filename="out.raw" }

	@osc1 \ @output <
]]


@@in | foo | @@out
	att1=@@a
	att2=@@b
	att3=@@c
	att4=@@d
[[
	@o1: { osc freq=@@a }
	@o2: { osc freq=@@b }
	@o3: { osc freq=@@c }

	>
		@o1 @o2 @o3
		{osc freq=@@d}
		@@in
	/ {mix} \ {filt} \ @@out < <
]]

@@in | foo2 | @@out
	att1=@@a
	att2=@@b
	att3=@@c
	att4=@@d
[[
	@o1: { osc freq=@@a }
	@o2: { osc freq=@@b }
	@o3: { osc freq=@@c }

	> >
		@o1 @o2 @o3
		{osc freq=@@d}
		@@in
	/ {mix} / {filt} \ @@out <
]]

@@note @@trig @@velo | trumpet
[[
	@osc1: { osc level="0.2" freq=@@note }
	@output: { fileout filename="out.raw" }

	@osc1 \ @output <
]]

@@note @@trig @@velo | harp
[[
	@osc1: { osc level="0.2" freq=@@note }
	@output: { fileout filename="out.raw" }

	@osc1 \ @output <
]]

@@note @@trig @@velo | foo
[[
	@osc1: { osc level="0.2" freq=@@note }
	@output: { fileout filename="out.raw" }

	@osc1 \ @output <
]]

