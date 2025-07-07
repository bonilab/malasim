#include "gtest/gtest.h"
#include "Population/ParasiteDensity/ParasiteDensityUpdateFunction.h"

// Dummy subclass for testing
class DummyParasiteDensityUpdateFunction : public ParasiteDensityUpdateFunction {
public:
    DummyParasiteDensityUpdateFunction() = default;
    double get_current_parasite_density(ClonalParasitePopulation* parasite, int duration) override {
        return duration * 2.0;
    }
};

TEST(ParasiteDensityUpdateFunctionTest, ConstructionAndDestruction) {
    EXPECT_NO_THROW({ DummyParasiteDensityUpdateFunction func; });
}

TEST(ParasiteDensityUpdateFunctionTest, GetCurrentParasiteDensity) {
    DummyParasiteDensityUpdateFunction func;
    EXPECT_DOUBLE_EQ(func.get_current_parasite_density(nullptr, 0), 0.0);
    EXPECT_DOUBLE_EQ(func.get_current_parasite_density(nullptr, 3), 6.0);
    EXPECT_DOUBLE_EQ(func.get_current_parasite_density(nullptr, -2), -4.0);
}
