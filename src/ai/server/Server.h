#pragma once

#include <AI.h>
#include <tree/TreeNode.h>
#include "Network.h"
#include "SelectHandler.h"
#include "PauseHandler.h"
#include "ResetHandler.h"
#include "StepHandler.h"
#include "ChangeHandler.h"

namespace ai {

class AIStateNode;

/**
 * @brief The server can serialize the state of the AI and broadcast it to all connected clients.
 *
 * If you start a server, you can add the @c AI instances to it by calling @c addAI. If you do so, make
 * sure to remove it when you remove that particular @c AI instance from your world. You should not do that
 * from different threads. The server should only be managed from one thread.
 *
 * The server will broadcast the world state - that is: It will send out an @c AIStateMessage to all connected
 * clients. If someone selected a particular @c AI instance by sending @c AISelectMessage to the server, it
 * will also broadcast an @c AICharacterDetailsMessage to all connected clients.
 */
class Server: public INetworkListener {
protected:
	typedef std::list<Zone*> ZoneList;
	typedef ZoneList::const_iterator ZoneConstIter;
	typedef ZoneList::iterator ZoneIter;
	ZoneList _zones;
	Network& _network;
	CharacterId _selectedCharacterId;
	long _time;
	SelectHandler _selectHandler;
	PauseHandler _pauseHandler;
	ResetHandler _resetHandler;
	StepHandler _stepHandler;
	ChangeHandler _changeHandler;
	bool _pause;

	void addChildren(const TreeNodePtr& node, AIStateNode& parent, AI& ai) const;
	void broadcastState();
	void broadcastCharacterDetails();
	void onConnect(Client* client) override;
public:
	Server(short port = 10001, const std::string& hostname = "0.0.0.0");
	virtual ~Server();

	/**
	 * @brief Start to listen on the specified port
	 */
	bool start();

	/**
	 * @brief Select a particular character (resp. @c AI instance) and send detail
	 * information to all the connected clients for this entity.
	 */
	void select(const ClientId& clientId, const CharacterId& id);
	/**
	 * @brief Will pause/unpause the execution of the behaviour trees for all watched @c AI instances.
	 */
	void pause(const ClientId& clientId, bool pause);

	/**
	 * @brief Performs one step of the ai in pause mode
	 */
	void step();

	/**
	 * @brief Resets the ai states
	 */
	void reset();
	/**
	 * @brief call this to update the server - should get called somewhere from your game tick
	 */
	void update(long deltaTime);
};

}
