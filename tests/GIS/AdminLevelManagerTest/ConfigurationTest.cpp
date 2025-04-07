#include <gtest/gtest.h>

#include "Spatial/GIS/AscFile.h"
#include "TestHelpers.h"

class ConfigurationTest : public AdminLevelManagerTestHelper, public ::testing::Test {
protected:
  void SetUp() override { AdminLevelManagerTestHelper::SetUp(); }

  void TearDown() override { AdminLevelManagerTestHelper::TearDown(); }
};

TEST_F(ConfigurationTest, ZeroBasedIndexing) {
  create_test_raster("test_district.asc", true);  // zero-based

  SpatialData::get_instance().load("test_district.asc", SpatialData::DISTRICTS);
  SpatialData::get_instance().using_raster = true;
  std::string errors;
  if (SpatialData::get_instance().check_catalog(errors)) { throw std::runtime_error(errors); }
  auto district_raster = SpatialData::get_instance().get_raster(SpatialData::DISTRICTS);
  SpatialData::get_instance().generate_locations(district_raster);
  SpatialData::get_instance().parse_complete();
  auto raster = std::make_unique<AscFile>(*district_raster);
  EXPECT_NO_THROW(manager.register_level("district"));
  EXPECT_NO_THROW(manager.setup_boundary("district", std::move(raster).get()));
  raster.reset();

  const auto* boundary = manager.get_boundary("district");
  ASSERT_NE(boundary, nullptr);
  EXPECT_EQ(boundary->min_unit_id, 0);
  EXPECT_EQ(boundary->max_unit_id, 2);
  EXPECT_EQ(boundary->unit_count, 3);
}

TEST_F(ConfigurationTest, OneBasedIndexing) {
  create_test_raster("test_district.asc", false);  // one-based

  SpatialData::get_instance().load("test_district.asc", SpatialData::DISTRICTS);
  SpatialData::get_instance().using_raster = true;
  std::string errors;
  if (SpatialData::get_instance().check_catalog(errors)) { throw std::runtime_error(errors); }
  auto district_raster = SpatialData::get_instance().get_raster(SpatialData::DISTRICTS);
  SpatialData::get_instance().generate_locations(district_raster);
  SpatialData::get_instance().parse_complete();
  auto raster = std::make_unique<AscFile>(*district_raster);
  EXPECT_NO_THROW(manager.register_level("district"));
  EXPECT_NO_THROW(manager.setup_boundary("district", std::move(raster).get()));
  raster.reset();

  const auto* boundary = manager.get_boundary("district");
  ASSERT_NE(boundary, nullptr);
  EXPECT_EQ(boundary->min_unit_id, 1);
}

TEST_F(ConfigurationTest, DifferentRasterDimensions) {
  create_test_raster("test_district.asc", true, 5, 4);  // 5x4 raster

  SpatialData::get_instance().reset_raster_info();
  SpatialData::get_instance().load("test_district.asc", SpatialData::DISTRICTS);
  SpatialData::get_instance().using_raster = true;
  std::string errors;
  if (SpatialData::get_instance().check_catalog(errors)) { throw std::runtime_error(errors); }
  auto district_raster = SpatialData::get_instance().get_raster(SpatialData::DISTRICTS);
  SpatialData::get_instance().generate_locations(district_raster);
  SpatialData::get_instance().parse_complete();
  auto raster = std::make_unique<AscFile>(*district_raster);
  EXPECT_NO_THROW(manager.register_level("district"));
  EXPECT_NO_THROW(manager.setup_boundary("district", std::move(raster).get()));
  raster.reset();

  const auto* boundary = manager.get_boundary("district");
  ASSERT_NE(boundary, nullptr);
  EXPECT_EQ(boundary->min_unit_id, 0);
  EXPECT_EQ(boundary->max_unit_id, 2);
  EXPECT_EQ(boundary->unit_count, 3);
}

