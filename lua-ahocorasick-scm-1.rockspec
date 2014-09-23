package = "lua-ahocorasick"
version = "scm-1"
source = {
	url = "git://github.com/chatid/lua-ahocorasick.git";
}
description = {
	summary = "This is a lua binding to MultiFast, which is used for quickly and efficiently searching text for multiple strings." ;
	license = "MIT" ;
}
dependencies = {
	"lua >= 5.1"
}
external_dependencies = {
	MULTIFAST = {
		header = "ahocorasick.h"
	}
}
build = {
	type = "builtin" ;
	modules = {
		[package] = {
			sources = { "src/lahocorasick.c" } ;
			libraries = { "ahocorasick" } ;
			incdirs = { "$(MULTIFAST_INCDIR)" } ;
		} ;
	} ;
}
