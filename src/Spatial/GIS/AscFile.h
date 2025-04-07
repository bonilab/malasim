/*
 * AscFile.h
 *
 * Define the means of reading and writing ESRI ASCII files. Functionality has
 * been verified against ESRI ArcMap 10.7.1
 *
 * http://webhelp.esri.com/arcgisdesktop/9.3/index.cfm?TopicName=ESRI%20ASCII%20Raster%20format
 */
#ifndef ASCFILE_H
#define ASCFILE_H

#include <memory>
#include <string>
#include <vector>
// The ASC file either as read, or to be written. Note that since the
// specification does not provide a header indicating if the data is floating
// point or integer, the data is presumed to be floating point.
struct AscFile {
  // Flag to indicate the value has not been set yet
  static const int NOT_SET = -1;

  // End-of-line for ASC files, ArcMap seems to be platform independent though.
  inline static const std::string CRLF = "\r\n";

  // Number of rows
  int nrows = NOT_SET;

  // Number of columns
  int ncols = NOT_SET;

  // X-center coordinate
  double xllcenter = NOT_SET;

  // Y-center coordinate
  double yllcenter = NOT_SET;

  // X-lower left corner coordinate
  double xllcorner = NOT_SET;

  // X-lower left corner coordinate
  double yllcorner = NOT_SET;

  // Size of a cell, in units of projection
  double cellsize = NOT_SET;

  // Representation of a cell with no data, default value is zero at time of
  // initialization
  double nodata_value = 0;

  // The data stored in the file
  std::vector<std::vector<float>> data;
};

class AscFileManager {
private:
  static const int HEADER_WIDTH = 14;

  // Static class, no need to instantiate.
  AscFileManager() = default;

public:
  // Returns an empty string if the file is valid, otherwise returns a string
  // describing the errors.
  static std::string check_asc_file(const AscFile* file);
  static std::unique_ptr<AscFile> read(const std::string &file_name);
  static void write(AscFile* file, const std::string &file_name);
};

#endif
