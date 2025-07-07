#include "gtest/gtest.h"
#include "Population/ClinicalUpdateFunction.h"

// Dummy Model and ClonalParasitePopulation for isolation
class DummyModel {};

class DummyClinicalUpdateFunction : public ClinicalUpdateFunction {
public:
    DummyClinicalUpdateFunction() : ClinicalUpdateFunction(nullptr) {}
    double get_current_parasite_density(ClonalParasitePopulation* parasite, int duration) override {
        return duration * 10.0;
    }
};

TEST(ClinicalUpdateFunctionTest, ConstructionAndDestruction) {
    EXPECT_NO_THROW({ DummyClinicalUpdateFunction func; });
}

TEST(ClinicalUpdateFunctionTest, GetCurrentParasiteDensity) {
    DummyClinicalUpdateFunction func;
    EXPECT_DOUBLE_EQ(func.get_current_parasite_density(nullptr, 0), 0.0);
    EXPECT_DOUBLE_EQ(func.get_current_parasite_density(nullptr, 2), 20.0);
    EXPECT_DOUBLE_EQ(func.get_current_parasite_density(nullptr, -1), -10.0);
}
