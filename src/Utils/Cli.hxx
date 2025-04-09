#include <spdlog/spdlog.h>

#include <CLI/CLI.hpp>
#include <string>

namespace utils {
class Cli {
  struct MaSimAppInput {
    std::string input_path{"input.yml"};
    std::string output_path;
    std::string reporter;
    int verbosity{0};
    int job_number{0};
    int replicate{1};
    std::string list_reporters{"lr"};
    std::string help{"h"};
    bool dump_movement_matrix{false};
    bool record_individual_movement{false};
    bool record_cell_movement{false};
    bool record_district_movement{false};
    bool record_movement{false};
  };

public:
  // Static method to get the single instance of the class
  static Cli &get_instance() {
    static Cli instance;  // Guaranteed to be destroyed and instantiated on first use.
    return instance;
  }

  Cli(Cli &&) = delete;
  Cli &operator=(Cli &&) = delete;
  // Delete copy constructor and assignment operator
  Cli(const Cli &) = delete;
  void operator=(const Cli &) = delete;

  explicit Cli(MaSimAppInput cli_input) : cli_input_(std::move(cli_input)) {}

  // Parse function
  void parse(int argc, char** argv) {
    create_cli_options(app_, cli_input_);
    spdlog::info("Parsing command line arguments");
    try {
      app_.parse(argc, argv);
      validate_config(cli_input_);
    } catch (const CLI::ParseError &e) { std::exit(app_.exit(e)); }
  }

  // Accessors for parameters
  [[nodiscard]] std::string get_input_path() const { return cli_input_.input_path; }
  void set_input_path(const std::string &input_path) { cli_input_.input_path = input_path; }
  [[nodiscard]] int get_job_number() const { return cli_input_.job_number; }
  [[nodiscard]] int get_replicate() const { return cli_input_.replicate; }
  [[nodiscard]] std::string get_reporter() const { return cli_input_.reporter; }
  [[nodiscard]] std::string get_output_path() const { return cli_input_.output_path; }
  void set_output_path(const std::string &output_path) { cli_input_.output_path = output_path; }
  [[nodiscard]] int get_verbosity() const { return cli_input_.verbosity; }
  [[nodiscard]] bool get_dump_movement_matrix() const { return cli_input_.dump_movement_matrix; }
  [[nodiscard]] bool get_record_individual_movement() const {
    return cli_input_.record_individual_movement;
  }
  [[nodiscard]] bool get_record_cell_movement() const { return cli_input_.record_cell_movement; }
  [[nodiscard]] bool get_record_district_movement() const {
    return cli_input_.record_district_movement;
  }
  [[nodiscard]] bool get_record_movement() const { return cli_input_.record_movement; }

  static void create_cli_options(CLI::App &app, MaSimAppInput &input) {
    app.add_option("-i,--input", input.input_path, "Input filename. Default: `input.yml`.");

    app.add_option("-o,--output", input.output_path, "Output path. Default: `./`.");

    app.add_option("-r,--reporter", input.reporter, "Reporter type. Default: `MonthlyReporter`.");

    app.add_option("-v,--verbosity", input.verbosity,
                   "Sets the verbosity of the logging. Default: 0");

    app.add_option(
        "-j,--job", input.job_number,
        "Sets the study to associate with the configuration (or database id). Default: 0");

    app.add_option("-d,--dump", input.dump_movement_matrix,
                   "Dump the movement matrix as calculated.");

    app.add_option("-l,--list", input.list_reporters, "List the possible reporters.");

    app.add_option("--im", input.record_individual_movement, "Record individual movement data.");

    app.add_option("--mc", input.record_cell_movement, "Record the movement between cells.");

    app.add_option("--md", input.record_district_movement,
                   "Record the movement between districts.");

    app.add_option("--replicate", input.replicate, "Replicate number. Default: 1");
  }

  static bool validate_config(MaSimAppInput &input) {
    std::ifstream f_input(input.input_path.c_str());
    if (!f_input.good()) {
      spdlog::error("Err: Input file error or not found!");
      return false;
    }

    if (input.record_cell_movement && input.record_district_movement) {
      spdlog::error("--mc and --md are mutual exclusive and may not be run together.");
      return false;
    }

    input.record_movement = input.record_individual_movement || input.record_cell_movement
                            || input.record_district_movement;

    if (input.record_movement) { spdlog::info("Movement data will be recorded."); }

    switch (input.verbosity) {
      case 0: {
        spdlog::set_level(spdlog::level::info);
        spdlog::info("Verbosity level set to 0. Only info will be logged.");
        break;
      }
      case 1: {
        spdlog::set_level(spdlog::level::debug);
        spdlog::info("Verbosity level set to 1. Info, debug will be logged.");
        break;
      }
      case 2: {
        spdlog::set_level(spdlog::level::trace);
        spdlog::info("Verbosity level set to 2. Info, debug and trace will be logged.");
        break;
      }
      default: {
        spdlog::set_level(spdlog::level::info);
        spdlog::info("Verbosity level set to 0. Only info will be logged.");
        break;
      }
    }

    return true;
  }

private:
  // Private constructor for Singleton
  Cli() = default;
  ~Cli() = default;
  CLI::App app_{"Individual-based simulation for malaria"};
  MaSimAppInput cli_input_;
};
}  // namespace utils

