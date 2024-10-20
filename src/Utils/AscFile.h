#ifndef ASCFILE_H
#define ASCFILE_H

#include <string>
#include <vector>

class AscFile {
public:
  // Enum for ASC file types
  enum class Type {
    Beta,
    Population,
    District,
    Treatment,
    Mosquito_IFR,
    Mosquito_Size
  };

  // Constructor
  AscFile();

  // Public interface to load and validate the file
  [[nodiscard]] bool load_and_validate(const std::string& file_path, Type file_type);

  // Getter functions
  [[nodiscard]] int get_ncols() const;
  [[nodiscard]] int get_nrows() const;
  [[nodiscard]] double get_xllcorner() const;
  [[nodiscard]] double get_yllcorner() const;
  [[nodiscard]] int get_cellsize() const;
  [[nodiscard]] int get_nodata_value() const;
  [[nodiscard]] const std::vector<std::vector<double>>& get_data() const;

private:
  // Member variables for header info and type
  std::string file_path_;
  Type file_type_;
  int ncols_, nrows_;
  double xllcorner_, yllcorner_;
  int cellsize_;
  int nodata_value_;
  std::vector<std::vector<double>> data_;

  // Private methods
  bool read_header();
  bool validate_header() const;
  bool read_data();
  bool validate_data() const;
  bool validate_value(const double value) const;
};

#endif // ASCFILE_H
