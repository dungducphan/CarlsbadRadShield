#include <TH2.h>
#include <TROOT.h>
#include <TTree.h>
#include <TChain.h>
#include <TSystem.h>
#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TMath.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <tuple>

using voxelData = std::tuple<int, int, int, double, double, int>;

std::vector<voxelData> ReadVoxels(const std::string& filename) {
    std::vector<voxelData> voxels;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        return voxels;
    }

    // Ignore headers
    std::string line;
    file >> line;
    file >> line;
    file >> line;
    file >> line;

    // Read data
    while (file >> line) {
        std::istringstream ss(line);
        std::string str_x, str_y, str_z, str_dose, str_doseSSq, str_nofEntries;
        if (std::getline(ss, str_x, ',') &&
            std::getline(ss, str_y, ',') &&
            std::getline(ss, str_z, ',') &&
            std::getline(ss, str_dose, ',') &&
            std::getline(ss, str_doseSSq, ',')&&
            std::getline(ss, str_nofEntries, ',')) {
            voxels.push_back(
                std::make_tuple(std::stoi(str_x),
                                std::stoi(str_y),
                                std::stoi(str_z),
                                std::stod(str_dose),
                                std::stod(str_doseSSq),
                                std::stoi(str_nofEntries))
            );
        }
    }
    file.close();

    return voxels;
}

#if !defined(__CINT__) && !defined(__CLING__) && !defined(__ACLIC__)

std::string voxelDataPath = "/home/dphan/Documents/GitHub/CarlsbadRadShield/cmake-build-debug/G4Sim/doseDep.txt";

int main() {
    auto data = ReadVoxels(voxelDataPath);

    double maxDose = 0;
    int maxDoseIdx = 0;
    for (int i = 0; i < data.size(); i++) {
        if (std::get<3>(data[i]) > maxDose) {
            maxDose = std::get<3>(data[i]);
            maxDoseIdx = i;
        }
    }
    std::cout << "Max dose: " << maxDose << " Gy." << std::endl;
    std::cout << "Max dose voxel: (" << std::get<0>(data[maxDoseIdx]) << ", " << std::get<1>(data[maxDoseIdx]) << ", " << std::get<2>(data[maxDoseIdx]) << ")." << std::endl;

    return 0;
}
#endif