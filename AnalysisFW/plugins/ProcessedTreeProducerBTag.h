#ifndef ProcessedTreeProducerBTag_h
#define ProcessedTreeProducerBTag_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/JetCorrFactors.h"
#include "SMPJ/AnalysisFW/interface/QCDJet.h"
#include "SMPJ/AnalysisFW/interface/QCDEvent.h"
#include "SMPJ/AnalysisFW/interface/QCDEventHdr.h"
#include "SMPJ/AnalysisFW/interface/QCDPFJet.h"
#include "SMPJ/AnalysisFW/interface/QCDPFJetBTag.h"
#include "SMPJ/AnalysisFW/interface/QCDMET.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

//Hadron level definition
#include "SimDataFormats/JetMatching/interface/JetFlavourInfo.h"              
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"
#include "DataFormats/Math/interface/deltaR.h"

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
    //---- configurable parameters --------
    bool   mAK4;
    bool   mPrintTriggerMenu;
    bool   isPFJecUncSetCHS_; // Not to be set by the user
    int    mGoodVtxNdof;
    double mGoodVtxZ;
    int    mMinNPFJets;
    double mMinPFPt,mMinPFPtThirdJet,mMinGenPt,mMaxY,mMinJJMass;
    std::string mPFPayloadNameCHS;
    std::string mRunYear;
    std::string mPFJetPUIDCHS;

    // unc file for CHS jet ---- //
    std::string mPFJECUncSrcCHS;
    std::vector<std::string> mPFJECUncSrcNames;
    edm::EDGetTokenT<reco::VertexCollection> mOfflineVertices;
    edm::EDGetTokenT<reco::BeamSpot> mBeamSpot;
    edm::EDGetTokenT<edm::View<pat::Jet> >mPFJetsNameCHS;
    edm::EDGetTokenT<double> mSrcCaloRho;
    edm::EDGetTokenT<double> mSrcPFRho;
    //---- MET -----------------------------
    edm::EDGetTokenT<pat::METCollection> mPFMETt1;
    edm::EDGetTokenT<pat::METCollection> mPFMETt0pc;
    edm::EDGetTokenT<pat::METCollection> mPFMETt0pct1;
    //---- GEN -----------------------------
    bool   mIsMCarlo;
    bool   mUseGenInfo;
    int    mMCType;
    edm::EDGetTokenT<GenJetCollection> mGenJetsName;
    edm::EDGetTokenT<reco::GenParticleCollection> mGenParticles;
    edm::EDGetTokenT<GenEventInfoProduct> mEventInfo;
    edm::EDGetTokenT<std::vector<PileupSummaryInfo> > mSrcPU;
    edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> jetFlavourInfosToken_;
    edm::EDGetTokenT<reco::JetFlavourInfoMatchingCollection> jetFlavourInfosTokenPhysicsDef_;
    //---- TRIGGER -------------------------
    std::string   processName_;
    std::vector<std::string> triggerNames_;
    std::vector<std::string> goodTriggerNames_; // Not to be set by the user
    std::vector<unsigned int> triggerIndex_; // Not to be set by the user
    edm::EDGetTokenT<edm::TriggerResults> triggerResultsTag_;
    edm::EDGetTokenT<trigger::TriggerEvent> triggerEventTag_;
    edm::EDGetTokenT<bool> mHBHENoiseFilterResultLabel;
    edm::EDGetTokenT<bool> mHBHENoiseFilterResultNoMinZLabel;
    edm::Handle<edm::TriggerResults>   triggerResultsHandle_;
    edm::Handle<trigger::TriggerEvent> triggerEventHandle_;

    HLTConfigProvider hltConfig_;
    //---- CORRECTORS ----------------------
    const JetCorrector *mPFJEC;
    // ---- non CHS jet uncertainty ------ //
    JetCorrectionUncertainty *mPFUncCHS;
    // -------- CHS jet uncertainty sources -------- //
    std::vector<JetCorrectionUncertainty*> mPFUncSrcCHS;

    edm::Service<TFileService> fs;
    TTree *mTree;
    TH1F *mTriggerPassHisto,*mTriggerNamesHisto;
    //---- TREE variables --------
    QCDEvent *mEvent;

    int getMatchedPartonGen(edm::Event const& event, GenJetCollection::const_iterator i_gen);
    int getMatchedHadronGen(edm::Event const& event, GenJetCollection::const_iterator i_gen);

    HLTPrescaleProvider hltPrescale_;
};

#endif
