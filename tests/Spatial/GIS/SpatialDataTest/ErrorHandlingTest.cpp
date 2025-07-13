#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>

#include <fstream>

#include "Spatial/GIS/SpatialData.h"
#include "TestHelpers.h"

class ErrorHandlingTest : public ::testing::Test, protected SpatialDataTestHelper {
protected:
  void SetUp() override { SpatialDataTestHelper::SetUp(); }

  void TearDown() override { SpatialDataTestHelper::TearDown(); }

  void create_mismatched_raster(const std::string &filename) {
    std::ofstream file(filename);
    file << "ncols 4\n";
    file << "nrows 4\n";
    file << "xllcorner 0.0\n";
    file << "yllcorner 0.0\n";
    file << "cellsize 1.0\n";
    file << "NODATA_value -9999\n";
    file << "1 1 1 1\n";
    file << "1 1 1 1\n";
    file << "1 1 1 1\n";
    file << "1 1 1 1\n";
    file.close();
  }

  void create_invalid_district_raster(const std::string &filename) {
    std::ofstream file(filename);
    file << "ncols 3\nnrows 3\nxllcorner 0.0\nyllcorner 0.0\ncellsize 1.0\nNODATA_value -9999\n";
    file << "1 1 3\n";
    file << "1 3 3\n";
    file << "-9999 3 3\n";
    file.close();
  }
};

TEST_F(ErrorHandlingTest, MissingFiles) {
  auto node = YAML::Node();
  node["district_raster"] = "nonexistent.asc";
  EXPECT_THROW(spatial_data->process_config(node), std::runtime_error);
}

TEST_F(ErrorHandlingTest, InvalidDistrictNumbering) {
  create_invalid_district_raster("test_invalid.asc");
  create_population_raster("test_population.asc");
  auto node = createBasicNode();
  node["district_raster"] = "test_invalid.asc";
  EXPECT_THROW(spatial_data->process_config(node), std::runtime_error);
  std::remove("test_invalid.asc");
  std::remove("test_population.asc");
}

TEST_F(ErrorHandlingTest, MismatchedRasterDimensions) {
  create_district_raster("test_district.asc");
  create_mismatched_raster("test_mismatch.asc");
  auto node = createBasicNode();
  node["population_raster"] = "test_mismatch.asc";
  EXPECT_THROW(spatial_data->process_config(node), std::runtime_error);
  std::remove("test_district.asc");
  std::remove("test_mismatch.asc");
}

TEST_F(ErrorHandlingTest, InvalidLocationAccess) {
  EXPECT_THROW(
      { [[maybe_unused]] auto district = spatial_data->get_admin_unit("district", 999); },
      std::out_of_range);
}

