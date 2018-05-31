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
    unsigned partonorder(double pt, edm::Handle<reco::GenParticleCollection> prtns, unsigned blw, unsigned abv) {
      unsigned nxt = (abv+blw)/2;
      if (nxt==blw) return nxt;
      double nxtpt = prtns->at(nxt).p4().Pt();
      if (nxtpt>pt)
        return partonorder(pt,prtns,blw,nxt);
      else
        return partonorder(pt,prtns,nxt,abv);
    }
    // configurable parameters //
    float mULimCEF; // Not to be set by the user
    float mULimNEF; // Not to be set by the user
    float mULimNHF; // Not to be set by the user
    float mLLimNEF; // Not to be set by the user
    float mLLimNHF; // Not to be set by the user
    bool   mAK4;
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
    edm::EDGetTokenT<GenEventInfoProduct> mEventInfo;
    edm::EDGetTokenT<vector<PileupSummaryInfo> > mSrcPU;
    edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> mJetFlavourInfosToken;
    edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> mJetFlavourInfosTokenPhysicsDef;
    // TRIGGER //
    string   mProcessName;
    vector<string> mTriggerNames;
    vector<string> mTriggerFollows;
    vector<unsigned int> mTriggerIndex; // Not to be set by the user
    edm::EDGetTokenT<edm::TriggerResults> mTriggerResultsTag;
    edm::EDGetTokenT<trigger::TriggerEvent> mTriggerEventTag;
    edm::EDGetTokenT<bool> mHBHENoiseFilterResultLabel;
    edm::EDGetTokenT<bool> mHBHENoiseFilterResultNoMinZLabel;
    edm::Handle<edm::TriggerResults>   mTriggerResultsHandle;
    edm::Handle<trigger::TriggerEvent> mTriggerEventHandle;

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

    int getMatchedPartonGen(edm::Event const& event, GenJetCollection::const_iterator i_gen);
    int getMatchedHadronGen(edm::Event const& event, GenJetCollection::const_iterator i_gen);
};

#endif
