describe("Aho-Corasick Library",function()
	local aho
	it("load the library",function()
		aho = require "lahocorasick"
		assert ( type ( aho ) == "table" )
		assert ( type ( aho.new ) == "function" )
	end)
	it("Works",function()
		local auto = aho.new()
		auto:add [[his]]
		auto:add [[hers]]
		auto:add [[she]]
		auto:add [[he]]
		auto:finalize ( )

		local str = "some hishe text"
		local found = 0
		auto:search ( str , function(s,e)
				found = found + 1
			end )
		assert.equal ( 3 , found )
	end)
end)
