#include <gtest/gtest.h>
#include "Spatial/GIS/SpatialData.h"
#include "TestHelpers.h"
#include <yaml-cpp/yaml.h>
#include "Configuration/Config.h"
#include "Simulation/Model.h"

class BasicOperationsTest : public ::testing::Test, protected SpatialDataTestHelper {
protected:
  void SetUp() override {
    SpatialDataTestHelper::SetUp();
  }
  void TearDown() override {
    SpatialDataTestHelper::TearDown();
  }
};

TEST_F(BasicOperationsTest, VerifyDistrictRasterState) {
  auto& spatial_data = SpatialData::get_instance();
  ASSERT_TRUE(spatial_data.using_raster);
  EXPECT_EQ(spatial_data.district_count, 3);  // Districts 1 and 2 and 3
}

TEST_F(BasicOperationsTest, VerifyPopulationRasterProperties) {
  auto& spatial_data = SpatialData::get_instance();
  ASSERT_TRUE(spatial_data.using_raster);
  auto header = spatial_data.get_raster_header();
  EXPECT_EQ(header.number_columns, 3);
  EXPECT_EQ(header.number_rows, 3);
  EXPECT_FLOAT_EQ(header.cellsize, 1.0f);
}

TEST_F(BasicOperationsTest, VerifyCompleteConfigurationState) {
  auto& spatial_data = SpatialData::get_instance();
  ASSERT_TRUE(spatial_data.using_raster);
  EXPECT_EQ(spatial_data.district_count, 3);
}
