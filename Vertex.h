#pragma once

#include <vector>

#include "TVector3.h"

class Vertex
{
    public:
        Vertex();
        virtual ~Vertex();

        double GetPosition(int i) const { return fPosition[i]; } 
        double GetTime() const { return fTime; }
        int GetNumOfVisTrack() const { return fNVisTrack; }
        int GetPDG(int i) const { return fPDG[i]; }
        double GetKineticEnergy(int i) const { return fKineticEnergy[i]; }
        double GetVisibleEnergy(int i) const { return fVisibleEnergy[i]; }
        double GetDirection(int i, int j) const { return fDirection[i][j]; }
        
        void SetPosition(const TVector3 &v) { fPosition = v; }
        void SetTime(double d) { fTime = d; }
        void AddVisTrack(int pdg, double evis, double ekin, const TVector3 &d)
        {
            fPDG.push_back( pdg );    
            fVisibleEnergy.push_back( evis );
            fKineticEnergy.push_back( ekin );
            fDirection.push_back( d );
            fNVisTrack += 1;
        }

    private:
        TVector3 fPosition;
        double fTime;
        int fNVisTrack;
        std::vector<int> fPDG;
        std::vector<double> fKineticEnergy;
        std::vector<double> fVisibleEnergy;
        std::vector<TVector3> fDirection;
};
