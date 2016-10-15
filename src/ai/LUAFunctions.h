/**
 * @file
 * @ingroup LUA
 */
#pragma once

#include "common/LUA.h"

namespace ai {

static inline const char *lua_metaai() {
	return "__meta_ai";
}

static inline const char *lua_metazone() {
	return "__meta_zone";
}

static inline const char* lua_metaaggromgr() {
	return "__meta_aggromgr";
}

static inline const char* lua_metacharacter() {
	return "__meta_character";
}

static void lua_aisetupmetatable(lua_State* s, const std::string& type, const luaL_Reg *funcs, const std::string& name) {
	const std::string& metaFull = "__meta_" + name + "_" + type;
	// make global
	lua_setfield(s, LUA_REGISTRYINDEX, metaFull.c_str());
	// put back onto stack
	lua_getfield(s, LUA_REGISTRYINDEX, metaFull.c_str());

	// setup meta table - create a new one manually, otherwise we aren't
	// able to override the particular function on a per instance base. Also
	// this 'metatable' must not be in the global registry.
	lua_createtable(s, 0, 2);
	lua_pushvalue(s, -1);
	lua_setfield(s, -2, "__index");
	lua_pushstring(s, name.c_str());
	lua_setfield(s, -2, "__name");
	lua_pushstring(s, type.c_str());
	lua_setfield(s, -2, "type");
	luaL_setfuncs(s, funcs, 0);
	lua_setmetatable(s, -2);
}

static int lua_ainewindex(lua_State* s) {
	// -3 is userdata
	lua_getmetatable(s, -3);
	// -3 is now the field string
	const char *field = luaL_checkstring(s, -3);
	// push -2 to -1 (the value)
	lua_pushvalue(s, -2);
	// set the value into the field
	lua_setfield(s, -2, field);
	lua_pop(s, 1);
	return 0;
}

template<class T>
static inline T* lua_getaiudata(lua_State* s, int n, const char *name) {
	return *(T **) luaL_checkudata(s, n, name);
}

template<class T>
static inline T* lua_ainewuserdata(lua_State* s, T* ptr) {
	T ** udata = (T**) lua_newuserdata(s, sizeof(T*));
	*udata = ptr;
	return ptr;
}

template<class T>
static inline int lua_pushaiudata(lua_State* s, T* ptr, const char *name) {
	T ** udata = (T **) lua_newuserdata(s, sizeof(T *));
	luaL_getmetatable(s, name);
	if (!lua_istable(s, -1)) {
		ai_log_error("LUA: metatable for %s doesn't exist", name);
		return 0;
	}
	lua_setmetatable(s, -2);
	*udata = ptr;
	return 1;
}

/**
 * @see @ai{LUAAIRegistry::pushAIMetatable()}
 */
static AI* lua_ctxai(lua_State * s, int n) {
	return lua_getaiudata<AI>(s, n, lua_metaai());
}

static Zone* lua_ctxzone(lua_State * s, int n) {
	return lua_getaiudata<Zone>(s, n, lua_metazone());
}

static AggroMgr* lua_ctxaggromgr(lua_State * s, int n) {
	return lua_getaiudata<AggroMgr>(s, n, lua_metaaggromgr());
}

static ICharacter* lua_ctxcharacter(lua_State * s, int n) {
	return lua_getaiudata<ICharacter>(s, n, lua_metacharacter());
}

static int lua_pushzone(lua_State* s, Zone* zone) {
	return lua_pushaiudata<Zone>(s, zone, lua_metazone());
}

static int lua_pushaggromgr(lua_State* s, AggroMgr* aggroMgr) {
	return lua_pushaiudata<AggroMgr>(s, aggroMgr, lua_metaaggromgr());
}

static int lua_pushcharacter(lua_State* s, ICharacter* character) {
	return lua_pushaiudata<ICharacter>(s, character, lua_metacharacter());
}

static int lua_pushai(lua_State* s, AI* ai) {
	return lua_pushaiudata<AI>(s, ai, lua_metaai());
}

static int lua_zonetostring(lua_State* s) {
	const Zone* zone = lua_ctxzone(s, 1);
	lua_pushfstring(s, "zone: %s", zone->getName().c_str());
	return 1;
}

static int lua_zonesize(lua_State* s) {
	const Zone* zone = lua_ctxzone(s, 1);
	lua_pushinteger(s, zone->size());
	return 1;
}

static int lua_aggromgraddaggro(lua_State* s) {
	AggroMgr* aggroMgr = lua_ctxaggromgr(s, 1);
	const int chrId = luaL_checkinteger(s, 2);
	const float amount = luaL_checknumber(s, 3);
	aggroMgr->addAggro((CharacterId)chrId, amount);
	return 0;
}

static int lua_aggromgrtostring(lua_State* s) {
	lua_pushliteral(s, "aggroMgr");
	return 1;
}

static int lua_characterid(lua_State* s) {
	const ICharacter* chr = lua_ctxcharacter(s, 1);
	lua_pushinteger(s, chr->getId());
	return 1;
}

static int lua_characterspeed(lua_State* s) {
	const ICharacter* chr = lua_ctxcharacter(s, 1);
	lua_pushnumber(s, chr->getSpeed());
	return 1;
}

static int lua_characterorientation(lua_State* s) {
	const ICharacter* chr = lua_ctxcharacter(s, 1);
	lua_pushnumber(s, chr->getOrientation());
	return 1;
}

static int lua_charactersetspeed(lua_State* s) {
	ICharacter* chr = lua_ctxcharacter(s, 1);
	const float value = luaL_checknumber(s, 2);
	chr->setSpeed(value);
	return 0;
}

static int lua_charactersetorientation(lua_State* s) {
	ICharacter* chr = lua_ctxcharacter(s, 1);
	const float value = luaL_checknumber(s, 2);
	chr->setOrientation(value);
	return 0;
}

static int lua_characterattributes(lua_State* s) {
	const ICharacter* chr = lua_ctxcharacter(s, 1);
	const CharacterAttributes& attributes = chr->getAttributes();

	lua_newtable(s);
	const int top = lua_gettop(s);

	for (auto it = attributes.begin(); it != attributes.end(); ++it) {
		const std::string& key = it->first;
		const std::string& value = it->second;
		lua_pushlstring(s, key.c_str(), key.size());
		lua_pushlstring(s, value.c_str(), value.size());
		lua_settable(s, top);
	}
	return 1;
}

static int lua_charactersetattribute(lua_State* s) {
	ICharacter* chr = lua_ctxcharacter(s, 1);
	const char* key = luaL_checkstring(s, 2);
	const char* value = luaL_checkstring(s, 3);
	chr->setAttribute(key, value);
	return 0;
}

static int lua_charactertostring(lua_State* s) {
	ICharacter* character = lua_ctxcharacter(s, 1);
	lua_pushfstring(s, "Character: %d", (lua_Integer)character->getId());
	return 1;
}

static int lua_aiid(lua_State* s) {
	const AI* ai = lua_ctxai(s, 1);
	lua_pushinteger(s, ai->getId());
	return 1;
}

static int lua_aitime(lua_State* s) {
	const AI* ai = lua_ctxai(s, 1);
	lua_pushinteger(s, ai->getTime());
	return 1;
}

static int lua_aigetzone(lua_State* s) {
	const AI* ai = lua_ctxai(s, 1);
	return lua_pushzone(s, ai->getZone());
}

static int lua_aigetaggromgr(lua_State* s) {
	AI* ai = lua_ctxai(s, 1);
	return lua_pushaggromgr(s, &ai->getAggroMgr());
}

static int lua_aigetcharacter(lua_State* s) {
	const AI* ai = lua_ctxai(s, 1);
	return lua_pushcharacter(s, ai->getCharacter().get());
}

static int lua_aihaszone(lua_State* s) {
	const AI* ai = lua_ctxai(s, 1);
	lua_pushboolean(s, ai->hasZone() ? 1 : 0);
	return 1;
}

static int lua_aitostring(lua_State* s) {
	const AI* ai = lua_ctxai(s, 1);
	TreeNodePtr treeNode = ai->getBehaviour();
	if (treeNode) {
		lua_pushfstring(s, "ai: %s", treeNode->getName().c_str());
	} else {
		lua_pushstring(s, "ai: no behaviour tree set");
	}
	return 1;
}

}
