#include <gtest/gtest.h>

#include "Spatial/GIS/AscFile.h"
#include "TestHelpers.h"

class BoundaryValidationTest : public AdminLevelManagerTestHelper, public ::testing::Test {
protected:
  void SetUp() override { AdminLevelManagerTestHelper::SetUp(); }

  void TearDown() override { AdminLevelManagerTestHelper::TearDown(); }
};

TEST_F(BoundaryValidationTest, NullRasterHandling) {
  EXPECT_NO_THROW(manager.register_level("district"));
  EXPECT_THROW(manager.setup_boundary("district", nullptr), std::runtime_error);
}

TEST_F(BoundaryValidationTest, InvalidRasterDimensions) {
  std::ofstream file("test_invalid.asc");
  file << "ncols 0\n";  // Invalid dimension
  file << "nrows 3\n";
  file << "xllcorner 0.0\nyllcorner 0.0\ncellsize 1.0\nNODATA_value -9999\n";
  file << "1 1 1\n1 1 1\n1 1 1\n";
  file.close();
  Model::get_spatial_data()->load("test_invalid.asc", SpatialData::DISTRICTS);
  Model::get_spatial_data()->set_using_raster(true);
  std::string errors;
  EXPECT_TRUE(Model::get_spatial_data()->check_catalog(errors));
  Model::get_spatial_data()->set_using_raster(false);
  Model::get_spatial_data()->parse_complete();
}

TEST_F(BoundaryValidationTest, InvalidIndexingValues) {
  // Create raster with invalid starting index (2)
  std::vector<std::vector<int>> invalid_values = {{2, 2, 3}, {2, 3, 3}, {2, 3, 3}};
  create_custom_raster("test_invalid.asc", invalid_values);

  Model::get_spatial_data()->load("test_invalid.asc", SpatialData::DISTRICTS);
  Model::get_spatial_data()->set_using_raster(true);
  // Check to make sure our data is OK
  std::string errors;
  if (Model::get_spatial_data()->check_catalog(errors)) { throw std::runtime_error(errors); }
  auto district_raster = Model::get_spatial_data()->get_raster(SpatialData::DISTRICTS);
  Model::get_spatial_data()->generate_locations(district_raster);
  Model::get_spatial_data()->set_using_raster(false);
  auto raster = std::make_unique<AscFile>(*district_raster);
  EXPECT_THROW(manager.setup_boundary("district", raster.get()), std::runtime_error);
  Model::get_spatial_data()->parse_complete();
}

TEST_F(BoundaryValidationTest, AllNODATAValues) {
  std::vector<std::vector<int>> nodata_values = {
      {-9999, -9999, -9999}, {-9999, -9999, -9999}, {-9999, -9999, -9999}};
  create_custom_raster("test_invalid.asc", nodata_values);

  Model::get_spatial_data()->load("test_invalid.asc", SpatialData::DISTRICTS);
  Model::get_spatial_data()->set_using_raster(true);
  // Check to make sure our data is OK
  std::string errors;
  if (Model::get_spatial_data()->check_catalog(errors)) { throw std::runtime_error(errors); }
  auto district_raster = Model::get_spatial_data()->get_raster(SpatialData::DISTRICTS);
  EXPECT_THROW(Model::get_spatial_data()->generate_locations(district_raster), std::runtime_error);
  Model::get_spatial_data()->parse_complete();
}

TEST_F(BoundaryValidationTest, SameDimensionsAreAccepted) {
  // Create district raster (3x3)
  std::vector<std::vector<int>> district_values = {{1, 1, 2}, {1, 2, 2}, {3, 3, 3}};
  create_custom_raster("test_district.asc", district_values);

  // Create province raster with same dimensions
  std::vector<std::vector<int>> province_values = {{1, 1, 1}, {1, 1, 1}, {2, 2, 2}};
  create_custom_raster("test_province.asc", province_values);

  Model::get_spatial_data()->load("test_district.asc", SpatialData::DISTRICTS);
  Model::get_spatial_data()->set_using_raster(true);
  std::string errors;
  if (Model::get_spatial_data()->check_catalog(errors)) { throw std::runtime_error(errors); }
  auto district_raster = Model::get_spatial_data()->get_raster(SpatialData::DISTRICTS);
  Model::get_spatial_data()->generate_locations(district_raster);
  Model::get_spatial_data()->parse_complete();
  auto raster = std::make_unique<AscFile>(*district_raster);
  EXPECT_NO_THROW(manager.register_level("district"));
  EXPECT_NO_THROW(manager.setup_boundary("district", std::move(raster).get()));

  Model::get_spatial_data()->load("test_district.asc", SpatialData::DISTRICTS);
  Model::get_spatial_data()->set_using_raster(true);
  if (Model::get_spatial_data()->check_catalog(errors)) { throw std::runtime_error(errors); }
  auto province_raster = Model::get_spatial_data()->get_raster(SpatialData::DISTRICTS);
  Model::get_spatial_data()->generate_locations(province_raster);
  Model::get_spatial_data()->parse_complete();
  raster = std::make_unique<AscFile>(*province_raster);
  EXPECT_NO_THROW(manager.register_level("province"));
  EXPECT_NO_THROW(manager.setup_boundary("province", std::move(raster).get()));
  raster.reset();
}

TEST_F(BoundaryValidationTest, DifferentDimensionsAreRejected) {
  // Create district raster (3x3)
  std::vector<std::vector<int>> district_values = {{1, 1, 2}, {1, 2, 2}, {3, 3, 3}};
  create_custom_raster("test_district.asc", district_values);

  Model::get_spatial_data()->load("test_district.asc", SpatialData::DISTRICTS);
  Model::get_spatial_data()->set_using_raster(true);
  std::string errors;
  if (Model::get_spatial_data()->check_catalog(errors)) { throw std::runtime_error(errors); }
  auto district_raster = Model::get_spatial_data()->get_raster(SpatialData::DISTRICTS);
  Model::get_spatial_data()->generate_locations(district_raster);
  Model::get_spatial_data()->parse_complete();
  auto raster = std::make_unique<AscFile>(*district_raster);
  EXPECT_NO_THROW(manager.register_level("district"));
  EXPECT_NO_THROW(manager.setup_boundary("district", std::move(raster).get()));
  raster.reset();

  // Create province raster with different dimensions (4x4)
  std::vector<std::vector<int>> province_values = {
      {1, 1, 1, 1}, {1, 1, 1, 1}, {2, 2, 2, 2}, {2, 2, 2, 2}};
  create_custom_raster("test_province.asc", province_values);

  Model::get_spatial_data()->load("test_province.asc", SpatialData::DISTRICTS);
  Model::get_spatial_data()->set_using_raster(true);
  EXPECT_TRUE(Model::get_spatial_data()->check_catalog(errors));
  Model::get_spatial_data()->parse_complete();
}

TEST_F(BoundaryValidationTest, MissingDistrictLevel) {
  EXPECT_NO_THROW(manager.register_level("province"));
  EXPECT_THROW(manager.validate(), std::runtime_error);
}

TEST_F(BoundaryValidationTest, UninitializedBoundary) {
  EXPECT_NO_THROW(manager.register_level("district"));
  EXPECT_THROW(manager.validate(), std::runtime_error);
}

TEST_F(BoundaryValidationTest, ValidConfigurationWithDistrict) {
  create_test_raster("test_district.asc");

  Model::get_spatial_data()->load("test_district.asc", SpatialData::DISTRICTS);
  Model::get_spatial_data()->set_using_raster(true);
  std::string errors;
  if (Model::get_spatial_data()->check_catalog(errors)) { throw std::runtime_error(errors); }
  auto district_raster = Model::get_spatial_data()->get_raster(SpatialData::DISTRICTS);
  Model::get_spatial_data()->generate_locations(district_raster);
  Model::get_spatial_data()->parse_complete();
  auto raster = std::make_unique<AscFile>(*district_raster);
  EXPECT_NO_THROW(manager.register_level("district"));
  EXPECT_NO_THROW(manager.setup_boundary("district", std::move(raster).get()));
  EXPECT_NO_THROW(manager.validate());
  raster.reset();
}

