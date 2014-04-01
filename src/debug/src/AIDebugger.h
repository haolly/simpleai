#pragma once

#include <vector>
#include <AI.h>
#include <server/AIStateMessage.h>
#include <server/AIPauseMessage.h>
#include <server/AICharacterDetailsMessage.h>
#include <server/AIStubTypes.h>
#include <server/IProtocolHandler.h>
#include <QTcpSocket>
#include <QApplication>
#include <QSettings>
#include <QFile>

namespace ai {
namespace debug {

class MapView;
class AIDebuggerWindow;

/**
 * @brief This is the remote debugger for the ai entities.
 *
 * You can extend this class and override AIDebugger::createMapWidget to create our own @c MapView instance
 * to render additional details to your characters or even the map the entities are spawned on.
 */
class AIDebugger: public QApplication {
	Q_OBJECT
friend class PauseHandler;
public:
	typedef std::vector<AIStateWorld> Entities;
	typedef Entities::const_iterator EntitiesIter;
protected:
	typedef Entities::iterator Iter;
	Entities _entities;

	ai::streamContainer _stream;

	ai::CharacterId _selectedId;
	std::vector<AIStateAggroEntry> _aggro;
	AIStateNode _node;
	CharacterAttributes _attributes;
	AIDebuggerWindow *_window;
	QTcpSocket _socket;
	bool _pause;

	bool writeMessage(const IProtocolMessage& msg);

private slots:
	void readTcpData();
	void onDisconnect();
public:
	AIDebugger(int argc, char** argv);
	virtual ~AIDebugger();

	/**
	 * @return The list of ai controlled entities
	 */
	const Entities& getEntities() const;
	void setEntities(const Entities& entities);
	void setCharacterDetails(const CharacterId& id, const AIStateAggro& aggro, const AIStateNode& node, const CharacterAttributes& attributes);
	/**
	 * @return The behaviour tree node that is assigned to the selected entity
	 */
	const AIStateNode& getNode() const;
	/**
	 * @return The key/value pairs of attributes that are assigned on the server side to the selected ai entity
	 */
	const CharacterAttributes& getAttributes() const;
	const std::vector<AIStateAggroEntry>& getAggro() const;

	/**
	 * @brief Start the debugger - call this from your main method
	 */
	int run();
	bool connectToAIServer(const QString& hostname, short port);

	bool isSelected(const ai::AIStateWorld& ai) const;
	const CharacterId& getSelected() const;
	void select(const ai::AIStateWorld& ai);
	void togglePause();
	void unselect();

	virtual MapView* createMapWidget();

signals:
	void onPause(bool pause);
};

inline void AIDebugger::setCharacterDetails(const CharacterId& id, const AIStateAggro& aggro, const AIStateNode& node, const CharacterAttributes& attributes) {
	_selectedId = id;
	_aggro = aggro.getAggro();
	_node = node;
	_attributes = attributes;
}

inline const std::vector<AIStateAggroEntry>& AIDebugger::getAggro() const {
	return _aggro;
}

inline const AIStateNode& AIDebugger::getNode() const {
	return _node;
}

inline const CharacterAttributes& AIDebugger::getAttributes() const {
	return _attributes;
}

inline void AIDebugger::setEntities(const Entities& entities) {
	_entities = entities;
}

inline const AIDebugger::Entities& AIDebugger::getEntities() const {
	return _entities;
}

}
}
