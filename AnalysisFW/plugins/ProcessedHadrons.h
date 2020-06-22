#ifndef ProcessedHadrons_h
#define ProcessedHadrons_h

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
#include <map>
#include <cassert>
#include <regex>
#include <TLorentzVector.h>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
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
#include "DataFormats/L1Trigger/interface/BXVector.h"
#include "DataFormats/L1Trigger/interface/Jet.h"
#include "DataFormats/L1Trigger/interface/EtSum.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "SimDataFormats/GeneratorProducts/interface/LHEEventProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/LHERunInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfo.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"

#include "PhysicsTools/PatUtils/interface/bJetSelector.h"
#include "PhysicsTools/PatExamples/interface/BTagPerformance.h"
#include "PhysicsTools/PatExamples/interface/PatBTagCommonHistos.h"

using namespace edm;
using namespace reco;
using namespace std;
using namespace trigger;

class ProcessedHadrons : public edm::EDAnalyzer
{
public:
  // Public functions for the ED analyzer
  typedef reco::Particle::LorentzVector LorentzVector;
  explicit ProcessedHadrons(edm::ParameterSet const& cfg);
  virtual void beginJob();
  virtual void beginRun(edm::Run const &, edm::EventSetup const& iSetup);
  virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
  virtual void endJob();
  virtual void beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& set);
  ~ProcessedHadrons() {}
private:
  // Configurable parameters
  bool            mSatisfactory;

  // GEN //
  bool                                                     mIsMCarlo;
  bool                                                     mNewTrigs;
  edm::EDGetTokenT<GenParticleCollection>                  mGenParticles;
  edm::EDGetTokenT<pat::PackedCandidateCollection>         mCands;

  // TRIGGER & FILTER // 
  bool                                                     mTrigObjs;
  bool                                                     mFilterPAT;
  int                                                      mFilterEcalBad;
  vector<int>                                              mFilterMissing;
  const vector<string>                                     mFilterNames;
  vector<int>                                              mFilterIndex;
  edm::EDGetTokenT<bool>                                   mFilterBitEcal;
  edm::EDGetTokenT<edm::TriggerResults>                    mFilterBitsRECO;
  edm::EDGetTokenT<edm::TriggerResults>                    mFilterBitsPAT;
  const vector<string>                                     mTriggerNames;
  edm::EDGetTokenT<edm::TriggerResults>                    mTriggerBits;
  map<string,vector<string>>                               mTriggerNamesMap;
  map<string,int>                                          mTriggerNamesIndexMap;
  vector<int>                                              mTriggerIndex;
  map<string,vector<int>>                                  mTriggerIndexMap;
  // MISC //
  HLTConfigProvider                                        mHLTConfig;
  //HLTPrescaleProvider                                      mHLTPrescale;

  edm::Service<TFileService>                               fs;
  TTree                                                   *mTree;
    
  float mPt;
  float mEta;
  float mPhi;
  float mE;
  float mEfEr;
  float mEfE;
  float mEfHr;
  float mEfH;

  // Private functions for various purposes
  bool trigUpdate(const vector<string> &tNames, vector<int> &tIndex);

  bool is_bhadr(int pdgid) {
    int aid = abs(pdgid);
    if (aid/100 == 5 or aid/1000==5) {
      return true;
    }
    return false;
  }
};


#endif
