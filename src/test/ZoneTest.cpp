#include "ZoneTest.h"

class ZoneTest: public TestSuite {
protected:
	void massAdd(int n) {
		ai::Zone zone("test1");
		ai::TreeNodePtr root = std::make_shared<ai::PrioritySelector>("test", "", ai::True::get());
		for (int i = 0; i < n; ++i) {
			ai::ICharacterPtr character = std::make_shared<TestEntity>(i);
			ai::AIPtr ai = std::make_shared<ai::AI>(root);
			ai->setCharacter(character);
			ASSERT_TRUE(zone.addAI(ai)) << "Could not add ai to the zone";
		}
		zone.update(0l);
		ASSERT_EQ(n, (int)zone.size());
	}
};

TEST_F(ZoneTest, testChanges) {
	ai::Zone zone("test1");
	ai::TreeNodePtr root = std::make_shared<ai::PrioritySelector>("test", "", ai::True::get());
	ai::ICharacterPtr character = std::make_shared<TestEntity>(1);
	ai::AIPtr ai = std::make_shared<ai::AI>(root);
	ai->setCharacter(character);

	ai::ICharacterPtr character2 = std::make_shared<TestEntity>(2);
	ai::AIPtr ai2 = std::make_shared<ai::AI>(root);
	ai2->setCharacter(character2);

	ASSERT_TRUE(zone.addAI(ai)) << "Could not add ai to the zone";
	zone.setDebug(true);
	zone.update(1);
	ASSERT_TRUE(ai->isDebuggingActive()) << "Debug is not active for the entity";
	ASSERT_TRUE(zone.addAI(ai2)) << "Could not add ai to the zone";
	zone.update(1);
	ASSERT_TRUE(ai2->isDebuggingActive()) << "Debug is not active for newly added entity";
	zone.setDebug(false);
	zone.update(1);
	ASSERT_FALSE(ai->isDebuggingActive()) << "Debug is still active for entity";
	ASSERT_FALSE(ai2->isDebuggingActive()) << "Debug is still active for newly added entity";
	ASSERT_TRUE(zone.removeAI(ai)) << "Could not remove ai from zone";
	ASSERT_TRUE(zone.removeAI(ai2)) << "Could not remove ai from zone";
}

TEST_F(ZoneTest, testMassAdd10) {
	massAdd(10);
}

TEST_F(ZoneTest, testMassAdd100) {
	massAdd(100);
}

TEST_F(ZoneTest, testMassAdd1000) {
	massAdd(1000);
}

TEST_F(ZoneTest, testMassAdd10000) {
	massAdd(10000);
}

TEST_F(ZoneTest, testMassAdd100000) {
	massAdd(100000);
}

TEST_F(ZoneTest, testMassAdd1000000) {
	massAdd(1000000);
}
