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

#include "SMPJ/AnalysisFW/interface/QCDJet.h"
#include "SMPJ/AnalysisFW/interface/QCDEvent.h"
#include "SMPJ/AnalysisFW/interface/QCDEventHdr.h"
#include "SMPJ/AnalysisFW/interface/QCDPFJet.h"
#include "SMPJ/AnalysisFW/interface/QCDMET.h"

using namespace edm;
using namespace reco;
using namespace std;
using namespace trigger;

class GenEventInfoProduct;
class GenLumiInfoHeader;
class LHEEventProduct;
class LHERunInfoProduct;

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
  virtual void beginLuminosityBlock(edm::LuminosityBlock const& iLumi, edm::EventSetup const& set);
  ~ProcessedTreeProducerBTag() {}
private:
  // Configurable parameters
  bool            mSaveWeights; // Not to be set by the user
  bool            mAK4;
  bool            mZB;
  bool            mPrintTriggerMenu;
  bool            mIsPFJecUncSet; // Not to be set by the user
  int             mGoodVtxNdof;
  double          mGoodVtxZ;
  unsigned int    mMinNPFJets;
  double          mMinPFPt,mMinPFPtThirdJet,mMinGenPt,mMaxEta;
  string          mPFPayloadName;
  string          mRunYear; 
  string          mPFJetPUID;
  bool            mSatisfactory;

  // unc file for CHS jet
  string                                   mPFJECUncSrc;
  vector<string>                           mPFJECUncSrcNames;
  edm::EDGetTokenT<reco::VertexCollection> mOfflineVertices;
  edm::EDGetTokenT<reco::BeamSpot>         mBeamSpot;
  edm::EDGetTokenT<edm::View<pat::Jet> >   mPFJetsName;
  edm::EDGetTokenT<double>                 mSrcCaloRho;
  edm::EDGetTokenT<double>                 mSrcPFRho;
  // MET //
  edm::EDGetTokenT<pat::METCollection> mPFMETt1;
  // GEN //
  bool                                                     mIsMCarlo;
  bool                                                     mUseGenInfo;
  bool                                                     mNewTrigs;
  int                                                      mMCType;
  edm::EDGetTokenT<GenJetCollection>                       mGenJetsName;
  edm::EDGetTokenT<GenParticleCollection>                  mGenParticles;
  edm::Handle<GenParticleCollection>                       mGenParts;
  edm::EDGetTokenT<GenEventInfoProduct>                    mEventInfo;
  edm::EDGetTokenT<std::vector<PileupSummaryInfo> >        mSrcPU;
  edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> mJetFlavourInfosToken;
  edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> mJetFlavourInfosTokenPhysicsDef;
  edm::EDGetTokenT<edm::ValueMap<float>>                   mQGLToken;
  edm::EDGetTokenT<edm::ValueMap<float>>                   mQGAx2Token;
  edm::EDGetTokenT<edm::ValueMap<int>>                     mQGMulToken;
  edm::EDGetTokenT<edm::ValueMap<float>>                   mQGPtDToken;

  edm::InputTag                                            mGenEventSrc_;
  edm::EDGetTokenT<GenEventInfoProduct>                    mGenEventToken_;
  edm::EDGetTokenT<GenLumiInfoHeader>                      mLumiHeaderToken_;

  edm::InputTag                                            mLHEEventSrc_;
  edm::EDGetTokenT<LHEEventProduct>                        mLHEEventToken_;
  edm::EDGetTokenT<LHERunInfoProduct>                      mLHEInfoToken_;

  // TRIGGER & FILTER // 
  bool                                                     mDiscardFilter;
  bool                                                     mTrigSimple;
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
  const vector<string>                                     mTriggerNamesFlw;
  map<string,vector<string>>                               mTriggerNamesMap;
  map<string,int>                                          mTriggerNamesIndexMap;
  vector<int>                                              mTriggerIndex;
  vector<int>                                              mTriggerIndexFlw;
  map<string,vector<int>>                                  mTriggerIndexMap;
  edm::EDGetTokenT<edm::TriggerResults>                    mTriggerBits;
  edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> mTriggerHLTObjs;
  edm::EDGetTokenT<BXVector<l1t::Jet> >                    mTriggerL1Objs;
  edm::EDGetTokenT<BXVector<l1t::EtSum> >                  mTriggerL1HTObjs;
  edm::EDGetTokenT<pat::PackedTriggerPrescales>            mTriggerPrescales;
  edm::EDGetTokenT<pat::PackedTriggerPrescales>            mTriggerPrescalesL1Min;
  edm::EDGetTokenT<pat::PackedTriggerPrescales>            mTriggerPrescalesL1Max;
  // CORRECTORS //
  JetCorrectionUncertainty                                *mPFUnc;
  vector<JetCorrectionUncertainty*>                        mPFUncSrc;
  // MISC //
  edm::EDGetTokenT<bool>                                   mHBHENoiseFilterResultNoMinZLabel;
  edm::EDGetTokenT<pat::PackedCandidateCollection>         mCands;
  HLTConfigProvider                                        mHLTConfig;
  HLTPrescaleProvider                                      mHLTPrescale;

  edm::Service<TFileService>                               fs;
  TTree                                                   *mTree;
  TH1F                                                    *mTriggerPassHisto,*mTriggerNamesHisto,*mFilterActiveHisto,*mPSWeightHisto;
  QCDEvent                                                *mEvent;

  vector<int>           mGenFlavour;
  vector<int>           mGenFlavourHadr;
  vector<int>           mGenFlavourPhys;
  vector<float>         mGenBPt;
  vector<LorentzVector> mGenJets;
  map<int,int>          mGenJetPhysFlav;
  map<int,int>          mGenJetAlgoFlav;

  bool                  mRedoPhysDef;
  bool                  mRedoAlgoDef;
    
  // Private functions for various purposes
  bool trigUpdate(const vector<string> &tNames, vector<int> &tIndex, bool active);
  bool trigCheck(const edm::TriggerNames &names, const vector<string> &tNames, vector<int> &tIndex, bool active);
  
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

  bool is_bhadr(int pdgid) {
    int aid = abs(pdgid);
    if (aid/100 == 5 or aid/1000==5) {
      return true;
    }
    return false;
  }

  // Find the gen jet best matching to the current jet
  template<typename T>      
  pair<int,float> best_genjet(const T &jet) {
    int imin = -1;
    float rmin = 999.;

    int limit = mGenJets.size();
    if (limit>0) {
      float ratiouplim = (jet.pt()>20 ? 1.5 : 5.0);
      float ratiololim = (jet.pt()>20 ? 0.4 : 0.2);
      // Find approximately the closest pt and then iterate up and down
      // If a good match is found, iteration stops
      int dwn = (jet.pt()>20 ? gen_ptpos(jet.pt(),0,limit-1) : limit-1);
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
    }
    return make_pair(imin,rmin);
  }

  // Find the gen jet best matching to the current jet
  template<typename T>      
  pair<int,float> best_jet4prtn(const T &prtn) {
    int imin = -1;
    float rmin = 999.;

    // Iterate the pt-ordered genjet set up & down
    for (unsigned igen = 0; igen < mGenJets.size(); ++igen) {
      float deltaR = reco::deltaR(prtn,mGenJets[igen]);
      if (deltaR<rmin) {
        imin = igen;
        rmin = deltaR;
      }
    }
    return make_pair(imin,rmin);
  }

  void flavs_physdef() {
    mGenJetPhysFlav.clear();
    vector<int> hpParts;
    for (unsigned i = 0; i<mGenParts->size(); ++i) {
      if (hpParts.size()==2) break;
      const auto &gPart = mGenParts->at(i);
      int id = gPart.pdgId();
      int absId = abs(id);
      if (absId>10 and absId!=21) continue; // Non-partons
      if (fabs(gPart.eta())>5.0) continue; // Bad eta region
  
      if (mMCType==0) { // Pythia 8
        if (abs(gPart.pdgId())!=23) continue;
      } else if (mMCType==1) { // Herwig++
        bool hardProc = false;
        if (gPart.numberOfMothers()==1) {
          auto &moths = gPart.motherRefVector();
          auto *moth = moths.at(0).get();
          if (moth!=nullptr and moth->pdgId()!=2212 and fabs(moth->eta())>10.0) hardProc = true; 
        }
        if (!hardProc) continue;
      } else {
        return;
      }
      hpParts.push_back(i);
    }

    for (auto &ihp : hpParts) {
      auto &hp = mGenParts->at(ihp);
      float drmin = 999.;
      int imin = -1;
      for (unsigned igen = 0; igen < mGenJets.size(); ++igen) {
        auto &gjet = mGenJets[igen];
        float dR = reco::deltaR(gjet,hp);
        if (dR<drmin) {
          drmin = dR;
          imin = igen;
        }
      }
      if (imin!=-1 and drmin<0.4) mGenJetPhysFlav[imin] = hp.pdgId();
    }
  }

  void flavs_newalgodef() {
    mGenJetAlgoFlav.clear();
    map<int,int> jet2prtn;
    for (unsigned i = 0; i<mGenParts->size(); ++i) {
      const auto &gPart = mGenParts->at(i);
      int id = gPart.pdgId();
      int absId = abs(id);
      if (absId>10 and absId!=21) continue; // Non-partons
      if (fabs(gPart.eta())>5.0) continue; // Bad eta region
  
      if (mMCType!=0 and mMCType!=1) // Non- Pythia8 & Herwig++
        return;

      float drmin = 999.;
      int imin = -1;
      for (unsigned igen = 0; igen < mGenJets.size(); ++igen) {
        auto &gjet = mGenJets[igen];
        float dR = reco::deltaR(gjet,gPart);
        if (dR<drmin) {
          drmin = dR;
          imin = igen;
        }
      }
      if (imin!=-1 and drmin<0.4) {
        if (jet2prtn.find(imin)==jet2prtn.end()) {
          jet2prtn[imin] = i;
        } else {
          auto &currBest = mGenParts->at(jet2prtn[imin]);
          if (gPart.pt()>currBest.pt())
            jet2prtn[imin] = i;
        }
      }
    }
    for (auto &j2p : jet2prtn) {
      auto &bestPrtn = mGenParts->at(j2p.second);
      mGenJetAlgoFlav[j2p.first] = bestPrtn.pdgId();
    }
  }
};


#endif
