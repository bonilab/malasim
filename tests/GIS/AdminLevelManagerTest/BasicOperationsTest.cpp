#include "TestHelpers.h"
#include <gtest/gtest.h>

#include "Simulation/Model.h"
#include "Spatial/GIS/AscFile.h"
#include "Spatial/Location/Location.h"

class BasicOperationsTest : public AdminLevelManagerTestHelper, public ::testing::Test {
protected:
    void SetUp() override {
        AdminLevelManagerTestHelper::SetUp();
    }

    void TearDown() override {
        AdminLevelManagerTestHelper::TearDown();
    }
};

TEST_F(BasicOperationsTest, RegisterNewAdministrativeLevel) {
    EXPECT_NO_THROW(manager.register_level("district", "Test District"));
    EXPECT_TRUE(manager.has_level("district"));
    EXPECT_TRUE(manager.has_district());
}

TEST_F(BasicOperationsTest, RegisterMultipleLevels) {
    EXPECT_NO_THROW(manager.register_level("district", "Test District"));
    EXPECT_NO_THROW(manager.register_level("province", "Test Province"));
    EXPECT_TRUE(manager.has_level("district"));
    EXPECT_TRUE(manager.has_level("province"));

    auto levels = manager.get_level_names();
    EXPECT_EQ(levels.size(), 2);
    EXPECT_EQ(levels[0], "district");
    EXPECT_EQ(levels[1], "province");
}

TEST_F(BasicOperationsTest, PreventDuplicateRegistration) {
    EXPECT_NO_THROW(manager.register_level("district"));
    EXPECT_THROW(manager.register_level("district"), std::runtime_error);
}

TEST_F(BasicOperationsTest, SetupDistrictBoundary) {
    std::vector<std::vector<int>> district_values = {
        {1, 1, 2},
        {1, 2, 2},
        {3, 3, 3}
    };
    create_custom_raster("test_district.asc", district_values);
    {
        std::ifstream test_file("test_district.asc");
        EXPECT_TRUE(test_file.good());
    }

    int district_id = manager.register_level("district");
    EXPECT_EQ(district_id, 0);
    EXPECT_TRUE(manager.has_level("district"));
    EXPECT_TRUE(manager.has_district());

  SpatialData::get_instance().load("test_district.asc",SpatialData::Districts);
    SpatialData::get_instance().using_raster = true;
    // Check to make sure our data is OK
    std::string errors;
    if (SpatialData::get_instance().check_catalog(errors)) {
      throw std::runtime_error(errors);
    }
    auto raw_raster = SpatialData::get_instance().get_raster(SpatialData::Districts);
    SpatialData::get_instance().generate_locations(raw_raster);
    SpatialData::get_instance().generate_distances();
    SpatialData::get_instance().populate_dependent_data();
    SpatialData::get_instance().parse_complete();
    EXPECT_NE(raw_raster, nullptr);
    EXPECT_EQ(raw_raster->NROWS, 3);
    EXPECT_EQ(raw_raster->NCOLS, 3);

    EXPECT_NE(Model::get_config(), nullptr);
    EXPECT_EQ(Model::get_instance().number_of_locations(), 9);
    EXPECT_EQ(Model::get_instance().location_db().size(), 9);
    EXPECT_NE(Model::get_instance().location_db()[0].coordinate, nullptr);

    auto raster = std::make_unique<AscFile>(*raw_raster);
    manager.setup_boundary("district", std::move(raster));

    EXPECT_EQ(manager.get_unit_count("district"), 3);
    EXPECT_EQ(manager.get_admin_unit(0, "district"), 1);
    EXPECT_EQ(manager.get_admin_unit(2, "district"), 2);
    EXPECT_EQ(manager.get_admin_unit(8, "district"), 3);
    raster.reset();
}

TEST_F(BasicOperationsTest, SetupMultipleBoundariesSafely) {
    EXPECT_NO_THROW(manager.register_level("district"));

    std::vector<std::vector<int>> district_values = {
        {1, 1, 2},
        {1, 2, 2},
        {3, 3, 3}
    };
    create_custom_raster("test_district.asc", district_values);

    SpatialData::get_instance().load("test_district.asc",SpatialData::Districts);
    SpatialData::get_instance().using_raster = true;
    // Check to make sure our data is OK
    std::string errors;
    if (SpatialData::get_instance().check_catalog(errors)) {
      throw std::runtime_error(errors);
    }
    auto district_raster = SpatialData::get_instance().get_raster(SpatialData::Districts);
    SpatialData::get_instance().generate_locations(district_raster);
    SpatialData::get_instance().parse_complete();
    auto raster = std::make_unique<AscFile>(*district_raster);
    EXPECT_NE(district_raster, nullptr);
    EXPECT_NO_THROW(manager.setup_boundary("district", std::move(raster)));
    EXPECT_EQ(manager.get_unit_count("district"), 3);

    EXPECT_NO_THROW(manager.register_level("province"));
    std::vector<std::vector<int>> province_values = {
      {1, 1, 1},
      {1, 1, 1},
      {2, 2, 2}
    };
    create_custom_raster("test_province.asc", province_values);
    SpatialData::get_instance().load("test_province.asc",SpatialData::Districts);
    SpatialData::get_instance().using_raster = true;
    if (SpatialData::get_instance().check_catalog(errors)) {
      throw std::runtime_error(errors);
    }
    auto province_raster = SpatialData::get_instance().get_raster(SpatialData::Districts);
    SpatialData::get_instance().generate_locations(province_raster);
    SpatialData::get_instance().parse_complete();
    EXPECT_NE(province_raster, nullptr);
    raster = std::make_unique<AscFile>(*province_raster);
    EXPECT_NO_THROW(manager.setup_boundary("province", std::move(raster)));
    EXPECT_EQ(manager.get_unit_count("province"), 2);
    raster.reset();
}