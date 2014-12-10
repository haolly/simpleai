#pragma once

#include <AI.h>

namespace ai {

/**
 * @brief A zone represents one logical zone that groups Ais.
 */
class Zone {
public:
	typedef std::map<CharacterId, AI*> AIMap;
	typedef AIMap::const_iterator AIMapConstIter;
	typedef AIMap::iterator AIMapIter;

private:
	const std::string _name;
	AIMap _ais;
	bool _debug;

public:
	Zone(const std::string& name) :
			_name(name), _debug(false) {
	}

	/**
	 * @brief call then when you spawn a new @code AI that should be traceable via the debug viewer.
	 *
	 * @note Make sure to also call @c removeAI whenever you despawn the given @c AI instance
	 */
	bool addAI(AI& ai);
	bool removeAI(AI& ai);

	/**
	 * @brief Every zone has its own name that identifies it
	 */
	const std::string& getName() const;

	const AIMap& getAIMap() const;

	/**
	 * @brief Activate the debugging for this particular server instance
	 * @param[in] debug @c true if you want to activate the debugging and send
	 * the npc states of this server to all connected clients, or @c false if
	 * none of the managed entities is broadcasted.
	 */
	void setDebug(bool debug);

};

inline void Zone::setDebug (bool debug) {
	_debug = debug;
}

inline const std::string& Zone::getName() const {
	return _name;
}

inline const Zone::AIMap& Zone::getAIMap() const {
	return _ais;
}

}