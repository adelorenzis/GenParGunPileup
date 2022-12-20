#include "Vertex.h"

Vertex::Vertex()
{
    fPosition = TVector3(-9999., -9999., -9999.);
    fTime = -9999.;
    fNVisTrack = 0;
    fPDG.clear();
    fVisibleEnergy.clear();
    fKineticEnergy.clear();
    fDirection.clear();
   
}

Vertex::~Vertex()
{
    std::vector<int>().swap(fPDG);
    std::vector<double>().swap(fKineticEnergy);
    std::vector<double>().swap(fVisibleEnergy);
    std::vector<TVector3>().swap(fDirection);
}
