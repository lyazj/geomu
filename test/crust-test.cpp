#include "geomu/CrustMaterialFactory.h"
#include <assert.h>

using namespace std;

int main()
{
  G4Material *material = CrustMaterialFactory::Instance()->GetMaterial(2.7);
  assert(CrustMaterialFactory::Instance()->GetMaterial(1.0) != material);
  assert(CrustMaterialFactory::Instance()->GetMaterial(2.7) == material);
  assert(CrustMaterialFactory::Instance()->GetMaterial(2.7, "U2") != material);
}
