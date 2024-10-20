#include <gtest/gtest.h>
#include <yaml-cpp/yaml.h>
#include "Configuration/GenotypeParameters.h"

class GenotypeParametersTest : public ::testing::Test {
protected:
    GenotypeParameters genotype_parameters;

    void SetUp() override {
        // Set up multiplicative effect on EC50
        GenotypeParameters::MultiplicativeEffectOnEC50 multiplicative_effect;
        multiplicative_effect.set_drug_id(4);
        multiplicative_effect.set_factors({1.0, 2.44444444});

        // Set up amino acid positions
        GenotypeParameters::AminoAcidPosition aa_position;
        aa_position.set_position(86);
        aa_position.set_amino_acids({"N", "Y"});
        aa_position.set_daily_crs({0.0, 0.0005});
        aa_position.set_multiplicative_effect_on_EC50({multiplicative_effect});

        // Set up gene info
        GenotypeParameters::GeneInfo gene_info;
        gene_info.set_name("Pfmdr1");
        gene_info.set_max_copies(2);
        gene_info.set_cnv_daily_crs({0.0, 0.0005});
        gene_info.set_cnv_multiplicative_effect_on_EC50({multiplicative_effect});
        gene_info.set_aa_positions({aa_position});

        // Set up chromosome info
        GenotypeParameters::ChromosomeInfo chromosome_info;
        chromosome_info.set_chromosome(5);
        chromosome_info.set_genes({gene_info});

        // Set up override EC50 patterns
        GenotypeParameters::OverrideEC50Pattern override_ec50;
        override_ec50.set_pattern("||||NY1||1111111,0||||||000000000010|1");
        override_ec50.set_drug_id(1);
        override_ec50.set_ec50(0.8);

        // Set up initial parasite info
        GenotypeParameters::GenotypeInfo genotype_info;
        genotype_info.set_aa_sequence("||||YY1||TTHFIMG,x||||||FNCMYRIPRPCA|1");
        genotype_info.set_prevalence(0.05);

        GenotypeParameters::InitialGenotypeInfo initial_genotype_info;
        initial_genotype_info.set_location_id(-1);
        initial_genotype_info.set_genotype_info({genotype_info});

        // Set up genotype parameters
        genotype_parameters.set_mutation_mask("||||111||1111111,0||||||000000000010|1");
        genotype_parameters.set_mutation_probability_per_locus(0.001);
        genotype_parameters.set_pf_genotype_info({chromosome_info});
        genotype_parameters.set_override_ec50_patterns({override_ec50});
        genotype_parameters.set_initial_genotype_info({initial_genotype_info});
    }
};

// Test encoding functionality for GenotypeParameters
TEST_F(GenotypeParametersTest, EncodeGenotypeParameters) {
    YAML::Node node = YAML::convert<GenotypeParameters>::encode(genotype_parameters);

    // Validate encoding of genotype parameters
    EXPECT_EQ(node["mutation_mask"].as<std::string>(), "||||111||1111111,0||||||000000000010|1");
    EXPECT_EQ(node["mutation_probability_per_locus"].as<double>(), 0.001);
    EXPECT_EQ(node["pf_genotype_info"][0]["chromosome"].as<int>(), 5);
    EXPECT_EQ(node["pf_genotype_info"][0]["genes"][0]["name"].as<std::string>(), "Pfmdr1");
    EXPECT_EQ(node["pf_genotype_info"][0]["genes"][0]["aa_positions"][0]["position"].as<int>(), 86);
    EXPECT_EQ(node["override_ec50_patterns"][0]["pattern"].as<std::string>(), "||||NY1||1111111,0||||||000000000010|1");
    EXPECT_EQ(node["initial_genotype_info"][0]["location_id"].as<int>(), -1);
}

// Test decoding functionality for GenotypeParameters
TEST_F(GenotypeParametersTest, DecodeGenotypeParameters) {
    YAML::Node node;
    node["mutation_mask"] = "||||111||1111111,0||||||000000000010|1";
    node["mutation_probability_per_locus"] = 0.001;

    node["pf_genotype_info"][0]["chromosome"] = 5;
    node["pf_genotype_info"][0]["genes"][0]["name"] = "Pfmdr1";
    node["pf_genotype_info"][0]["genes"][0]["multiplicative_effect_on_EC50_for_2_or_more_mutations"][0]["drug_id"] = 1;
    node["pf_genotype_info"][0]["genes"][0]["multiplicative_effect_on_EC50_for_2_or_more_mutations"][0]["factor"] = 1.05;
    node["pf_genotype_info"][0]["genes"][0]["max_copies"] = 2;
    node["pf_genotype_info"][0]["genes"][0]["cnv_daily_crs"] = std::vector<double>{0.0, 0.0005};
    node["pf_genotype_info"][0]["genes"][0]["cnv_multiplicative_effect_on_EC50"][0]["drug_id"] = 4;
    node["pf_genotype_info"][0]["genes"][0]["cnv_multiplicative_effect_on_EC50"][0]["factors"] = std::vector<double>{1.0, 2.44444444};
    node["pf_genotype_info"][0]["genes"][0]["cnv_multiplicative_effect_on_EC50"][1]["drug_id"] = 1;
    node["pf_genotype_info"][0]["genes"][0]["cnv_multiplicative_effect_on_EC50"][1]["factors"] = std::vector<double>{1.0, 1.3};
    node["pf_genotype_info"][0]["genes"][0]["aa_positions"][0]["position"] = 86;
    node["pf_genotype_info"][0]["genes"][0]["aa_positions"][0]["amino_acids"] = std::vector<std::string>{"N", "Y"};
    node["pf_genotype_info"][0]["genes"][0]["aa_positions"][0]["daily_crs"] = std::vector<double>{0.0, 0.0005};
    node["pf_genotype_info"][0]["genes"][0]["aa_positions"][0]["multiplicative_effect_on_EC50"][0]["drug_id"] = 6;
    node["pf_genotype_info"][0]["genes"][0]["aa_positions"][0]["multiplicative_effect_on_EC50"][0]["factors"] = std::vector<double>{1.0, 1.25};
    node["pf_genotype_info"][0]["genes"][0]["aa_positions"][0]["multiplicative_effect_on_EC50"][1]["drug_id"] = 1;
    node["pf_genotype_info"][0]["genes"][0]["aa_positions"][0]["multiplicative_effect_on_EC50"][1]["factors"] = std::vector<double>{1.25, 1.0};

    node["pf_genotype_info"][1]["chromosome"] = 7;
    node["pf_genotype_info"][1]["genes"][0]["name"] = "Pfcrt";
    node["pf_genotype_info"][1]["genes"][0]["multiplicative_effect_on_EC50_for_2_or_more_mutations"][0]["drug_id"] = 3;
    node["pf_genotype_info"][1]["genes"][0]["multiplicative_effect_on_EC50_for_2_or_more_mutations"][0]["factor"] = 1.0;
    node["pf_genotype_info"][1]["genes"][0]["average_daily_crs"] = 0.1290;
    node["pf_genotype_info"][1]["genes"][0]["aa_positions"][0]["position"] = 76;
    node["pf_genotype_info"][1]["genes"][0]["aa_positions"][0]["amino_acids"] = std::vector<std::string>{"K", "T"};
    node["pf_genotype_info"][1]["genes"][0]["aa_positions"][0]["daily_crs"] = std::vector<double>{0.0, 0.003875969};
    node["pf_genotype_info"][1]["genes"][0]["aa_positions"][0]["multiplicative_effect_on_EC50"][0]["drug_id"] = 6;
    node["pf_genotype_info"][1]["genes"][0]["aa_positions"][0]["multiplicative_effect_on_EC50"][0]["factors"] = std::vector<double>{1.0, 1.6};
    node["pf_genotype_info"][1]["genes"][0]["aa_positions"][0]["multiplicative_effect_on_EC50"][1]["drug_id"] = 1;
    node["pf_genotype_info"][1]["genes"][0]["aa_positions"][0]["multiplicative_effect_on_EC50"][1]["factors"] = std::vector<double>{1.1, 1.0};
    node["pf_genotype_info"][1]["genes"][0]["aa_positions"][0]["multiplicative_effect_on_EC50"][2]["drug_id"] = 2;
    node["pf_genotype_info"][1]["genes"][0]["aa_positions"][0]["multiplicative_effect_on_EC50"][2]["factors"] = std::vector<double>{1.0, 1.2};

    // Chromosome 13 data (Pfkelch13 gene)
    node["pf_genotype_info"][2]["chromosome"] = 13;
    node["pf_genotype_info"][2]["genes"][0]["name"] = "Pfk13";
    node["pf_genotype_info"][2]["genes"][0]["multiplicative_effect_on_EC50_for_2_or_more_mutations"][0]["drug_id"] = 0;
    node["pf_genotype_info"][2]["genes"][0]["multiplicative_effect_on_EC50_for_2_or_more_mutations"][0]["factor"] = 1.1;
    node["pf_genotype_info"][2]["genes"][0]["aa_positions"][0]["position"] = 446;
    node["pf_genotype_info"][2]["genes"][0]["aa_positions"][0]["amino_acids"] = std::vector<std::string>{"F", "I"};
    node["pf_genotype_info"][2]["genes"][0]["aa_positions"][0]["daily_crs"] = std::vector<double>{0.0, 0.0005};
    node["pf_genotype_info"][2]["genes"][0]["aa_positions"][0]["multiplicative_effect_on_EC50"][0]["drug_id"] = 0;
    node["pf_genotype_info"][2]["genes"][0]["aa_positions"][0]["multiplicative_effect_on_EC50"][0]["factors"] = std::vector<double>{1.0, 1.6648};

    // Chromosome 14 data (Pfplasmepsin gene)
    node["pf_genotype_info"][3]["chromosome"] = 14;
    node["pf_genotype_info"][3]["genes"][0]["name"] = "Pfpm23";
    node["pf_genotype_info"][3]["genes"][0]["max_copies"] = 2;
    node["pf_genotype_info"][3]["genes"][0]["cnv_daily_crs"] = std::vector<double>{0.0, 0.0005};
    node["pf_genotype_info"][3]["genes"][0]["cnv_multiplicative_effect_on_EC50"][0]["drug_id"] = 3;
    node["pf_genotype_info"][3]["genes"][0]["cnv_multiplicative_effect_on_EC50"][0]["factors"] = std::vector<double>{1.0, 1.37};
    node["pf_genotype_info"][3]["genes"][0]["aa_positions"] = std::vector<YAML::Node>{};

    // Override EC50 patterns
    node["override_ec50_patterns"][0]["pattern"] = "||||NY1||1111111,0||||||000000000010|1";
    node["override_ec50_patterns"][0]["drug_id"] = 1;
    node["override_ec50_patterns"][0]["ec50"] = 0.8;

    // Initial parasite info
    node["initial_genotype_info"][0]["location_id"] = -1;
    node["initial_genotype_info"][0]["genotype_info"][0]["aa_sequence"] = "||||YY1||TTHFIMG,x||||||FNCMYRIPRPCA|1";
    node["initial_genotype_info"][0]["genotype_info"][0]["prevalence"] = 0.05;

    GenotypeParameters decoded_parameters;
    EXPECT_NO_THROW(YAML::convert<GenotypeParameters>::decode(node, decoded_parameters));

    // Validate decoding
    EXPECT_EQ(decoded_parameters.get_mutation_mask(), "||||111||1111111,0||||||000000000010|1");
    EXPECT_EQ(decoded_parameters.get_mutation_probability_per_locus(), 0.001);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_chromosome(), 5);
    EXPECT_EQ(decoded_parameters.get_override_ec50_patterns()[0].get_pattern(), "||||NY1||1111111,0||||||000000000010|1");
    EXPECT_EQ(decoded_parameters.get_initial_genotype_info()[0].get_location_id(), -1);

    // Validate Chromosome 5
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_chromosome(), 5);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_name(), "Pfmdr1");
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_max_copies(), 2);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_cnv_daily_crs()[0], 0.0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_cnv_daily_crs()[1], 0.0005);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_cnv_multiplicative_effect_on_EC50()[0].get_drug_id(), 4);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_cnv_multiplicative_effect_on_EC50()[0].get_factors()[0], 1.0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_cnv_multiplicative_effect_on_EC50()[0].get_factors()[1], 2.44444444);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_cnv_multiplicative_effect_on_EC50()[1].get_drug_id(), 1);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_cnv_multiplicative_effect_on_EC50()[1].get_factors()[0], 1.0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_cnv_multiplicative_effect_on_EC50()[1].get_factors()[1], 1.3);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_aa_positions()[0].get_position(), 86);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_aa_positions()[0].get_amino_acids()[0], "N");
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_aa_positions()[0].get_amino_acids()[1], "Y");
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[0].get_drug_id(), 6);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[0].get_factors()[0], 1.0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[0].get_factors()[1], 1.25);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[1].get_drug_id(), 1);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[1].get_factors()[0], 1.25);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[0].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[1].get_factors()[1], 1.0);

    //Validate chromosome 7
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_chromosome(), 7);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_name(), "Pfcrt");
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_average_daily_crs(), 0.1290);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_position(), 76);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_amino_acids()[0], "K");
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_amino_acids()[1], "T");
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_daily_crs()[0], 0.0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_daily_crs()[1], 0.003875969);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[0].get_drug_id(), 6);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[0].get_factors()[0], 1.0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[0].get_factors()[1], 1.6);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[1].get_drug_id(), 1);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[1].get_factors()[0], 1.1);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[1].get_factors()[1], 1.0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[2].get_drug_id(), 2);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[2].get_factors()[0], 1.0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[1].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[2].get_factors()[1], 1.2);

    //Validate chromosome 13
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[2].get_chromosome(), 13);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[2].get_genes()[0].get_name(), "Pfk13");
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[2].get_genes()[0].get_aa_positions()[0].get_position(), 446);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[2].get_genes()[0].get_aa_positions()[0].get_amino_acids()[0], "F");
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[2].get_genes()[0].get_aa_positions()[0].get_amino_acids()[1], "I");
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[2].get_genes()[0].get_aa_positions()[0].get_daily_crs()[0], 0.0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[2].get_genes()[0].get_aa_positions()[0].get_daily_crs()[1], 0.0005);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[2].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[0].get_drug_id(), 0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[2].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[0].get_factors()[0], 1.0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[2].get_genes()[0].get_aa_positions()[0].get_multiplicative_effect_on_EC50()[0].get_factors()[1], 1.6648);

    //Validate chromosome 14
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[3].get_chromosome(), 14);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[3].get_genes()[0].get_name(), "Pfpm23");
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[3].get_genes()[0].get_max_copies(), 2);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[3].get_genes()[0].get_cnv_daily_crs()[0], 0.0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[3].get_genes()[0].get_cnv_daily_crs()[1], 0.0005);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[3].get_genes()[0].get_cnv_multiplicative_effect_on_EC50()[0].get_drug_id(), 3);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[3].get_genes()[0].get_cnv_multiplicative_effect_on_EC50()[0].get_factors()[0], 1.0);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[3].get_genes()[0].get_cnv_multiplicative_effect_on_EC50()[0].get_factors()[1], 1.37);
    EXPECT_EQ(decoded_parameters.get_pf_genotype_info()[3].get_genes()[0].get_aa_positions().size(), 0);
}

// Test for decoding with missing fields
TEST_F(GenotypeParametersTest, DecodeGenotypeParametersMissingField) {
    // Test case: Missing mutation_probability_per_locus
    YAML::Node node1;
    node1["mutation_mask"] = "||||111||1111111,0||||||000000000010|1";  // Missing mutation_probability_per_locus
    node1["pf_genotype_info"] = YAML::Node();
    node1["override_ec50_patterns"] = YAML::Node();
    node1["initial_genotype_info"] = YAML::Node();

    GenotypeParameters decoded_parameters1;
    EXPECT_THROW(YAML::convert<GenotypeParameters>::decode(node1, decoded_parameters1), std::runtime_error);

    // Test case: Missing pf_genotype_info
    YAML::Node node2;
    node2["mutation_mask"] = "||||111||1111111,0||||||000000000010|1";
    node2["mutation_probability_per_locus"] = 0.001;  // Missing pf_genotype_info
    node2["override_ec50_patterns"] = YAML::Node();
    node2["initial_genotype_info"] = YAML::Node();

    GenotypeParameters decoded_parameters2;
    EXPECT_THROW(YAML::convert<GenotypeParameters>::decode(node2, decoded_parameters2), std::runtime_error);

    // Test case: Missing override_ec50_patterns
    YAML::Node node3;
    node3["mutation_mask"] = "||||111||1111111,0||||||000000000010|1";
    node3["mutation_probability_per_locus"] = 0.001;
    node3["pf_genotype_info"] = YAML::Node();  // Missing override_ec50_patterns
    node3["initial_genotype_info"] = YAML::Node();

    GenotypeParameters decoded_parameters3;
    EXPECT_THROW(YAML::convert<GenotypeParameters>::decode(node3, decoded_parameters3), std::runtime_error);

    // Test case: Missing initial_genotype_info
    YAML::Node node4;
    node4["mutation_mask"] = "||||111||1111111,0||||||000000000010|1";
    node4["mutation_probability_per_locus"] = 0.001;
    node4["pf_genotype_info"] = YAML::Node();
    node4["override_ec50_patterns"] = YAML::Node();  // Missing initial_genotype_info

    GenotypeParameters decoded_parameters4;
    EXPECT_THROW(YAML::convert<GenotypeParameters>::decode(node4, decoded_parameters4), std::runtime_error);

    // Test case: Completely empty pf_genotype_info
    YAML::Node node5;
    node5["mutation_mask"] = "||||111||1111111,0||||||000000000010|1";
    node5["mutation_probability_per_locus"] = 0.001;
    node5["pf_genotype_info"] = YAML::Node();  // Empty pf_genotype_info
    node5["override_ec50_patterns"] = YAML::Node();
    node5["initial_genotype_info"] = YAML::Node();

    GenotypeParameters decoded_parameters5;
    EXPECT_THROW(YAML::convert<GenotypeParameters>::decode(node5, decoded_parameters5), std::runtime_error);

    // Test case: Missing fields inside pf_genotype_info
    YAML::Node node6;
    node6["mutation_mask"] = "||||111||1111111,0||||||000000000010|1";
    node6["mutation_probability_per_locus"] = 0.001;
    node6["pf_genotype_info"][0]["chromosome"] = 5;  // Missing genes in pf_genotype_info
    node6["override_ec50_patterns"] = YAML::Node();
    node6["initial_genotype_info"] = YAML::Node();

    GenotypeParameters decoded_parameters6;
    EXPECT_THROW(YAML::convert<GenotypeParameters>::decode(node6, decoded_parameters6), std::runtime_error);
}

