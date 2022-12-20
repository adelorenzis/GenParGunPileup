#include "VertexGenerator.h"

#include <iostream>

VertexGenerator::VertexGenerator(int seed)
{
    fRand  = new TRandom3(seed);

    fMaxVertexPerEvent = 2;
    // fMinTrackPerVertex = 1;
    // fMaxTotalEvisPerEvent = 2000.; // MeV
    // fMaxVisTrackPerEvent = 5;

    fTankRadius = 400.; // cm     
    fTankHalfHeight = 300.; // cm

    fBunchSigma = 25.;
    fBunchPeak = 200.;

    fNParType = 0;
    this->InitiParticleType();

    fRotXAngle = TMath::Pi()/2.;

    fVtxVector.clear();
}

VertexGenerator::~VertexGenerator()
{
    delete fRand; fRand = NULL;
    std::vector<std::tuple<int, double, double>>().swap(fParticle);
    std::vector<Vertex*>().swap(fVtxVector); 
}


void VertexGenerator::InitiParticleType()
{
    const double kRIndexWater = 1.34;
    const double kBetaCh = 1./kRIndexWater;
    const double kChThrFac=TMath::Sqrt( 1./(1. - kBetaCh*kBetaCh ) )-1.;
    double mass = 0.;

    // e-
    mass = 0.5109989461;
    fParticle.push_back(std::make_tuple(11, mass, kChThrFac*mass));
    // mu-
    mass = 105.6583745;
    fParticle.push_back(std::make_tuple(13, mass, kChThrFac*mass));
    // pi-
    mass = 139.57061;
    fParticle.push_back(std::make_tuple(-211, mass, kChThrFac*mass));
    // pi+
    mass = 139.57061;
    fParticle.push_back(std::make_tuple(211, mass, kChThrFac*mass));
    // pi0
    mass = 134.9770; 
    fParticle.push_back(std::make_tuple(111, mass, 0.));

    fNParType = (int)fParticle.size();
}


TVector3 VertexGenerator::DrawVertexPosition()
{
    double x, y, z, r;
    r = fTankRadius*TMath::Sqrt(fRand->Rndm());
    fRand->Circle(x, y, r);
    z = fTankHalfHeight*fRand->Uniform(-1., 1.);
    TVector3 v(x,y,z);
    v.RotateX(fRotXAngle);
    return v;
}

TVector3 VertexGenerator::DrawDirection()
{
    double x, y, z;
    fRand->Sphere(x, y, z, 1.);
    TVector3 v(x,y,z);
    v.RotateX(fRotXAngle);
    return v;
}

double VertexGenerator::DrawVertexTime()
{
    return fRand->Gaus(fBunchPeak, fBunchSigma);
}


int VertexGenerator::DrawNumOfVisTrack(int &nTrackRemain)
{
    if( nTrackRemain==0 ){ return 0; }
    int n = fRand->Integer(nTrackRemain+1);

    if( n!=0 ){ nTrackRemain -= n; }
    return n;
}

void VertexGenerator::DrawParticleType(int &pdg, double &kch)
{
    int idx = fRand->Integer(fNParType);
    pdg = std::get<0>(fParticle[idx]);
    kch = std::get<2>(fParticle[idx]);
}


double VertexGenerator::DrawVisEnergy(double &EvisRemain)
{
    double Evis = fRand->Uniform(EvisRemain);
    EvisRemain -= Evis;
    return Evis;
}

void VertexGenerator::Generate()
{
    fVtxVector.clear();

    TVector3 pos;        
    double time;
    
    // Number of vertices of this event
    const int NVertex = fMaxVertexPerEvent;

    // Each event can have at most fMaxVisTrackPerEvent visble tracks.
    // Each vertex shold have at least fMinTrackPerVertex visible tracks
    // const int MaxNVisTrackRemain = fMaxVisTrackPerEvent - NVertex*fMinTrackPerVertex;

    // NVisTrackRemain will be shared among vertices
    // If NVisTrack = 0, that measn each vertex has just one visible track
    // int NVisTrack = fRand->Integer(MaxNVisTrackRemain+1);

    // Total visible energy of this event
    // This is shared among NVertex*fMinTrackPerVertex + NVisTrack particles 
    // double TotalVisibleEnergy = fRand->Uniform(fMaxTotalEvisPerEvent);

    int pdg;
    double kch;
    for(int i=0; i<NVertex; i++)
    {
        pos = this->DrawVertexPosition();
        time = this->DrawVertexTime();
        
        
        // int nTracks = fMinTrackPerVertex + this->DrawNumOfVisTrack(NVisTrack);
        // if( i==NVertex-1 && NVisTrack>0 ){ nTracks += NVisTrack; }
        int nTracks = NumTracksPerVertex[i];
        
        Vertex *v = new Vertex();
        v->SetPosition(pos);
        v->SetTime(time);
        for(int j=0; j<nTracks; j++)
        {
            // this->DrawParticleType(pdg, kch);
            pdf = PidPerTrackPerVertex[i][j];
            // kch = TO DO 
            
            TVector3 dir = this->DrawDirection();
            double evis = this->DrawVisEnergy(TotalVisibleEnergy);
            if( i==NVertex-1 && j==nTracks-1 ){ evis +=TotalVisibleEnergy ; }
            double ekin = evis + kch;
            v->AddVisTrack(pdg, evis, ekin, dir);
        }
        fVtxVector.push_back(v);
    }
}
