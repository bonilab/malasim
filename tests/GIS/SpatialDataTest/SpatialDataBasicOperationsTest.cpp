#include <gtest/gtest.h>
#include "Spatial/GIS/SpatialData.h"
#include "TestHelpers.h"
#include <yaml-cpp/yaml.h>
#include "Configuration/Config.h"
#include "Simulation/Model.h"

class SpatialDataBasicOperationsTest : public ::testing::Test, protected SpatialDataTestHelper {
protected:
  void SetUp() override {
    SpatialDataTestHelper::SetUp();
  }
  void TearDown() override {
    SpatialDataTestHelper::TearDown();
  }
};

TEST_F(SpatialDataBasicOperationsTest, VerifyDistrictRasterState) {
  auto& spatial_data = SpatialData::get_instance();
  ASSERT_TRUE(spatial_data.using_raster);
  EXPECT_EQ(spatial_data.get_boundary("district")->unit_count, 2);
}

TEST_F(SpatialDataBasicOperationsTest, VerifyPopulationRasterProperties) {
  auto& spatial_data = SpatialData::get_instance();
  ASSERT_TRUE(spatial_data.using_raster);
  auto header = spatial_data.get_raster_header();
  EXPECT_EQ(header.number_columns, 3);
  EXPECT_EQ(header.number_rows, 3);
  EXPECT_FLOAT_EQ(header.cellsize, 1.0f);
}

TEST_F(SpatialDataBasicOperationsTest, VerifyCompleteConfigurationState) {
  auto& spatial_data = SpatialData::get_instance();
  ASSERT_TRUE(spatial_data.using_raster);
  EXPECT_EQ(spatial_data.get_boundary("district")->unit_count, 2);
}
