#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <atomic>

class G4Material;

class CrustMaterialFactory {

public:
  G4Material *GetMaterial(double density, const std::string &name = "C1");
  static CrustMaterialFactory *Instance();

private:
  CrustMaterialFactory();
  ~CrustMaterialFactory();
  CrustMaterialFactory(const CrustMaterialFactory &) = delete;

  std::unordered_map<std::string, size_t> name_to_id_;
  std::vector<std::string> elem_names_;  // [element_id]
  std::vector<std::vector<double>> elem_abundances_;  // [name_id][element_id]

  static std::atomic<CrustMaterialFactory *> instance_;

  void LoadData();

};
