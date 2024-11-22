#include <CLI/CLI.hpp>
#include <string>

namespace utils{
class Cli {
public:
    // Static method to get the single instance of the class
    static Cli& get_instance() {
        static Cli instance;  // Guaranteed to be destroyed and instantiated on first use.
        return instance;
    }

    // Delete copy constructor and assignment operator
    Cli(const Cli&) = delete;
    Cli& operator=(const Cli&) = delete;

    // Parse function
    void parse(int argc, char** argv) {
        try {
            app.parse(argc, argv);
        } catch (const CLI::ParseError &e) {
            std::exit(app.exit(e));
        }
    }

    // Accessors for parameters
    std::string get_input_file() const { return input_file; }
    int get_cluster_job_number() const { return cluster_job_number; }
    std::string get_reporter() const { return reporter; }
    std::string get_output_path() const { return output_path; }
    int get_verbosity() const { return verbosity; }
    bool is_help_requested() const { return help; }

private:
    // Private constructor for Singleton
    Cli() {
        app.description("Individual-based simulation for malaria.");

        // Command options with required() to make them compulsory
        app.add_option("-i,--input,--config", input_file, "The config file (YAML format) (Required). \nEx: MaSim -i input.yml")->required();
        app.add_option("-j,--job", cluster_job_number, "Cluster job number (Optional). \nEx: MaSim -j 1");
        app.add_option("-r,--reporter", reporter, "Reporter Type (Optional). \nEx: MaSim -r MonthlyReporter");
        app.add_option("-o,--output", output_path, "Path for output files, default is current directory (Optional). \nEx: MaSim -o out");

        // Global options
        app.add_option("--v", verbosity, "Sets the current verbosity of the logging, default zero");
    }

    CLI::App app{"Individual-based simulation for malaria"};

    // Parameters
    std::string input_file;
    int cluster_job_number;
    std::string reporter;
    std::string output_path;
    int verbosity = 0;
    bool help = false;
};
}  // namespace utils

