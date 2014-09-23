# lua-ahocorasick

This is a lua binding to libahocorasick, part of [MultiFast](http://multifast.sourceforge.net/)

The [Aho-Corasick algorithm](https://en.wikipedia.org/wiki/Aho%E2%80%93Corasick_string_matching_algorithm) allows for fast and efficient string matching against large sets of strings.

## Usage

```lua
new_automata = require "lua-ahocorasick".new
```

First, an automata must be built up.
You need to add each word you want to search for to the automata.

```lua
my_automata = new_automata()

my_automata:add("some")
my_automata:add("strings")
my_automata:add("to")
my_automata:add("search")
my_automata:add("for")
```

Once the automata has been finalised, it can be used:

```lua
my_automata:finalize()
```

You can inspect the automata (prints to `stdout`):

```lua
my_automata:display()
```

### Finding a needle in a haystack

Without a callback, the result is a Boolean indicating if a match was found.

```lua
found = my_automata:search("a string with a word to find")
```

### Getting the matches

If called with a callback, it will be called for each match.
Return `true` from your callback if you want to stop searching.

```lua
local str = "A long string with some words in it to find."
did_break = my_automata:search(str, function(start_pos, end_pos, n_matches)
	print("match found at positions "..start_pos.." through "..end_pos..": "..str:sub(start_pos, end_pos))
	return nil -- Continue searching
end, false)
```

```
match found at positions 20 through 23: some
match found at positions 37 through 38: to
```

### Streaming input

If you pass `true` as the next argument, the search will continue from where it left off.

```lua
print("A")
my_automata:search("a series of stri", print)
print("B")
my_automata:search("ngs broken over multi", print, true)
print("C")
my_automata:search("ple packets in which t", print, true)
print("D")
my_automata:search("o search.", print, true)
print("E")
```

```
A
B
13	19	1
C
D
59	60	1
62	67	1
E
```
