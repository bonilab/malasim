#include "AscFile.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>

// Constructor
AscFile::AscFile() {
    ncols_ = 0;
    nrows_ = 0;
    xllcorner_ = 0.0;
    yllcorner_ = 0.0;
    cellsize_ = 0.0;
    nodata_value_ = -9999;
}

// Public interface to load and validate the file
bool AscFile::load_and_validate(const std::string& file_path, Type file_type) {
    file_path_ = file_path;
    file_type_ = file_type;
    return read_header() && validate_header() && read_data() && validate_data();
}

// Getter functions
int AscFile::get_ncols() const { return ncols_; }
int AscFile::get_nrows() const { return nrows_; }
double AscFile::get_xllcorner() const { return xllcorner_; }
double AscFile::get_yllcorner() const { return yllcorner_; }
int AscFile::get_cellsize() const { return cellsize_; }
int AscFile::get_nodata_value() const { return nodata_value_; }
const std::vector<std::vector<double>>& AscFile::get_data() const { return data_; }

// Private methods
bool AscFile::read_header() {
    std::ifstream file(file_path_);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << file_path_ << std::endl;
        return false;
    }

    std::string line;
    for (int i = 0; i < 6; ++i) {
        if (!std::getline(file, line)) {
            std::cerr << "Error: Unexpected end of file while reading header." << std::endl;
            return false;
        }
        std::istringstream iss(line);
        std::string key;
        if (!(iss >> key)) {
            std::cerr << "Error: Invalid header format." << std::endl;
            return false;
        }

        if (key == "ncols") {
            if (!(iss >> ncols_) || ncols_ <= 0) {
                std::cerr << "Error: Invalid ncols value." << std::endl;
                return false;
            }
        } else if (key == "nrows") {
            if (!(iss >> nrows_) || nrows_ <= 0) {
                std::cerr << "Error: Invalid nrows value." << std::endl;
                return false;
            }
        } else if (key == "xllcorner" || key == "xllcenter") {
            if (!(iss >> xllcorner_)) {
                std::cerr << "Error: Invalid xllcorner/xllcenter value." << std::endl;
                return false;
            }
        } else if (key == "yllcorner" || key == "yllcenter") {
            if (!(iss >> yllcorner_)) {
                std::cerr << "Error: Invalid yllcorner/yllcenter value." << std::endl;
                return false;
            }
        } else if (key == "cellsize") {
            if (!(iss >> cellsize_) || cellsize_ <= 0) {
                std::cerr << "Error: Invalid cellsize value." << std::endl;
                return false;
            }
        } else if (key == "NODATA_value") {
            if (!(iss >> nodata_value_)) {
                std::cerr << "Error: Invalid NODATA_value." << std::endl;
                return false;
            }
        } else {
            std::cerr << "Error: Unrecognized header key: " << key << std::endl;
            return false;
        }
    }

    return true;
}

bool AscFile::validate_header() const {
    if (ncols_ <= 0) {
        std::cerr << "Validation Error: ncols must be greater than 0." << std::endl;
        return false;
    }
    if (nrows_ <= 0) {
        std::cerr << "Validation Error: nrows must be greater than 0." << std::endl;
        return false;
    }
    if (cellsize_ <= 0) {
        std::cerr << "Validation Error: cellsize must be greater than 0." << std::endl;
        return false;
    }
    return true;
}

bool AscFile::read_data() {
    std::ifstream file(file_path_);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << file_path_ << std::endl;
        return false;
    }

    // Skip the header (6 lines)
    std::string line;
    for (int i = 0; i < 6; ++i) {
        std::getline(file, line);
    }

    // Read the grid values
    data_.resize(nrows_, std::vector<double>(ncols_));
    int row_count = 0;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        double value;
        int col_count = 0;
        while (iss >> value) {
            if (col_count < ncols_) {
                data_[row_count][col_count] = value;
            }
            ++col_count;
        }
        if (col_count != ncols_) {
            std::cerr << "Error: Mismatch in the number of columns in row " << row_count << std::endl;
            return false;
        }
        ++row_count;
    }

    if (row_count != nrows_) {
        std::cerr << "Error: Mismatch in the number of rows. Expected: " << nrows_ << ", got: " << row_count << std::endl;
        return false;
    }

    return true;
}

bool AscFile::validate_data() const {
    std::cout << "Validating data file " << file_path_ << " " << static_cast<int>(file_type_) << "\n";
    for (int i = 0; i < nrows_; ++i) {
        for (int j = 0; j < ncols_; ++j) {
            const double value = data_[i][j];
            if (value != nodata_value_ && !validate_value(value)) {
                std::cerr << "Validation Error: Invalid data value in the grid at row " << i << ", column " << j  << " value " << value << std::endl;
                return false;
            }
        }
    }
    return true;
}

// Validate value based on the file type
bool AscFile::validate_value(const double value) const {
    switch (file_type_) {
        case Type::Beta:
            return value >= 0.0 && value <= 20.0;
        case Type::Population:
            return value >= 0.0;
        case Type::District:
            return value >= 0.0;
        case Type::Treatment:
            return value >= 0.0 && value <= 1.0;
        case Type::Mosquito_IFR:
            return value >= 0.0 && value <= 1.0;
        case Type::Mosquito_Size:
            return value >= 0.0;
        default:
            return false;
    }
}
