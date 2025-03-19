#include <gtest/gtest.h>
#include "Spatial/GIS/SpatialData.h"
#include "TestHelpers.h"
#include <yaml-cpp/yaml.h>
#include "Configuration/Config.h"
#include "Simulation/Model.h"

class DistrictOperationsTest : public ::testing::Test, protected SpatialDataTestHelper {
protected:
  void SetUp() override {
    SpatialDataTestHelper::SetUp();
  }
  void TearDown() override {
    SpatialDataTestHelper::TearDown();
  }
};

TEST_F(DistrictOperationsTest, GetDistrictForLocation) {
  auto& spatial_data = SpatialData::get_instance();
  EXPECT_EQ(spatial_data.get_district(0), 1);  // First cell, district 1
  EXPECT_EQ(spatial_data.get_district(2), 2);  // Third cell, district 2
  EXPECT_EQ(spatial_data.get_district(4), 2);  // Fifth cell, district 2
}

TEST_F(DistrictOperationsTest, GetDistrictLocations) {
  auto& spatial_data = SpatialData::get_instance();
  auto locations_d1 = spatial_data.get_district_locations(1);
  EXPECT_EQ(locations_d1.size(), 3);  // District 1 has 3 cells
  auto locations_d2 = spatial_data.get_district_locations(2);
  EXPECT_EQ(locations_d2.size(), 5);  // District 2 has 5 cells
}
