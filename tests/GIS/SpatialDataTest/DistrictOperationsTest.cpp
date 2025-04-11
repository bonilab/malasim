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
  // auto& spatial_data = Model::get_spatial_data();
  EXPECT_EQ(spatial_data->get_admin_unit("district", 0), 1);  // First cell, district 1
  EXPECT_EQ(spatial_data->get_admin_unit("district", 2), 2);  // Third cell, district 2
  EXPECT_EQ(spatial_data->get_admin_unit("district", 4), 2);  // Fifth cell, district 2
}