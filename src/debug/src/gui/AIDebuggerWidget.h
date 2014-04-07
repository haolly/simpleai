#pragma once

#include <QtGui>
#include <QLabel>
#include <QToolBar>
#include <QStatusBar>
#include <QMenu>

namespace ai {
namespace debug {

class StateTable;
class EntityList;
class AggroTable;
class NodeTreeView;
class ZoomFrame;
class MapView;
class AIDebugger;

class AIDebuggerWidget: public QWidget {
Q_OBJECT
public:
	AIDebuggerWidget(AIDebugger& debugger);
	virtual ~AIDebuggerWidget();

	void connectToAIServer(const QString& hostname, short port);

	void contributeToStatusBar(QStatusBar* statusBar);
	void contributeToToolBar(QToolBar* toolBar);
	void contributeToFileMenu(QMenu *fileMenu);
	void contributeToHelpMenu(QMenu *helpMenu);

	void removeFromStatusBar(QStatusBar* statusBar);
	void removeFromToolBar(QToolBar* toolBar);
	void removeFromFileMenu(QMenu *fileMenu);
	void removeFromHelpMenu(QMenu *helpMenu);


private slots:
	void about();
	void tick();
	void connectToAIServer();
	void requestPause();
	void setPause(bool pause);

private:
	void createView();
	void createActions();

	QWidget *createTopWidget();
	QWidget *createBottomWidget();
	QLabel *createLabel(const QString &text) const;

	NodeTreeView *_nodeTree;
	ZoomFrame *_nodeTreeFrame;
	StateTable *_stateTable;
	ZoomFrame *_mapFrame;
	MapView *_mapWidget;
	EntityList *_entityList;
	AggroTable *_aggroTable;
	QAction *_connectAction;
	QAction *_pauseAction;
	QAction *_aboutAction;
	QLabel *_statusBarLabel;
	QLabel *_selectedLabel;

	AIDebugger& _debugger;
};

}
}