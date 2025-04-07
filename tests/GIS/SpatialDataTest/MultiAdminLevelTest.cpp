#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Spatial/GIS/SpatialData.h"
#include "TestHelpers.h"
#include "Configuration/Config.h"
#include "Simulation/Model.h"

class AdminBoundaryFixture : public ::testing::Test, protected MultiAdminLevelTest {
protected:
    void SetUp() override {
        MultiAdminLevelTest::SetUp();
    }
    void TearDown() override {
        MultiAdminLevelTest::TearDown();
    }
};

class CustomAdminLevelFixture : public ::testing::Test, protected SpatialDataTestHelper {
protected:
    void SetUp() override {
        add_admin_level("zone");
        add_admin_level("country");
        SpatialDataTestHelper::SetUp();
    }
    void TearDown() override {
        SpatialDataTestHelper::TearDown();
    }
};

class DistrictOnlyFixture : public ::testing::Test, protected SpatialDataTestHelper {
protected:
    void SetUp() override {
        SpatialDataTestHelper::SetUp();
    }
    void TearDown() override {
        SpatialDataTestHelper::TearDown();
    }
};

TEST_F(AdminBoundaryFixture, AdminLevelsAreProperlyInitialized) {
    auto& spatial_data = SpatialData::get_instance();
    auto admin_levels = spatial_data.get_admin_levels();
    ASSERT_EQ(admin_levels.size(), 3);
    EXPECT_TRUE(spatial_data.has_admin_level("district"));
    EXPECT_TRUE(spatial_data.has_admin_level("province"));
    EXPECT_TRUE(spatial_data.has_admin_level("region"));
    EXPECT_FALSE(spatial_data.has_admin_level("nonexistent"));
}

TEST_F(AdminBoundaryFixture, AdminUnitQueriesWorkCorrectly) {
    auto& spatial_data = SpatialData::get_instance();
    EXPECT_EQ(spatial_data.get_admin_units("district"), std::make_pair(1, 2));
    EXPECT_EQ(spatial_data.get_admin_units("province"), std::make_pair(1, 3));
    EXPECT_EQ(spatial_data.get_admin_units("region"), std::make_pair(1, 3));

    EXPECT_EQ(spatial_data.get_unit_count("district"), 2);
    EXPECT_EQ(spatial_data.get_unit_count("province"), 3);
    EXPECT_EQ(spatial_data.get_unit_count("region"), 3);
}

TEST_F(AdminBoundaryFixture, LocationToAdminUnitMappingWorksCorrectly) {
    auto& spatial_data = SpatialData::get_instance();
    ASSERT_EQ(Model::get_config()->number_of_locations(), 8);

    EXPECT_EQ(spatial_data.get_admin_unit("district", 0), 1);
    EXPECT_EQ(spatial_data.get_admin_unit("district", 1), 1);
    EXPECT_EQ(spatial_data.get_admin_unit("district", 2), 2);
    EXPECT_EQ(spatial_data.get_admin_unit("district", 3), 1);
    EXPECT_EQ(spatial_data.get_admin_unit("district", 4), 2);
    EXPECT_EQ(spatial_data.get_admin_unit("district", 5), 2);
    EXPECT_EQ(spatial_data.get_admin_unit("district", 6), 2);
    EXPECT_EQ(spatial_data.get_admin_unit("district", 7), 2);

    EXPECT_EQ(spatial_data.get_admin_unit("province", 0), 1);
    EXPECT_EQ(spatial_data.get_admin_unit("province", 1), 2);
    EXPECT_EQ(spatial_data.get_admin_unit("province", 2), 2);
    EXPECT_EQ(spatial_data.get_admin_unit("province", 3), 1);
    EXPECT_EQ(spatial_data.get_admin_unit("province", 4), 1);
    EXPECT_EQ(spatial_data.get_admin_unit("province", 5), 3);
    EXPECT_EQ(spatial_data.get_admin_unit("province", 6), 3);
    EXPECT_EQ(spatial_data.get_admin_unit("province", 7), 3);
}

TEST_F(AdminBoundaryFixture, AdminUnitToLocationsMappingWorksCorrectly) {
    auto& spatial_data = SpatialData::get_instance();

    EXPECT_THAT(spatial_data.get_locations_in_unit("district", 1),
                ::testing::UnorderedElementsAre(0, 1, 3));
    EXPECT_THAT(spatial_data.get_locations_in_unit("district", 2),
                ::testing::UnorderedElementsAre(2, 4, 5, 6, 7));

    EXPECT_THAT(spatial_data.get_locations_in_unit("province", 1),
                ::testing::UnorderedElementsAre(0, 3, 4));
    EXPECT_THAT(spatial_data.get_locations_in_unit("province", 2),
                ::testing::UnorderedElementsAre(1, 2));
    EXPECT_THAT(spatial_data.get_locations_in_unit("province", 3),
                ::testing::UnorderedElementsAre(5, 6, 7));
}

TEST_F(AdminBoundaryFixture, ErrorHandlingWorksCorrectly) {
    auto& spatial_data = SpatialData::get_instance();

    EXPECT_THROW(
        {
            [[maybe_unused]] auto district = spatial_data.get_admin_unit("nonexistent", 0);
        },
        std::runtime_error);
    EXPECT_THROW(
        {
            [[maybe_unused]] auto locations = spatial_data.get_locations_in_unit("nonexistent", 1);
        },
        std::runtime_error);
    EXPECT_THROW(
        {
            [[maybe_unused]] auto locations = spatial_data.get_locations_in_unit("district", 999);
        },
        std::out_of_range);
    EXPECT_THROW(
        {
            [[maybe_unused]] auto district = spatial_data.get_admin_unit("district", 999);
        }, std::out_of_range);
}

TEST_F(CustomAdminLevelFixture, CustomAdminLevelsWorkCorrectly) {
    auto& spatial_data = SpatialData::get_instance();
    auto admin_levels = spatial_data.get_admin_levels();
    ASSERT_EQ(admin_levels.size(), 3);
    EXPECT_TRUE(spatial_data.has_admin_level("district"));
    EXPECT_TRUE(spatial_data.has_admin_level("zone"));
    EXPECT_TRUE(spatial_data.has_admin_level("country"));
}

TEST_F(AdminBoundaryFixture, BoundaryDataAccessWorksCorrectly) {
    auto& spatial_data = SpatialData::get_instance();

    const auto* district_boundary = spatial_data.get_boundary("district");
    const auto* province_boundary = spatial_data.get_boundary("province");
    const auto* region_boundary = spatial_data.get_boundary("region");

    ASSERT_NE(district_boundary, nullptr);
    ASSERT_NE(province_boundary, nullptr);
    ASSERT_NE(region_boundary, nullptr);

    EXPECT_EQ(district_boundary->min_unit_id, 1);
    EXPECT_EQ(district_boundary->max_unit_id, 2);
    EXPECT_EQ(district_boundary->unit_count, 2);
    EXPECT_EQ(province_boundary->min_unit_id, 1);
    EXPECT_EQ(province_boundary->max_unit_id, 3);
    EXPECT_EQ(province_boundary->unit_count, 3);

    EXPECT_EQ(district_boundary->location_to_unit.size(), 8);
    EXPECT_EQ(province_boundary->location_to_unit.size(), 8);

    EXPECT_EQ(district_boundary->unit_to_locations.size(), 3);
    EXPECT_EQ(province_boundary->unit_to_locations.size(), 4);
}

TEST_F(DistrictOnlyFixture, DistrictOnlyModeWorksCorrectly) {
    auto& spatial_data = SpatialData::get_instance();
    auto admin_levels = spatial_data.get_admin_levels();

    ASSERT_EQ(admin_levels.size(), 1);
    EXPECT_EQ(admin_levels[0], "district");
    EXPECT_TRUE(spatial_data.has_admin_level("district"));
    EXPECT_EQ(spatial_data.get_unit_count("district"), 2);

    EXPECT_EQ(spatial_data.get_admin_unit("district", 0), 1);
    EXPECT_EQ(spatial_data.get_admin_unit("district", 2), 2);

    EXPECT_THAT(spatial_data.get_locations_in_unit("district", 2),
                ::testing::UnorderedElementsAre(2, 4, 5, 6, 7));
}
