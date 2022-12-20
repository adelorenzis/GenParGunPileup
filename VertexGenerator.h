#include <iostream>
#include <vector>
#include <tuple>

#include "TFile.h"
#include "TRandom3.h"
#include "TMath.h"

#include "Vertex.h"

class VertexGenerator
{
    public:
        VertexGenerator(int seed=562792);
        virtual ~VertexGenerator();
        
        void Generate();
        int GetNumOfVertex() const { return (int)fVtxVector.size(); }
        const Vertex* GetVertex(int i) const { return fVtxVector[i]; }

        void SetMaxVertexPerEvent(int i){ fMaxVertexPerEvent = i; }
        // void SetMaxTotalVisibleEnergyPerEvent(double d){ fMaxTotalEvisPerEvent = d; }
        // void SetMaxVisibleTrackPerEvent(int i){ fMaxVisTrackPerEvent = i; }
        // void SetMinVisibleTrackPerVertex(int i){ fMinTrackPerVertex = i; }
        void SetNumTracksPerVertex(std::vector<int> v) { NumTracksPerVertex = v;}
        void SetPidPerTrackPerVertex(std::vector<std::vector<int> > v) { PidPerTrackPerVertex = v;}


    private: 
        void InitiParticleType();
        TVector3 DrawVertexPosition();
        double DrawVertexTime();
        int DrawNumOfVisTrack(int&);
        void DrawParticleType(int&, double&);
        double DrawVisEnergy(double&);
        TVector3 DrawDirection();

        TRandom3 *fRand; 
        
        int fMaxVertexPerEvent;
        // int fMinTrackPerVertex;
        // double fMaxTotalEvisPerEvent;
        // int fMaxVisTrackPerEvent;
        std::vector<int> NumTracksPerVertex;
        std::vector<std::vector<int>> PidPerTrackPerVertex;

        double fTankRadius;
        double fTankHalfHeight;
    
        double fBunchSigma;
        double fBunchPeak;

        int fNParType;
        std::vector<std::tuple<int, double, double>> fParticle;

        std::vector<Vertex*> fVtxVector; 

        double fRotXAngle;
};
