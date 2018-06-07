#ifndef ProcessedTreeProducerBTag_h
#define ProcessedTreeProducerBTag_h

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
#include <regex>
#include <TLorentzVector.h>

using namespace edm;
using namespace reco;
using namespace std;
using namespace trigger;

class ProcessedTreeProducerBTag : public edm::EDAnalyzer
{
  public:
    typedef reco::Particle::LorentzVector LorentzVector;
    explicit ProcessedTreeProducerBTag(edm::ParameterSet const& cfg);
    virtual void beginJob();
    virtual void beginRun(edm::Run const &, edm::EventSetup const& iSetup);
    virtual void analyze(edm::Event const& evt, edm::EventSetup const& iSetup);
    virtual void endJob();
    virtual ~ProcessedTreeProducerBTag();
  private:
    void buildTree();
    static bool sort_pfjets(QCDPFJet j1, QCDPFJet j2) {
      return j1.ptCor() > j2.ptCor();
    }
    unsigned genorder(double pt, Handle<GenJetCollection> gjets, unsigned blw, unsigned abv) {
      unsigned nxt = (abv+blw)/2;
      if (nxt==blw) return nxt;
      double nxtpt = gjets->at(nxt).p4().Pt();
      if (nxtpt<=pt)
        return genorder(pt,gjets,blw,nxt);
      else
        return genorder(pt,gjets,nxt,abv);
    }
    unsigned partonorder(double pt, edm::Handle<reco::GenParticleCollection> prtns, unsigned blw, unsigned abv)     {
      unsigned nxt = (abv+blw)/2;
      if (nxt==blw) return nxt;
      double nxtpt = prtns->at(nxt).p4().Pt();
      if (nxtpt>pt)
        return partonorder(pt,prtns,blw,nxt);
      else
        return partonorder(pt,prtns,nxt,abv);
    }

    //---- configurable parameters --------
    float  mULimCEF; // Not to be set by the user
    float  mULimNEF; // Not to be set by the user
    float  mULimNHF; // Not to be set by the user
    float  mLLimNEF; // Not to be set by the user
    float  mLLimNHF; // Not to be set by the user
    bool   mSaveWeights; // Not to be set by the user
    bool   mAK4;
    bool   mZB;
    bool   mPrintTriggerMenu;
    bool   mIsPFJecUncSet; // Not to be set by the user
    bool   mUseLegacyTag; // Not to be set by the user
    int    mGoodVtxNdof;
    double mGoodVtxZ;
    int    mMinNPFJets;
    double mMinPFPt,mMinPFPtThirdJet,mMinGenPt,mMaxY,mMinJJMass;
    string mPFPayloadName;
    string mRunYear; 
    string mPFJetPUID;

    // unc file for CHS jet //
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
    edm::EDGetTokenT<reco::GenParticleCollection>            mGenParticles;
    edm::EDGetTokenT<GenEventInfoProduct>                    mEventInfo;
    edm::EDGetTokenT<GenEventInfoProduct>                    mGenEvtInfo;
    edm::Handle<GenEventInfoProduct>                         mGenEvtInfoHandle;
    edm::EDGetTokenT<std::vector<PileupSummaryInfo> >        mSrcPU;
    edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> mJetFlavourInfosToken;
    edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> mJetFlavourInfosTokenPhysicsDef;
    edm::EDGetTokenT<edm::ValueMap<float>>                   mQGLToken;
    edm::EDGetTokenT<edm::ValueMap<float>>                   mQGAx2Token;
    edm::EDGetTokenT<edm::ValueMap<int>>                     mQGMulToken;
    edm::EDGetTokenT<edm::ValueMap<float>>                   mQGPtDToken;
    // TRIGGER // 
    string                                                   mProcessName;
    const vector<string>                                     mFilterNames;
    const vector<string>                                     mTriggerNames;
    const vector<string>                                     mTriggerFlwNames;
    vector<int>                                              mFilterIndex;
    vector<int>                                              mTriggerIndex;
    vector<int>                                              mTriggerFlwIndex;
    vector<int>                                              mTriggerFlgIndex;
    edm::EDGetTokenT<edm::TriggerResults>                    mFilterBits;
    edm::EDGetTokenT<edm::TriggerResults>                    mTriggerBits;
    edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> mTriggerHLTObjs;
    edm::EDGetTokenT<BXVector<l1t::Jet> >                    mTriggerL1Objs;
    edm::EDGetTokenT<BXVector<l1t::EtSum> >                  mTriggerL1HTObjs;
    edm::EDGetTokenT<pat::PackedTriggerPrescales>            mTriggerPrescales;
    edm::EDGetTokenT<pat::PackedTriggerPrescales>            mTriggerPrescalesL1Min;
    edm::EDGetTokenT<pat::PackedTriggerPrescales>            mTriggerPrescalesL1Max;
    TH1F *mTriggerPassHisto,*mTriggerNamesHisto;
    // CORRECTORS //
    JetCorrectionUncertainty                                *mPFUnc;
    vector<JetCorrectionUncertainty*>                        mPFUncSrc;
    // MISC //
    edm::EDGetTokenT<bool>                                   mHBHENoiseFilterResultNoMinZLabel;
    edm::EDGetTokenT<pat::PackedCandidateCollection>         mCands;
    HLTConfigProvider                                        mHLTConfig;
    HLTPrescaleProvider                                      mHLTPrescale;

    edm::Service<TFileService> fs;
    TTree *mTree;
    QCDEvent *mEvent;

    int getMatchedPartonGen(edm::Event const& event, GenJetCollection::const_iterator i_gen);
    int getMatchedHadronGen(edm::Event const& event, GenJetCollection::const_iterator i_gen);

};

#endif
