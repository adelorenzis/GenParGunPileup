// Generate pile-up particle gun events
// An event consists of >=1 vertex
// A vertex consists of >=1 visibe particles whose kinetic energies are above Cherenkov threshold
// All the paritlces of a vertex have common creation position and time like acutual neutrino interaction

#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <math.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>

#include "VertexGenerator.h"
#include "WCSimGeneRootParGunIO.h"

using std::cout;
using std::endl;
using std::string;

int fSeed = 73906;

// int fNEvtGen = 100;
// double fMaxTotalEVisPerEvent = 2000.; // MeV
int fMaxVertexPerEvent = 2;
// int fMaxTrackPerEvent = 5;
// int fMinTrackPerVertex = 1;

string fOutFileName = "out_GenParGunPileUp.root";
string fInFileName = "input.csv";

bool ParseCmdArguments(int argc, char **argv);

int main(int argc, char **argv)
{
    ParseCmdArguments(argc, argv);

    
    std::fstream file;
    file.open(fInFileName, std::ios::in);

    std::vector<std::string> row;
    std::string line, data;

    int nevents = 0;

    
    
    // This class generates vertices
    // A vertex consists of one or more tracks.
    // These tracks have the identical start position and creation time
    VertexGenerator *vg = new VertexGenerator(fSeed);

    
    // Total visible energy per event
    // This will be drawn during event generation
    // vg->SetMaxTotalVisibleEnergyPerEvent(fMaxTotalEVisPerEvent); // MeV

    // Number of vertices per event
    // This will be fixed drawn during event generation
    vg->SetMaxVertexPerEvent(fMaxVertexPerEvent);

    // Number of maximum tracks above Cherenkov threshold per event
    // NOTE that each vertex has at least one track
    // vg->SetMaxVisibleTrackPerEvent(fMaxTrackPerEvent);

    // Number of minimum tracks per vertex
    // This means a vertex should have at least fMinTrackPerVertex particles
    // vg->SetMinVisibleTrackPerVertex(fMinTrackPerVertex);

    // Save generated vertices
    WCSimGeneRootParGunIO *gr = new WCSimGeneRootParGunIO();  

    // Add extra information 
    TTree *ttmp = gr->GetTree();

    float TotalEvis = 0.;
    int NVertex = 0.;
    float VertexPos[10][3];
    float VertexTime[10];
    int VertexId[10000];
    float TrackEvis[10000];

    ttmp->Branch("TotalEvis", &TotalEvis, "TotalEvis/F");
    ttmp->Branch("NVertex", &NVertex, "NVertex/I");
    ttmp->Branch("VertexPos", VertexPos, "VertexPos[NVertex][3]/F");
    ttmp->Branch("VertexTime", VertexTime, "VertexTime[NVertex]/F");
    ttmp->Branch("TrackEvis", TrackEvis, "TrackEvis[grNPart]/F");
    
    
    
    while(file >> line) {
        row.clear();
        
        std::stringstream s(line); // used for breaking words
        
        // read every column data of a row and store it in a string variable, 'data'
        while (getline(s, data, ',')) {
            // add all the column data of a row to a vector
            row.push_back(data);
        }
        
        double Enu = stod(row[0]);
        double Etotvis = stod(row[1]);
        int ntracks = stoi(row[2]);
        
        std::vector<int> pid;
        std::vector<double> px, py, pz;
        
        for(int i = 0; i < ntracks; i++) {
            pid.push_back(stoi(row[3 + 4*i]));
            px.push_back(1000.*stod(row[4 + 4*i]));
            py.push_back(1000.*stod(row[5 + 4*i]));
            pz.push_back(1000.*stod(row[6 + 4*i]));
        }
        
        // print summary
        /*
         std::cout << "Event number: " << nevents << std::endl;
         std::cout << "Enu = " << Enu << ", Etotvis = " << Etotvis << ", ntracks = " << ntracks << std::endl;
         std::cout << "tracks (pid, px, py, pz): " << std::endl;
         for(int i = 0; i < ntracks; i++) std::cout << pid[i] << ", " << px[i] <<  ", " << py[i] << ", " << pz[i] << std::endl;
         */

        std::vector<int> NumTracksPerVertex;
        NumTracksPerVertex.push_back(ntracks); // must be generalized for MV
        vg->SetNumTracksPerVertex(NumTracksPerVertex);
        
        std::vector<std::vector<int>> PidPerTrackPerVertex;
        PidPerTrackPerVertex.push_back(pid); // must be generalized for MV
        vg->SetPidPerTrackPerVertex(PidPerTrackPerVertex);
        
        std::vector<std::vector<double> > PxPerTrackPerVertex;
        std::vector<std::vector<double> > PyPerTrackPerVertex;
        std::vector<std::vector<double> > PzPerTrackPerVertex;
        PxPerTrackPerVertex.push_back(px);
        PyPerTrackPerVertex.push_back(py);
        PzPerTrackPerVertex.push_back(pz);
        vg->SetPxPyPzPerTrackPerVertex(PxPerTrackPerVertex, PyPerTrackPerVertex, PzPerTrackPerVertex);

        
        vg->Generate();
        
        
        const int nVtx = vg->GetNumOfVertex();
        NVertex = nVtx;

        int n = 0;
        float evisTot = 0.;

        gr->grNPart = 0;
        for(int j=0; j<nVtx; j++)
        {
            const Vertex *vtx = vg->GetVertex(j);
            for(int k=0; k<3; k++){ VertexPos[j][k] = vtx->GetPosition(k); }
            VertexTime[j] = vtx->GetTime();
            
            const int nTrack = vtx->GetNumOfVisTrack();
            for(int k=0; k<nTrack; k++)
            {
                gr->grPDG[n] = vtx->GetPDG(k);
                gr->grTime[n] = vtx->GetTime();
                gr->grEkin[n] = vtx->GetKineticEnergy(k);
                for(int l=0; l<3; l++)
                {
                    gr->grPos[n][l] = vtx->GetPosition(l);
                    gr->grDir[n][l] = vtx->GetDirection(k, l);
                }
                gr->grFlagToSim[n] = true;
                gr->grVtxId[n] = j;
                TrackEvis[n] = vtx->GetVisibleEnergy(k);
                evisTot += TrackEvis[n];
                n++;
            }
        }
        TotalEvis = evisTot;
        
        gr->grNPart = n;
        gr->FillTree();
        
        
        nevents++;
    }
    

    TFile *fout = new TFile(fOutFileName.c_str(), "recreate");
    fout->cd(); 
    TTree *tout = gr->GetTree();
    tout->Write();
    fout->Close();
    cout<<" FINISHED " <<endl;
}

bool ParseCmdArguments(int argc, char **argv)
{
    cout<<" Parsing command line arguments..... " <<endl;
	for(int i=1; i<argc; i++)
	{
		cout<<"     - argv[" << i <<"] :" <<argv[i] <<endl;
	}
	cout<<endl;

	for(int i=1; i<argc; i++)
	{
		if( string(argv[i])=="-o" ) { fOutFileName = string( argv[i+1] );	i++;}
		else if( string(argv[i])=="-i" ) { fInFileName = string( argv[i+1] );    i++;}
        else if( string(argv[i])=="-s" ) { fSeed = atoi( argv[i+1] ); i++;}
        
		// else if( string(argv[i])=="-n" ) { fNEvtGen = atoi( argv[i+1] ); i++;}
        // else if( string(argv[i])=="--MaxEvis" ) { fMaxTotalEVisPerEvent = atof( argv[i+1] ); i++;}
        else if( string(argv[i])=="--MaxVtx" ) { fMaxVertexPerEvent = atoi( argv[i+1] ); i++; }
        // else if( string(argv[i])=="--MaxTrk" ) { fMaxTrackPerEvent = atoi( argv[i+1] ); i++; }
        // else if( string(argv[i])=="--MinTrk" ) { fMinTrackPerVertex = atoi( argv[i+1] ); i++; }
		else
		{
            cout<<" i: " << argv[i] <<endl;
			return false;
		}
	}
	return true;
}
