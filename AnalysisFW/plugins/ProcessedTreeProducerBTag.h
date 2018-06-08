#ifndef ProcessedTreeProducerBTag_h
#define ProcessedTreeProducerBTag_h

#include <iostream>
#include <sstream>
#include <istream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <functional>
#include "TTree.h"
#include <vector>
#include <cassert>
#include <TLorentzVector.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"

#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/PFJet.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/JetReco/interface/JetExtendedAssociation.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "DataFormats/METReco/interface/HcalNoiseSummary.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "PhysicsTools/PatUtils/interface/bJetSelector.h"
#include "PhysicsTools/PatExamples/interface/BTagPerformance.h"
#include "PhysicsTools/PatExamples/interface/PatBTagCommonHistos.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfo.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"

#include "SMPJ/AnalysisFW/interface/QCDJet.h"
#include "SMPJ/AnalysisFW/interface/QCDEvent.h"
#include "SMPJ/AnalysisFW/interface/QCDEventHdr.h"
#include "SMPJ/AnalysisFW/interface/QCDPFJet.h"
#include "SMPJ/AnalysisFW/interface/QCDPFJetBTag.h"
#include "SMPJ/AnalysisFW/interface/QCDMET.h"

using namespace edm;
using namespace reco;
using namespace std;
using namespace trigger;

class ProcessedTreeProducerBTag : public edm::EDAnalyzer
{
  public:
  // Public functions for the ED analyzer
    typedef reco::Particle::LorentzVector LorentzVector;
    explicit ProcessedTreeProducerBTag(edm::ParameterSet const& cfg);
    virtual void beginJob();
    virtual void beginRun(edm::Run const &, edm::EventSetup const& iSetup);
    virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
    virtual void endJob();
    ~ProcessedTreeProducerBTag() {}
  private:
  // Configurable parameters
    float        mULimCEF; // Not to be set by the user
    float        mULimNEF; // Not to be set by the user
    float        mULimNHF; // Not to be set by the user
    float        mLLimNEF; // Not to be set by the user
    float        mLLimNHF; // Not to be set by the user
    bool         mAK4;
    bool         mPrintTriggerMenu;
    bool         mIsPFJecUncSet; // Not to be set by the user
    bool         mUseLegacyTag; // Not to be set by the user
    int          mGoodVtxNdof;
    double       mGoodVtxZ;
    unsigned int mMinNPFJets;
    double       mMinPFPt,mMinPFPtThirdJet,mMinGenPt,mMaxEta;
    string       mPFPayloadName;
    string       mRunYear;
    string       mPFJetPUID;

    string mPFJECUncSrc;
    vector<string> mPFJECUncSrcNames;
    edm::EDGetTokenT<reco::VertexCollection> mOfflineVertices;
    edm::EDGetTokenT<reco::BeamSpot> mBeamSpot;
    edm::EDGetTokenT<edm::View<pat::Jet> >mPFJetsName;
    edm::EDGetTokenT<double> mSrcCaloRho;
    edm::EDGetTokenT<double> mSrcPFRho;
    // MET //
    edm::EDGetTokenT<pat::METCollection> mPFMETt1;
    edm::EDGetTokenT<pat::METCollection> mPFMETt0pc;
    edm::EDGetTokenT<pat::METCollection> mPFMETt0pct1;
    // GEN //
    bool   mIsMCarlo;
    bool   mUseGenInfo;
    int    mMCType;
    edm::EDGetTokenT<GenJetCollection> mGenJetsName;
    edm::EDGetTokenT<reco::GenParticleCollection> mGenParticles;
    edm::Handle<reco::GenParticleCollection>      mGenParts;
    edm::EDGetTokenT<GenEventInfoProduct> mEventInfo;
    edm::EDGetTokenT<vector<PileupSummaryInfo> > mSrcPU;
    edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> mJetFlavourInfosToken;
    edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> mJetFlavourInfosTokenPhysicsDef;
    // TRIGGER //
    string   mProcessName;
    vector<string> mTriggerNames;
    vector<string> mTriggerFlwNames;
    vector<int> mTriggerIndex; // Not to be set by the user
    vector<int> mTriggerFlwIndex; // Not to be set by the user
    edm::EDGetTokenT<edm::TriggerResults> mTriggerResultsTag;
    edm::EDGetTokenT<trigger::TriggerEvent> mTriggerEventTag;
    edm::EDGetTokenT<bool> mHBHENoiseFilterResultNoMinZLabel;

    HLTConfigProvider mHLTConfig;
    HLTPrescaleProvider mHLTPrescale;
    // CORRECTORS //
    const JetCorrector *mPFJEC;
    JetCorrectionUncertainty *mPFUnc;
    vector<JetCorrectionUncertainty*> mPFUncSrc;

    edm::Service<TFileService> fs;
    TTree *mTree;
    TH1F *mTriggerPassHisto,*mTriggerNamesHisto;
    QCDEvent *mEvent;

    vector<int>           mGenFlavour;
    vector<int>           mGenFlavourHadr;
    vector<int>           mGenFlavourPhys;
    vector<LorentzVector> mGenJets;

  // Private functions for various purposes

    // Sort the QCDPFJet collection according to pt
    static bool sort_pfjets(QCDPFJet j1, QCDPFJet j2) { return j1.ptCor() > j2.ptCor(); }

    // Search for the index of the given pt, genjets
    unsigned gen_ptpos(double pt, unsigned blw, unsigned abv) {
      unsigned nxt = (abv+blw)/2;
      if (nxt==blw) return nxt;
      double nxtpt = mGenJets[nxt].pt();
      if (nxtpt<=pt) return gen_ptpos(pt,blw,nxt);
      else           return gen_ptpos(pt,nxt,abv);
    }

    // Search for the index of the given pt, partons
    unsigned parton_ptpos(double pt, edm::Handle<reco::GenParticleCollection> prtns, unsigned blw, unsigned abv) {
      unsigned nxt = (abv+blw)/2;
      if (nxt==blw) return nxt;
      double nxtpt = prtns->at(nxt).p4().Pt();
      if (nxtpt>pt) return parton_ptpos(pt,prtns,blw,nxt);
      else          return parton_ptpos(pt,prtns,nxt,abv);
    }

    // Find the gen jet best matching to the current jet
    pair<int,float> best_genjet(const pat::Jet &jet) {
      int imin = -1;
      float rmin = 999.;

      float ratiouplim = (jet.pt()>20 ? 1.5 : 2.5);
      float ratiololim = (jet.pt()>20 ? 0.4 : 0.25);
      // Find approximately the closest pt and then iterate up and down
      // If a good match is found, iteration stops
      int limit = mGenJets.size();
      int dwn = gen_ptpos(jet.pt(),0,limit-1);
      int up = dwn+1;
      // Iterate the pt-ordered genjet set up & down
      while (dwn>=0 or up<limit) {
        // Take a step downwards (towards greater pt)
        if (dwn>=0) {
          if ((mGenJets[dwn].pt())/(jet.pt())<ratiouplim) {
            float deltaR = reco::deltaR(jet,mGenJets[dwn]);
            if (deltaR < rmin) {
              imin = dwn;
              rmin = deltaR;
              if (rmin<0.2) break; // Stop if the delta R match is good enough
            }
            --dwn;
          } else {
            // Ignore the "down" end when we reach the pt threshold
            dwn = -1;
          }
        }
        if (up<limit) {
          // Take a step upwards (towards smaller pt)
          if ((mGenJets[up].pt())/(jet.pt())>ratiololim) {
            float deltaR = reco::deltaR(jet,mGenJets[up]);
            if (deltaR < rmin) {
              imin = up;
              rmin = deltaR;
              if (rmin<0.2) break; // Stop if the delta R match is good enough
            }
            ++up;
          } else {
            // Ignore the "up" end when we reach the pt threshold
            up = limit;
          }
        }
      }
      return make_pair(imin,rmin);
    }

    // Do genparticles - genjet matching by hand (mGenParts needs to be filled)
    int get_gjetpartonflav(edm::Event const& event, GenJetCollection::const_iterator igen) {
      int jetFlavour = 0;
      bool switchB=false;
      bool switchC=false;
      
      double DeltaRmin=0.3;
      for (size_t i = 0; i < mGenParts->size (); ++i) {
        const GenParticle &genP = mGenParts->at(i);
        int pdgId = genP.pdgId();
        double DeltaR=deltaR(genP,*igen);
        if (DeltaR < DeltaRmin) {
          DeltaRmin=DeltaR;
          if(abs(pdgId)==5) { jetFlavour=5; switchB=true; }
          if(abs(pdgId)==4) { jetFlavour=4; switchC=true; }
          if(abs(pdgId)<=3 and abs(pdgId)>=1) { jetFlavour=1; }
          if(abs(pdgId)==21){ jetFlavour=21; }
        }  
        if (switchB) { jetFlavour=5; }
        if (switchC && !switchB) { jetFlavour=4; }
      }
      return jetFlavour;
    }

    // Do genhadron - genjet matching by hand (mGenParts needs to be filled)
    int get_gjethadronflav(edm::Event const& event, GenJetCollection::const_iterator igen) {
      int jetFlavour = 0;
    
      for (size_t i = 0; i < mGenParts->size(); ++i) {
        const GenParticle &genP = mGenParts->at(i);
        int aid = abs(genP.pdgId());
        if (aid/100 == 5 || aid/1000==5) {
          // 2J+1 == 1 (mesons) or 2 (baryons)
          if (aid%10 == 1 || aid%10 == 2) {
            // No B decaying to B
            if (aid != 5222 && aid != 5112 && aid != 5212 && aid != 5322) {
              double DeltaR=deltaR(genP,*igen);
              if(sqrt(DeltaR)<0.5){
                jetFlavour=5;
              }
              else jetFlavour=21;
            }
          }
        }
      }
      return jetFlavour;
    }
};

#endif
