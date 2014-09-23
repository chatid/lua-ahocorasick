#include <lua.h>
#include <lauxlib.h>

#include "ahocorasick.h"

#define AHO_METATABLE_KEY "lahocorasick"

typedef struct {
	lua_State* L;
	int cb;
} lahocorasick_cb_data;

int lahocorasick_cb (AC_MATCH_t* match, void* udata) {
	int i;
	for ( i=0 ; i<match->match_num ; i++ ) {
		lua_pushvalue ( (lua_State*)udata , 3 ); /* Re-push function */
		lua_pushnumber ( (lua_State*)udata , match->position-match->patterns[i].length+1 ); /* Push start position */
		lua_pushnumber ( (lua_State*)udata , match->position ); /* Push finish position */
		lua_call ( (lua_State*)udata , 2 , 1 );
		if ( lua_toboolean ( (lua_State*)udata , -1 ) ) return 1;
		lua_pop ( (lua_State*)udata , 1 );
	}
	return 0;
}

static int lahocorasick_new ( lua_State* L ) {
	*(AC_AUTOMATA_t**)lua_newuserdata (L,sizeof(AC_AUTOMATA_t*)) = ac_automata_init(&lahocorasick_cb);
	luaL_getmetatable ( L , AHO_METATABLE_KEY );
	lua_setmetatable ( L , -2 );
	return 1;
}

static int lahocorasick_tostring ( lua_State* L ) {
	lua_pushfstring ( L , "ahocorasick automata: %p" , lua_topointer ( L , 1 ) );
	return 1;
}

static int lahocorasick_add ( lua_State* L ) {
	AC_AUTOMATA_t* m = *(AC_AUTOMATA_t**)luaL_checkudata ( L , 1 , AHO_METATABLE_KEY );
	AC_PATTERN_t pat;
	size_t len;
	pat.astring = lua_tolstring(L, 2, &len);
	pat.length = len;

	switch ( ac_automata_add ( m , &pat ) ) {
		case ACERR_SUCCESS:
			lua_pushboolean ( L , 1 );
			return 1;
		case ACERR_DUPLICATE_PATTERN:
			lua_pushliteral ( L , "duplicate pattern" );
			break;
		case ACERR_LONG_PATTERN:
			lua_pushliteral ( L , "long pattern" );
			break;
		case ACERR_ZERO_PATTERN:
			lua_pushliteral ( L , "zero pattern" );
			break;
		case ACERR_AUTOMATA_CLOSED:
			lua_pushliteral ( L , "automata closed" );
			break;
	}
	return lua_error ( L );
}

static int lahocorasick_finalize ( lua_State* L ) {
	AC_AUTOMATA_t* m = *(AC_AUTOMATA_t**)luaL_checkudata ( L , 1 , AHO_METATABLE_KEY );
	ac_automata_finalize ( m );
	return 0;
}

static int lahocorasick_search ( lua_State* L ) {
	int cb;
	AC_AUTOMATA_t* m = *(AC_AUTOMATA_t**)luaL_checkudata ( L , 1 , AHO_METATABLE_KEY );
	AC_TEXT_t txt;
	size_t len;
	txt.astring = lua_tolstring(L, 2, &len);
	txt.length = len;
	lua_settop ( L , 3 );
	lua_pushnumber ( L , ac_automata_search ( m , &txt , (void*)L ) );
	return 1;
}

static int lahocorasick_reset ( lua_State* L ) {
	AC_AUTOMATA_t* m = *(AC_AUTOMATA_t**)luaL_checkudata ( L , 1 , AHO_METATABLE_KEY );
	ac_automata_reset ( m );
	return 0;
}

static int lahocorasick_release ( lua_State* L ) {
	AC_AUTOMATA_t* m = *(AC_AUTOMATA_t**)luaL_checkudata ( L , 1 , AHO_METATABLE_KEY );
	ac_automata_release ( m );
	return 0;
}

static int lahocorasick_display ( lua_State* L ) {
	AC_AUTOMATA_t* m = *(AC_AUTOMATA_t**)luaL_checkudata ( L , 1 , AHO_METATABLE_KEY );
	ac_automata_display ( m , 0 );
	return 0;
}

static const struct luaL_Reg lahocorasick [] = {
	{ "new" , lahocorasick_new },
	{ NULL , NULL }
};
static const struct luaL_Reg lahocorasick_meta [] = {
	{ "__tostring" , lahocorasick_tostring },
	{ "__gc" , lahocorasick_release },
	{ NULL , NULL }
};
static const struct luaL_Reg lahocorasick_methods [] = {
	{ "add" , lahocorasick_add },
	{ "finalize" , lahocorasick_finalize },
	{ "search" , lahocorasick_search },
	{ "reset" , lahocorasick_reset },
	{ "display" , lahocorasick_display },
	{ NULL , NULL }
};

extern int luaopen_ahocorasick ( lua_State*L ) {
	lua_newtable ( L );
	luaL_register ( L , NULL , lahocorasick );

	luaL_newmetatable ( L , AHO_METATABLE_KEY );
	luaL_register ( L , NULL , lahocorasick_meta );
		lua_newtable ( L );
		luaL_register ( L , NULL , lahocorasick_methods );
		lua_setfield ( L , -2 , "__index" );
	lua_pop(L,1);

	return 1;
}
