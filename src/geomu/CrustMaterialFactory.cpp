#include "geomu/CrustMaterialFactory.h"
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <iostream>
#include <fstream>
#include <sstream>

#ifndef CRUST_MATERIAL_DATAPATH
#define CRUST_MATERIAL_DATAPATH  PROJECT_BASEDIR "/data/crust-elem.tsv"
#endif  /* CRUST_MATERIAL_DATAPATH */

#include "geomu/diagnostic.h"

using namespace std;

CrustMaterialFactory *CrustMaterialFactory::instance_;

CrustMaterialFactory::CrustMaterialFactory()
{
  LoadData();
}

CrustMaterialFactory::~CrustMaterialFactory()
{
  // Empty.
}

G4Material *CrustMaterialFactory::GetMaterial(double density, const std::string &name)
{
  string material_name = "Crust_" + to_string(density) + "_" + name;
  G4Material *material = G4Material::GetMaterial(material_name, false);
  if(material) return material;

  const vector<double> &elem_abundance = elem_abundances_[name_to_id_.at(name)];
  size_t nelem = elem_abundance.size();

  difo << "Building material " << material_name << " with " << nelem << " elements:" << dend;
  material = new G4Material(material_name, density, nelem);
  for(size_t ielem = 0; ielem < nelem; ++ielem) {
    G4Element *element = G4NistManager::Instance()->FindOrBuildElement(ielem + 1);
    if(element->GetName() != elem_names_[ielem]) {
      throw runtime_error("unexpected element name: " + element->GetName() + " != " + elem_names_[ielem]);
    }
    material->AddElement(element, elem_abundance[ielem]);
    difo << "  - " << elem_names_[ielem] << " " << elem_abundance[ielem] << dend;
  }
  return material;
}

void CrustMaterialFactory::LoadData()
{
  ifstream datafile(CRUST_MATERIAL_DATAPATH);
  if(!datafile) throw runtime_error("error opening datafile: "s + CRUST_MATERIAL_DATAPATH);
  string line, s, s2;

  // Fetch material names.
  {
    getline(datafile, line);
    istringstream iss(line);
    if(!(iss >> s) || s != "Element") {
      throw runtime_error("error reading datafile: "s + CRUST_MATERIAL_DATAPATH);
    }
    while(iss >> s) {
      name_to_id_[s] = name_to_id_.size();
      elem_abundances_.emplace_back();
    }
  }

  // Fetch element names and abundances.
  while(getline(datafile, line)) {
    istringstream iss(line);
    if(!(iss >> s2 /* serial */ >> s && getline(iss, s2, '\t'))) {
      throw runtime_error("error reading datafile: "s + CRUST_MATERIAL_DATAPATH);
    }
    elem_names_.push_back(s);
    for(vector<double> &elem_abundance : elem_abundances_) {
      double a = 0.0;
      if(getline(iss, s, '\t') && !s.empty()) a = stod(s);
      elem_abundance.push_back(a);
    }
  }

  // Tighten up.
  for(vector<double> &elem_abundance : elem_abundances_) {
    elem_abundance.shrink_to_fit();
  }
}

CrustMaterialFactory *CrustMaterialFactory::Instance()
{
  // [XXX] The instance is never deleted.
  return instance_ ? : (instance_ = new CrustMaterialFactory);
}
