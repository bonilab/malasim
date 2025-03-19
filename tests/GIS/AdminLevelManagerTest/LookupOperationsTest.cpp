#include "TestHelpers.h"
#include <gtest/gtest.h>

#include "Spatial/GIS/AscFile.h"

class LookupOperationsTest : public AdminLevelManagerTestHelper, public ::testing::Test {
protected:
    void SetUp() override {
        AdminLevelManagerTestHelper::SetUp();
        setup_test_boundaries();
    }

    void TearDown() override {
        AdminLevelManagerTestHelper::TearDown();
    }

    void setup_test_boundaries() {
        // Create a simple district raster with known values
        std::vector<std::vector<int>> district_values = {
            {1, 1, 2},
            {1, 1, 2},
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
        EXPECT_NO_THROW(manager.register_level("district"));
        EXPECT_NO_THROW(manager.setup_boundary("district",std::move(raster)));
        raster.reset();
    }
};

TEST_F(LookupOperationsTest, GetAdminUnitForLocation) {
    // Test various locations based on the known raster pattern
    EXPECT_EQ(manager.get_admin_unit(0, "district"), 1);  // First district
    EXPECT_EQ(manager.get_admin_unit(4, "district"), 1);  // Second district
    EXPECT_EQ(manager.get_admin_unit(2, "district"), 2);  // Second district
    EXPECT_EQ(manager.get_admin_unit(8, "district"), 3);  // Third district
}

TEST_F(LookupOperationsTest, InvalidLocationHandling) {
    EXPECT_THROW(manager.get_admin_unit(-1, "district"), std::out_of_range);
    EXPECT_THROW(manager.get_admin_unit(999, "district"), std::out_of_range);
}

TEST_F(LookupOperationsTest, GetLocationsInAdminUnit) {
    auto locations_d1 = manager.get_locations_in_unit(1, "district");
    EXPECT_EQ(locations_d1.size(), 4);  // District 1 has 4 cells

    auto locations_d2 = manager.get_locations_in_unit(2, "district");
    EXPECT_EQ(locations_d2.size(), 2);  // District 2 has 2 cells

    auto locations_d3 = manager.get_locations_in_unit(3, "district");
    EXPECT_EQ(locations_d3.size(), 3);  // District 3 has 3 cells
}

TEST_F(LookupOperationsTest, InvalidAdminUnitHandling) {
    auto locations = manager.get_locations_in_unit(999, "district");
    EXPECT_TRUE(locations.empty());
}

TEST_F(LookupOperationsTest, GetBoundaryData) {
    const auto* boundary = manager.get_boundary("district");
    ASSERT_NE(boundary, nullptr);
    EXPECT_EQ(boundary->count, 3);  // Three unique districts
}

TEST_F(LookupOperationsTest, GetUnitCount) {
    EXPECT_EQ(manager.get_unit_count("district"), 3);
}

TEST_F(LookupOperationsTest, InvalidBoundaryAccess) {
    EXPECT_EQ(manager.get_boundary("nonexistent"), nullptr);
    EXPECT_THROW(manager.get_unit_count("nonexistent"), std::runtime_error);
}