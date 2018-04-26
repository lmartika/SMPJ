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

#include "SMPJ/AnalysisFW/plugins/ProcessedTreeProducerBTag.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Common/interface/TriggerResultsByName.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Math/interface/deltaR.h"
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

#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "HLTrigger/HLTcore/interface/HLTPrescaleProvider.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"

#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"

#include "PhysicsTools/PatUtils/interface/bJetSelector.h"
#include "PhysicsTools/PatExamples/interface/BTagPerformance.h"
#include "PhysicsTools/PatExamples/interface/PatBTagCommonHistos.h"

#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/ValueMap.h"

//hadron-level definition
#include "SimDataFormats/JetMatching/interface/JetFlavourInfo.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourInfoMatching.h"
#include "DataFormats/Math/interface/deltaR.h"

ProcessedTreeProducerBTag::ProcessedTreeProducerBTag(edm::ParameterSet const& cfg):
  mAK4(cfg.getUntrackedParameter                                                                                     <bool>("AK4",false)),
  mPrintTriggerMenu(                                                                        cfg.getUntrackedParameter<bool>("printTriggerMenu",false)),
  isPFJecUncSetCHS_(false),
  // Cut params
  mGoodVtxNdof(                                                                                    cfg.getParameter<double>("goodVtxNdof")),
  mGoodVtxZ(                                                                                       cfg.getParameter<double>("goodVtxZ")),
  mMinNPFJets(                                                                                        cfg.getParameter<int>("minNPFJets")),
  mMinPFPt(                                                                                        cfg.getParameter<double>("minPFPt")),
  mMinPFPtThirdJet(                                                                                cfg.getParameter<double>("minPFPtThirdJet")),
  mMinGenPt(                                                                              cfg.getUntrackedParameter<double>("minGenPt",20)),
  mMaxY(                                                                                           cfg.getParameter<double>("maxY")),
  mMinJJMass(                                                                                      cfg.getParameter<double>("minJJMass")),
  // Misc
  mPFPayloadNameCHS(                                                                          cfg.getParameter<std::string>("PFPayloadNameCHS")),
  mRunYear(                                                                          cfg.getUntrackedParameter<std::string>("runYear","2016")),
  mPFJetPUIDCHS(                                                                              cfg.getParameter<std::string>("pfchsjetpuid")),
  mPFJECUncSrcCHS(                                                                   cfg.getUntrackedParameter<std::string>("jecUncSrcCHS","")),
  mPFJECUncSrcNames(                                                            cfg.getParameter<std::vector<std::string> >("jecUncSrcNames")),
  mOfflineVertices(mayConsume<reco::VertexCollection>(                                      cfg.getParameter<edm::InputTag>("offlineVertices"))),
  mBeamSpot(mayConsume<reco::BeamSpot>(                                                     cfg.getParameter<edm::InputTag>("beamSpot"))),
  mPFJetsNameCHS(consumes<edm::View<pat::Jet>>(                                             cfg.getParameter<edm::InputTag>("pfjetschs"))),
  // Rho
  mSrcCaloRho(mayConsume<double>(                                                           cfg.getParameter<edm::InputTag>("srcCaloRho"))),
  mSrcPFRho(mayConsume<double>(                                                             cfg.getParameter<edm::InputTag>("srcPFRho"))),
  // MET
  mPFMETt1(mayConsume<pat::METCollection>(                                                  cfg.getParameter<edm::InputTag>("pfmetT1"))),
  mPFMETt0pc(mayConsume<pat::METCollection>(                                                cfg.getParameter<edm::InputTag>("pfmetT0pc"))),
  mPFMETt0pct1(mayConsume<pat::METCollection>(                                              cfg.getParameter<edm::InputTag>("pfmetT0pcT1"))),
  // GEN
  mIsMCarlo(                                                                                cfg.getUntrackedParameter<bool>("isMCarlo",false)),
  mUseGenInfo(                                                                              cfg.getUntrackedParameter<bool>("useGenInfo",false)),
  mMCType(                                                                                   cfg.getUntrackedParameter<int>("mcType",0)), // 0 for Pythia, 1 for Herwig++
  mGenJetsName(mayConsume<GenJetCollection>(                                       cfg.getUntrackedParameter<edm::InputTag>("genjets",edm::InputTag("")))),
  mGenParticles(consumes<reco::GenParticleCollection>(                             cfg.getUntrackedParameter<edm::InputTag>("GenParticles",edm::InputTag("")))),
  mEventInfo(consumes<GenEventInfoProduct>(                                        cfg.getUntrackedParameter<edm::InputTag>("EventInfo",edm::InputTag("")))),
  mSrcPU(mayConsume<std::vector<PileupSummaryInfo> >(                              cfg.getUntrackedParameter<edm::InputTag>("srcPULabel",edm::InputTag("")))),
  jetFlavourInfosToken_(consumes<reco::JetFlavourInfoMatchingCollection>(          cfg.getUntrackedParameter<edm::InputTag>("jetFlavourInfos",edm::InputTag("")))),
  jetFlavourInfosTokenPhysicsDef_(consumes<reco::JetFlavourInfoMatchingCollection>(cfg.getUntrackedParameter<edm::InputTag>("jetFlavourInfosPhysicsDef",edm::InputTag("")))),
  // Trigger
  processName_(                                                                      cfg.getUntrackedParameter<std::string>("processName","")),
  triggerNames_(                                                                cfg.getParameter<std::vector<std::string> >("triggerName")),
  triggerResultsTag_(mayConsume<edm::TriggerResults>(                              cfg.getUntrackedParameter<edm::InputTag>("triggerResults",edm::InputTag("")))),
  triggerEventTag_(mayConsume<trigger::TriggerEvent>(                              cfg.getUntrackedParameter<edm::InputTag>("triggerEvent",edm::InputTag("")))),
  mHBHENoiseFilterResultLabel(mayConsume<bool>(                                    cfg.getUntrackedParameter<edm::InputTag>("HBHENoiseFilterResultLabel",edm::InputTag("")))),
  mHBHENoiseFilterResultNoMinZLabel(mayConsume<bool>(                              cfg.getUntrackedParameter<edm::InputTag>("HBHENoiseFilterResultNoMinZLabel",edm::InputTag("")))),
  hltPrescale_(cfg, consumesCollector(), *this)
{}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::beginJob()
{
  mTree = fs->make<TTree>("ProcessedTree","ProcessedTree");
  mEvent = new QCDEvent();
  mTree->Branch("events","QCDEvent",&mEvent);
  mTriggerNamesHisto = fs->make<TH1F>("TriggerNames","TriggerNames",1,0,1);
  mTriggerNamesHisto->SetBit(TH1::kUserContour);
  mTriggerPassHisto = fs->make<TH1F>("TriggerPass","TriggerPass",1,0,1);
  mTriggerPassHisto->SetBit(TH1::kUserContour);
  mUseLegacyTag = (mRunYear=="2016" and mMCType==1);
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::endJob() {}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup)
{
  if (!mIsMCarlo) {
    bool changed(true);
    if (hltConfig_.init(iRun,iSetup,processName_,changed) and hltPrescale_.init(iRun, iSetup, processName_, changed)) {
      if (changed) {
        cout<<"New trigger menu found !!!"<<endl;
        triggerIndex_.clear();
        const unsigned int n(hltConfig_.size());
        // We select the triggers that are studied later on
        for (unsigned itrig=0; itrig<triggerNames_.size(); ++itrig) {
          auto &trgName = triggerNames_[itrig];
          auto trgIdx = hltConfig_.triggerIndex(trgName); 
          cout<<triggerNames_[itrig]<<" "<<trgIdx<<" ";
          if (trgIdx >= n) cout<<"does not exist in the current menu"<<endl;
          else {
            cout<<"exists"<<endl;
            triggerIndex_.push_back(trgIdx);
            goodTriggerNames_.push_back(trgName);
            mTriggerNamesHisto->Fill(trgName.c_str(),1);
          }
        }
        if (mPrintTriggerMenu) {
          cout<<"Available TriggerNames are:"<<endl;
          hltConfig_.dump("Triggers");
          cout<<"From these we selected the following:"<<endl;
          for (auto itrig=0u; itrig<goodTriggerNames_.size(); ++itrig)
            cout<<itrig<<" "<<goodTriggerNames_[itrig]<<" "<<triggerIndex_[itrig]<<endl;
        }
      }
    } else {
      cout<<"ProcessedTreeProducerBTag::analyze: config extraction failure with process name "<< processName_ <<endl;
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::analyze(edm::Event const& event, edm::EventSetup const& iSetup)
{
  vector<QCDPFJet>      mPFJetsCHS;
  QCDEventHdr mEvtHdr;
  QCDMET mPFMet_t1, mPFMet_t0pc, mPFMet_t0pct1;

  //-------------- Basic Event Info ------------------------------
  mEvtHdr.setRun(event.id().run());
  mEvtHdr.setEvt(event.id().event());
  mEvtHdr.setLumi(event.luminosityBlock());
  mEvtHdr.setBunch(event.bunchCrossing());
  //-------------- Beam Spot --------------------------------------
  Handle<reco::BeamSpot> beamSpot;
  event.getByToken(mBeamSpot,beamSpot);
  if (beamSpot.isValid()) mEvtHdr.setBS(beamSpot->x0(),beamSpot->y0(),beamSpot->z0());
  else mEvtHdr.setBS(-999,-999,-999);

  //-------------- HCAL Noise Summary -----------------------------
  if (mIsMCarlo) {
    mEvtHdr.setHCALNoise(true);
    mEvtHdr.setHCALNoiseNoMinZ(true);
  } else { 
    Handle<bool> noiseSummary;
    Handle<bool> noiseSummary_NoMinZ;

    event.getByToken(mHBHENoiseFilterResultLabel, noiseSummary);
    mEvtHdr.setHCALNoise(*noiseSummary);

    event.getByToken(mHBHENoiseFilterResultNoMinZLabel, noiseSummary_NoMinZ);
    mEvtHdr.setHCALNoiseNoMinZ(*noiseSummary_NoMinZ);
  }
  //-------------- Trigger Info -----------------------------------
  if (!mIsMCarlo) {
    vector<int> L1Prescales,HLTPrescales,Fired;
    vector<vector<LorentzVector> > mL1Objects, mHLTObjects;
    event.getByToken(triggerResultsTag_,triggerResultsHandle_);
    if (!triggerResultsHandle_.isValid()) {
      cout << "ProcessedTreeProducerBTag::analyze: Error in getting TriggerResults product from Event!" << endl;
      return;
    }
    event.getByToken(triggerEventTag_,triggerEventHandle_);
    if (!triggerEventHandle_.isValid()) {
      cout << "ProcessedTreeProducerBTag::analyze: Error in getting TriggerEvent product from Event!" << endl;
      return;
    }
    // sanity check
    assert(triggerResultsHandle_->size() == hltConfig_.size());
    //------ loop over triggers selected and available ---------
    for (unsigned itrig=0;itrig<goodTriggerNames_.size();itrig++) {
      auto &trgName = goodTriggerNames_[itrig];
      vector<LorentzVector> vvL1,vvHLT;
      bool accept = triggerResultsHandle_->accept(triggerIndex_[itrig]);
      
      std::pair<std::vector<std::pair<std::string,int> >,int> detailedPrescaleInfo = hltPrescale_.prescaleValuesInDetail(event, iSetup, trgName);
      int preHLT = detailedPrescaleInfo.second;

      // save l1 prescale values in standalone vector
      std::vector <int> l1prescalevals;
      for( size_t varind = 0; varind < detailedPrescaleInfo.first.size(); varind++ )
        l1prescalevals.push_back(detailedPrescaleInfo.first.at(varind).second);
     
      //find and save minimum l1 prescale of any ORed L1 that seeds the HLT
      std::vector<int>::iterator result = std::min_element(std::begin(l1prescalevals), std::end(l1prescalevals));
      size_t minind = std::distance(std::begin(l1prescalevals), result);
      // sometimes there are no L1s associated with a HLT. In that case, this branch stores -1 for the l1prescale
      int preL1 = minind < l1prescalevals.size() ? l1prescalevals.at(minind) : -1 ;//commented for 76X
      
      ///end in detail
      int tmpFired(0);
      if (accept) {
        mTriggerPassHisto->Fill(trgName.c_str(),1);
        tmpFired = 1;
      }

      //--------- modules on this trigger path--------------
      const vector<string>& moduleLabels(hltConfig_.moduleLabels(triggerIndex_[itrig]));
      const unsigned int moduleIndex(triggerResultsHandle_->index(triggerIndex_[itrig]));
      bool foundL1(false);
      for(unsigned int j=0; j<=moduleIndex; ++j) {
        const string& moduleLabel(moduleLabels[j]);
        const string  moduleType(hltConfig_.moduleType(moduleLabel));
        //--------check whether the module is packed up in TriggerEvent product
        const unsigned int filterIndex(triggerEventHandle_->filterIndex(InputTag(moduleLabel,"",processName_)));
        if (filterIndex<triggerEventHandle_->sizeFilters()) {
          const Vids& VIDS (triggerEventHandle_->filterIds(filterIndex));
          const Keys& KEYS(triggerEventHandle_->filterKeys(filterIndex));
          const size_type nI(VIDS.size());
          const size_type nK(KEYS.size());
          assert(nI==nK);
          const size_type n(max(nI,nK));
          const TriggerObjectCollection& TOC(triggerEventHandle_->getObjects());
          if (foundL1) {
            for(size_type i=0; i!=n; ++i) {
              const TriggerObject& TO(TOC[KEYS[i]]);
              TLorentzVector P4;
              P4.SetPtEtaPhiM(TO.pt(),TO.eta(),TO.phi(),TO.mass());
              LorentzVector qcdhltobj(P4.Px(),P4.Py(),P4.Pz(),P4.E());
              vvHLT.push_back(qcdhltobj);
            }
          } else {
            for(size_type i=0; i!=n; ++i) {
              const TriggerObject& TO(TOC[KEYS[i]]);
              TLorentzVector P4;
              P4.SetPtEtaPhiM(TO.pt(),TO.eta(),TO.phi(),TO.mass());
              LorentzVector qcdl1obj(P4.Px(),P4.Py(),P4.Pz(),P4.E());
              vvL1.push_back(qcdl1obj);
            }
            foundL1 = true;
          }
        }
      }// loop over modules
      Fired.push_back(tmpFired);
      L1Prescales.push_back(preL1);
      HLTPrescales.push_back(preHLT);
      mL1Objects.push_back(vvL1);
      mHLTObjects.push_back(vvHLT);
    }// loop over trigger names
    mEvent->setTrigDecision(Fired);
    mEvent->setPrescales(L1Prescales,HLTPrescales);
    mEvent->setL1Obj(mL1Objects);
    mEvent->setHLTObj(mHLTObjects);
  } // !isMCarlo

  //-------------- Vertex Info -----------------------------------
  Handle<reco::VertexCollection> recVtxs;
  event.getByToken(mOfflineVertices,recVtxs);
  //------------- reject events without reco vertices ------------
  int VtxGood(0);
  bool isPVgood(false);
  float PVx(0),PVy(0),PVz(0),PVndof(0);
  for (VertexCollection::const_iterator i_vtx = recVtxs->begin(); i_vtx != recVtxs->end(); i_vtx++) {
    int index = i_vtx-recVtxs->begin();
    if (index == 0) {
      PVx    = i_vtx->x();
      PVy    = i_vtx->y();
      PVz    = i_vtx->z();
      PVndof = i_vtx->ndof();
    }
    if (!(i_vtx->isFake()) and i_vtx->ndof() >= mGoodVtxNdof and fabs(i_vtx->z()) <= mGoodVtxZ) {
      if (index==0) isPVgood = true;
      VtxGood++;
    }
  }
  mEvtHdr.setVertices(recVtxs->size(),VtxGood);
  mEvtHdr.setPV(isPVgood,PVndof,PVx,PVy,PVz);

  //-------------- Rho ------------------------------------------------
  Handle<double> rhoCalo;
  event.getByToken(mSrcCaloRho,rhoCalo);
  Handle<double> rhoPF;
  event.getByToken(mSrcPFRho,rhoPF);
  mEvtHdr.setRho(*rhoCalo,*rhoPF);

  //-------------- Generator (incl. simulated PU) Info ----------------
  if (mIsMCarlo and mUseGenInfo) {
    Handle<GenEventInfoProduct> hEventInfo;
    Handle<std::vector<PileupSummaryInfo> > PupInfo;
    event.getByToken(mEventInfo, hEventInfo);
    if (hEventInfo->hasBinningValues()) mEvtHdr.setPthat(hEventInfo->binningValues()[0]);
    else mEvtHdr.setPthat(0);

    mEvtHdr.setWeight(hEventInfo->weight());
    event.getByToken(mSrcPU, PupInfo);
    int nbx = PupInfo->size();
    int ootpuEarly(0),ootpuLate(0),intpu(0);
    float Tnpv = 0.; // for computing pileup weight factor for the event

    for (auto PUI = PupInfo->begin(); PUI != PupInfo->end(); ++PUI) { // PileupSummaryInfo
      if (PUI->getBunchCrossing() < 0) {
        ootpuEarly += PUI->getPU_NumInteractions();      
      } else if (PUI->getBunchCrossing() > 0) {
        ootpuLate += PUI->getPU_NumInteractions();
      } else { // There should be only one of this kind
        intpu += PUI->getPU_NumInteractions();
        Tnpv += PUI->getTrueNumInteractions();
      }
    }
    mEvtHdr.setPU(nbx,ootpuEarly,ootpuLate,intpu);
    mEvtHdr.setTrPu(Tnpv);
  } else {
    mEvtHdr.setPthat(0);
    mEvtHdr.setWeight(0);
    mEvtHdr.setPU(0,0,0,0);
    mEvtHdr.setTrPu(0);
  }
  mEvent->setEvtHdr(mEvtHdr);

  //---------------- GenJets ------------------------------------------
  Handle<GenJetCollection>  genjets;
  vector<int> genFlavour;
  vector<int> genFlavourPhys;
  vector<int> genFlavourHadr;
  if (mIsMCarlo) {
    vector<float> GenFlavour;
    vector<float> GenHadronFlavour;
    vector<float> GenPartonFlavourPhysicsDef;
    vector<LorentzVector> mGenJets;
    event.getByToken(mGenJetsName,genjets);
    edm::Handle<reco::JetFlavourInfoMatchingCollection> theJetFlavourInfos;
    event.getByToken(jetFlavourInfosToken_, theJetFlavourInfos );
    auto j = theJetFlavourInfos->begin();
    edm::Handle<reco::JetFlavourInfoMatchingCollection> theJetFlavourInfosPhysicsDef;
    event.getByToken(jetFlavourInfosTokenPhysicsDef_, theJetFlavourInfosPhysicsDef );
    auto k = theJetFlavourInfosPhysicsDef->begin();
    for (auto i_gen = genjets->begin(); i_gen != genjets->end() and j != theJetFlavourInfos->end() and k != theJetFlavourInfosPhysicsDef->end(); ++i_gen, ++j, ++k) {
      int FlavourGen = 0;
      reco::JetFlavourInfo aInfo = j->second;
      if (mUseLegacyTag)FlavourGen = getMatchedPartonGen(event,i_gen);
      else FlavourGen = aInfo.getPartonFlavour();
      int FlavourGenHadron = aInfo.getHadronFlavour();
      reco::JetFlavourInfo bInfo = k->second;
      int FlavourGenPhysicsDef = bInfo.getPartonFlavour();

      genFlavour.push_back(FlavourGen);
      genFlavourPhys.push_back(FlavourGenPhysicsDef);
      genFlavourHadr.push_back(FlavourGenHadron);
      if (i_gen->pt() > mMinGenPt && fabs(i_gen->y()) < mMaxY) {
        mGenJets.push_back(i_gen->p4());
        GenFlavour.push_back(FlavourGen);
        GenPartonFlavourPhysicsDef.push_back(FlavourGenPhysicsDef);
        GenHadronFlavour.push_back(FlavourGenHadron);
      }
    }
    mEvent->setGenJets(mGenJets);
    mEvent->setGenFlavour(GenFlavour);
    mEvent->setGenHadronFlavour(GenHadronFlavour);
    mEvent->setGenFlavourPhysicsDef(GenPartonFlavourPhysicsDef);
  }
  
  //---------------- Jets ---------------------------------------------
  // -------- CHS Uncertainty part ----------------//
  edm::ESHandle<JetCorrectorParametersCollection> PFJetCorParCollCHS;
  if (mPFPayloadNameCHS != "" and !isPFJecUncSetCHS_){
    iSetup.get<JetCorrectionsRecord>().get(mPFPayloadNameCHS,PFJetCorParCollCHS);
    JetCorrectorParameters const& PFJetCorParCHS = (*PFJetCorParCollCHS)["Uncertainty"];
    mPFUncCHS = new JetCorrectionUncertainty(PFJetCorParCHS);
    if (mPFJECUncSrcCHS != "") {
      for(unsigned isrc=0;isrc<mPFJECUncSrcNames.size();isrc++) {
        JetCorrectorParameters *parchs = new JetCorrectorParameters(mPFJECUncSrcCHS,mPFJECUncSrcNames[isrc]);
        JetCorrectionUncertainty *tmpUncCHS = new JetCorrectionUncertainty(*parchs);
        mPFUncSrcCHS.push_back(tmpUncCHS);
      } // for(unsigned isrc=0;isrc<mPFJECUncSrcNames.size();isrc++)
    } // if (mPFJECUncSrcCHS != "")
    isPFJecUncSetCHS_ = true;
  } // if (mPFPayloadNameCHS != "" && !isPFJecUncSetCHS_)


  //----------- PFJets  CHS part -------------------------
  edm::Handle<edm::View<pat::Jet> > patjetschs;
  event.getByToken(mPFJetsNameCHS,patjetschs);

  // A store for track-vertex association
  map<reco::TrackRef,int> trk2vtx;
  //---- loop over all vertices ----------------------------
  for(unsigned ivtx = 0; ivtx < recVtxs->size(); ivtx++) {
    //---- loop over the tracks associated with the vertex ---
    if (!((*recVtxs)[ivtx].isFake()) && (*recVtxs)[ivtx].ndof() >= mGoodVtxNdof && fabs((*recVtxs)[ivtx].z()) <= mGoodVtxZ) {
      for (auto i_vtxTrk = (*recVtxs)[ivtx].tracks_begin(); i_vtxTrk != (*recVtxs)[ivtx].tracks_end(); ++i_vtxTrk) {
        //---- match the chsjet track to the track from the vertex ----
        reco::TrackRef trkRef(i_vtxTrk->castTo<reco::TrackRef>());
        trk2vtx[trkRef] = ivtx;
      }
    }
  }
    
  for (auto i_pfjetchs=patjetschs->begin(); i_pfjetchs!=patjetschs->end(); ++i_pfjetchs) { 
    if (!i_pfjetchs->isPFJet()) continue;

    //---- preselection -----------------
    int jetNo = i_pfjetchs-patjetschs->begin();
    if (fabs(i_pfjetchs->y()) > mMaxY or (i_pfjetchs->pt() < (jetNo<3 ? mMinPFPtThirdJet : mMinPFPt))) continue;
    
    QCDPFJet qcdpfjetchs;
    double scaleCHS = 1./i_pfjetchs->jecFactor(0); // --- the value of the JEC factor
          
    //---- vertex association -----------
    float sumTrkPtCHS(0.0);
    //---- Practical parameters for CHS jets
    float sumTrkPtBetaPrimeCHS(0.0),sumTrkPtBetaCHS(0.0),sumTrkPtBetaStarCHS(0.0);
    
    // Dunno how useful these are in chs jets...
    int mpuTrk(0), mlvTrk(0); // # of pile-up tracks & lead-vertex tracks ## Juska
    int mjtTrk(0); // multiplicity of _all_ tracks in jet (also vtx-unassociated!) ## Juska
  
    auto PFCHSCandidates = i_pfjetchs->getPFConstituents();
    //---- loop over the tracks of the jet (take the track associated to jet constituent) ----
    if (trk2vtx.size()>0) {
      vector<reco::TrackRef> usedtrk;
      for (auto &cand : PFCHSCandidates) {
        auto trkRef = cand->trackRef();
        if (trkRef.isNonnull() and trkRef.isAvailable()) { 
          usedtrk.push_back(trkRef);
          auto trkPt = trkRef->pt();
          auto trkPdgId = abs(cand->pdgId());
          if (trkPdgId>10 and trkPdgId<20) { // Electrons and muons
            // Same logic as in PFCandidate::bestTrack(): for electrons gsfTracks are better
            if (trkPdgId==11) { 
              auto elTrkRef = cand->gsfTrackRef();
              if (elTrkRef.isNonnull() and elTrkRef.isAvailable()) trkPt = elTrkRef->pt();
            }
          }
          sumTrkPtCHS += trkPt;
          mjtTrk++; //Juska

          // Search for the vertex associated with this track
          auto vtxit = trk2vtx.find(trkRef);
          if (vtxit!=trk2vtx.end()) {
            int currvtx = vtxit->second;
            if (currvtx == 0) {
              sumTrkPtBetaCHS += trkPt;
              mlvTrk++; //Juska
            } else {
              sumTrkPtBetaStarCHS += trkPt;
              mpuTrk++; //Juska
            }
          } // if: track has vertex
        } // if: valid track
      } // for: all chs jet particles

      // A routine for hunting the betastar removed by chs (betaprime)
      auto vTrksCHS(i_pfjetchs->associatedTracks());
      for (auto i_trkchs = vTrksCHS.begin(); i_trkchs != vTrksCHS.end(); i_trkchs++) {
        // Skip tracks associated with chs jets
        if (std::find(usedtrk.begin(),usedtrk.end(),*i_trkchs)==usedtrk.end()) {
          // Take tracks associated with a vertex other than zero
          auto vtxit = trk2vtx.find(*i_trkchs);
          if (vtxit!=trk2vtx.end() and vtxit->second != 0)
              sumTrkPtBetaPrimeCHS += (*i_trkchs)->pt();
        }
      } // for: all tracks (non-chs included)
    } // if: tracks associated with vertices > 0
    
    qcdpfjetchs.setBeta(sumTrkPtCHS ? sumTrkPtBetaCHS/sumTrkPtCHS : 0.0);
    qcdpfjetchs.setBetaStar(sumTrkPtCHS ? sumTrkPtBetaStarCHS/sumTrkPtCHS : 0.0);
    qcdpfjetchs.setBetaPrime(sumTrkPtCHS ? sumTrkPtBetaPrimeCHS/sumTrkPtCHS : 0.0);
  
    //---- jec uncertainty --------------
    double uncCHS(0.0);
    vector<float> uncSrcCHS(0);
    if (mPFPayloadNameCHS != "") {
      mPFUncCHS->setJetEta(i_pfjetchs->eta());
      mPFUncCHS->setJetPt(i_pfjetchs->pt());
      uncCHS = mPFUncCHS->getUncertainty(true);
    } // if (mPFPayloadNameCHS != "")
    if (mPFJECUncSrcCHS != "") {
      for(unsigned isrc=0;isrc<mPFJECUncSrcNames.size();isrc++) {
        mPFUncSrcCHS[isrc]->setJetEta(i_pfjetchs->eta());
        mPFUncSrcCHS[isrc]->setJetPt(i_pfjetchs->pt());
        float unc1 = mPFUncSrcCHS[isrc]->getUncertainty(true);
        uncSrcCHS.push_back(unc1);
      } // for(unsigned isrc=0;isrc<mPFJECUncSrcNames.size();isrc++)
    } // if (mPFJECUncSrc != "")
    
    qcdpfjetchs.setP4(i_pfjetchs->p4());
    qcdpfjetchs.setCor(scaleCHS);
    qcdpfjetchs.setUnc(uncCHS);
    qcdpfjetchs.setUncSrc(uncSrcCHS);
    qcdpfjetchs.setArea(i_pfjetchs->jetArea());
    
    double chf   = i_pfjetchs->chargedHadronEnergyFraction();
    double nhf   = i_pfjetchs->neutralHadronEnergyFraction();// + i_pfjetchs->HFHadronEnergyFraction();
    double nemf  = i_pfjetchs->neutralEmEnergyFraction(); // equals to deprecated phf but has HF info too
    double cemf  = i_pfjetchs->chargedEmEnergyFraction(); // equals to deprecated elf
    double muf   = i_pfjetchs->muonEnergyFraction();
    double hf_hf = i_pfjetchs->HFHadronEnergyFraction();
    double hf_phf= i_pfjetchs->HFEMEnergyFraction();
    int hf_hm    = i_pfjetchs->HFHadronMultiplicity();
    int hf_phm   = i_pfjetchs->HFEMMultiplicity();
    int chm      = i_pfjetchs->chargedHadronMultiplicity();
    int nhm      = i_pfjetchs->neutralHadronMultiplicity();
    int phm      = i_pfjetchs->photonMultiplicity();
    int elm      = i_pfjetchs->electronMultiplicity();
    int mum      = i_pfjetchs->muonMultiplicity();
    int npr      = i_pfjetchs->chargedMultiplicity() + i_pfjetchs->neutralMultiplicity();
    
    float abseta = fabs(i_pfjetchs->eta());
    int nm       = i_pfjetchs->neutralMultiplicity();
    int cm       = i_pfjetchs->chargedMultiplicity();
     
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID
    bool looseID, tightID;
    if (abseta <= 2.7) {
       looseID = nhf<0.99 and nemf<0.99 and npr>1 and (abseta>2.4 || (chf>0 and chm>0 and cemf <0.99));
       tightID = looseID and nhf<0.90 and nemf<0.90; 
    } else if (abseta <= 3.0) {
       looseID = nemf<0.90 and nm>2;
       tightID = looseID; 
    } else {
       looseID = nemf<0.90 and nm>10; 
       tightID = looseID; 
    }

    qcdpfjetchs.setLooseID(looseID);
    qcdpfjetchs.setTightID(tightID);
    qcdpfjetchs.setFrac(chf,nhf,nemf,cemf,muf);
    qcdpfjetchs.setMulti(npr,chm,nhm,phm,elm,mum,cm);
    qcdpfjetchs.setHFFrac(hf_hf,hf_phf);
    qcdpfjetchs.setHFMulti(hf_hm,hf_phm);
    
    double hof = i_pfjetchs->hoEnergyFraction(); // Juska
    qcdpfjetchs.setVtxInfo(mpuTrk,mlvTrk,mjtTrk);
    qcdpfjetchs.setHO(hof);
   
    double CSVpfPositive = i_pfjetchs->bDiscriminator("pfPositiveCombinedSecondaryVertexV2BJetTags");
    double CSVpfNegative = i_pfjetchs->bDiscriminator("pfNegativeCombinedSecondaryVertexV2BJetTags");

    double pfBoostedDoubleSecondaryVertex = i_pfjetchs->bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags");
    //C taggers
    double pfCombinedCvsL = i_pfjetchs->bDiscriminator("pfCombinedCvsLJetTags");
    double pfCombinedCvsB = i_pfjetchs->bDiscriminator("pfCombinedCvsBJetTags");

    float DeepCSVb   = i_pfjetchs->bDiscriminator("deepFlavourJetTags:probb");
    float DeepCSVc   = i_pfjetchs->bDiscriminator("deepFlavourJetTags:probc");
    float DeepCSVl   = i_pfjetchs->bDiscriminator("deepFlavourJetTags:probudsg");
    float DeepCSVbb  = i_pfjetchs->bDiscriminator("deepFlavourJetTags:probbb");
    float DeepCSVcc  = i_pfjetchs->bDiscriminator("deepFlavourJetTags:probcc");
    float DeepCSVbN  = i_pfjetchs->bDiscriminator("negativeDeepFlavourJetTags:probb");
    float DeepCSVcN  = i_pfjetchs->bDiscriminator("negativeDeepFlavourJetTags:probc");
    float DeepCSVlN  = i_pfjetchs->bDiscriminator("negativeDeepFlavourJetTags:probudsg");
    float DeepCSVbbN = i_pfjetchs->bDiscriminator("negativeDeepFlavourJetTags:probbb");
    float DeepCSVccN = i_pfjetchs->bDiscriminator("negativeDeepFlavourJetTags:probcc");
    float DeepCSVbP  = i_pfjetchs->bDiscriminator("positiveDeepFlavourJetTags:probb");
    float DeepCSVcP  = i_pfjetchs->bDiscriminator("positiveDeepFlavourJetTags:probc");
    float DeepCSVlP  = i_pfjetchs->bDiscriminator("positiveDeepFlavourJetTags:probudsg");
    float DeepCSVbbP = i_pfjetchs->bDiscriminator("positiveDeepFlavourJetTags:probbb");
    float DeepCSVccP = i_pfjetchs->bDiscriminator("positiveDeepFlavourJetTags:probcc");

    //the three recommended                                                                                                                                        
    double pfJetProbabilityBJetTags=i_pfjetchs->bDiscriminator("pfJetProbabilityBJetTags");
    double pfCombinedInclusiveSecondaryVertexV2BJetTags= i_pfjetchs->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
    double pfCombinedMVAV2BJetTags=i_pfjetchs->bDiscriminator("pfCombinedMVAV2BJetTags");
    
    float partonFlavour=0;
    float partonFlavourPhysicsDef=0;
    float hadronFlavour=0;
    if (mIsMCarlo and mUseGenInfo) {
      partonFlavour = i_pfjetchs->partonFlavour();
      hadronFlavour = i_pfjetchs->hadronFlavour();
      if (i_pfjetchs->genParton() != NULL) partonFlavourPhysicsDef = i_pfjetchs->genParton()->pdgId();
    }
    
    float QGTagger=-100; 
    if (mAK4) QGTagger = i_pfjetchs->userFloat("QGTaggerAK4PFCHS:qgLikelihood");
    
    //Filling B-tag infos
    qcdpfjetchs.setPositiveNegativeCSV(CSVpfPositive,CSVpfNegative);
    qcdpfjetchs.setTagRecommended(pfJetProbabilityBJetTags,pfCombinedInclusiveSecondaryVertexV2BJetTags,pfCombinedMVAV2BJetTags);
    qcdpfjetchs.setDeepCSV(DeepCSVb, DeepCSVc, DeepCSVl, DeepCSVbb, DeepCSVcc,
                           DeepCSVbN, DeepCSVcN, DeepCSVlN, DeepCSVbbN, DeepCSVccN,
                           DeepCSVbP, DeepCSVcP, DeepCSVlP, DeepCSVbbP, DeepCSVccP);
    
    qcdpfjetchs.setQGTagger(QGTagger);	 
    qcdpfjetchs.setBoosted(pfBoostedDoubleSecondaryVertex);
    qcdpfjetchs.setCTagger(pfCombinedCvsL,pfCombinedCvsB);
    
    float pileupJetId = -999;
    if (i_pfjetchs->hasUserFloat(mPFJetPUIDCHS)) pileupJetId = i_pfjetchs->userFloat(mPFJetPUIDCHS);
    qcdpfjetchs.SetPUJetId(pileupJetId);
    
    if (mIsMCarlo and genjets->size()>0) {
      float ratiouplim = (i_pfjetchs->pt()>20 ? 1.5 : 2.5);
      float ratiololim = (i_pfjetchs->pt()>20 ? 0.4 : 0.25);
      // Find approximately the closest pt and then iterate up and down
      // If a good match is found, iteration stops
      float rmin(999);
      int imin = -1;
      int dwn = genorder(i_pfjetchs->pt(),genjets,0,genjets->size()-1);
      int up = dwn+1;
      int limit = genjets->size();
      while (dwn>=0 or up<limit) {
        if (dwn>=0) {
          if ((genjets->at(dwn).pt())/(i_pfjetchs->pt())<ratiouplim) {
            float deltaR = reco::deltaR(*i_pfjetchs,genjets->at(dwn));
            if (deltaR < rmin) {
              imin = dwn;
              rmin = deltaR;
              if (rmin<0.2) break;
            }
            --dwn;
          } else {
            dwn = -1;
          }
        }
        if (up<limit) {
          if ((genjets->at(up).pt())/(i_pfjetchs->pt())>ratiololim) {
            float deltaR = reco::deltaR(*i_pfjetchs,genjets->at(up));
            if (deltaR < rmin) {
              imin = up;
              rmin = deltaR;
              if (rmin<0.2) break;
            }
            ++up;
          } else {
            up = limit;
          }
        }
      }
      if (imin!=-1 and rmin<0.4) {
        qcdpfjetchs.setGen(genjets->at(imin).p4(),rmin);
        if (partonFlavour==0) partonFlavour = genFlavour[imin];
        if (partonFlavourPhysicsDef==0) partonFlavourPhysicsDef = genFlavourPhys[imin];
        if (hadronFlavour==0) hadronFlavour = genFlavourHadr[imin];
      } else {
        qcdpfjetchs.setGen(LorentzVector(0,0,0,0),0);
      }
      qcdpfjetchs.setFlavour(partonFlavour,hadronFlavour,partonFlavourPhysicsDef);
    } else {
      LorentzVector tmpP4(0.0,0.0,0.0,0.0);
      qcdpfjetchs.setGen(tmpP4,0);
    }
    mPFJetsCHS.push_back(qcdpfjetchs);
  } // for: chs Jets
  sort(mPFJetsCHS.begin(),mPFJetsCHS.end(),sort_pfjets);
  mEvent->setPFJetsCHS(mPFJetsCHS); // -- later substitute chs jets
  
  //---------------- met ---------------------------------------------
  Handle<pat::METCollection> pfmet_t1, pfmet_t0pc, pfmet_t0pct1;
  event.getByToken(mPFMETt1, pfmet_t1);
  event.getByToken(mPFMETt0pc, pfmet_t0pc);
  event.getByToken(mPFMETt0pct1, pfmet_t0pct1);
  const pat::MET &met_t1 = pfmet_t1->front();
  mPFMet_t1.setVar(met_t1.et(),met_t1.sumEt(),met_t1.phi());
  const pat::MET &met_t0pc = pfmet_t0pc->front();
  mPFMet_t0pc.setVar(met_t0pc.et(),met_t0pc.sumEt(),met_t0pc.phi());
  const pat::MET &met_t0pct1 = pfmet_t0pct1->front();
  mPFMet_t0pct1.setVar(met_t0pct1.et(),met_t0pct1.sumEt(),met_t0pct1.phi());
  mEvent->setPFMET(mPFMet_t1,mPFMet_t0pc,mPFMet_t0pct1);

  //-------------- fill the tree -------------------------------------
  if ((mEvent->nPFJetsCHS() >= (unsigned)mMinNPFJets) ) {
    if ((mEvent->pfchsmjjcor(0) >= mMinJJMass) ) {
      mTree->Fill();
    }
  }
}


/////////////// Matching Flavour (deprecated) ///////////////////////////////

int ProcessedTreeProducerBTag::getMatchedPartonGen(edm::Event const& event,GenJetCollection::const_iterator i_gen)
{
  int jetFlavour=-100;
  bool switchB=0;
  bool switchC=0;

  edm::Handle<reco::GenParticleCollection> genParticles;
  event.getByToken(mGenParticles, genParticles);

  double DeltaRmin=0.3;
  for (size_t i = 0; i < genParticles->size (); ++i) {
    const GenParticle & genIt = (*genParticles)[i];
    int pdgId = genIt.pdgId();
    double DeltaR=deltaR(genIt.p4().eta(),genIt.p4().phi(),i_gen->eta(),i_gen->phi());
    if (DeltaR < DeltaRmin ){
      DeltaRmin=DeltaR;
      if(abs(pdgId)==5){ jetFlavour=5; switchB=true;}
      if(abs(pdgId)==4){ jetFlavour=4; switchC=true;}
      if(abs(pdgId)<=3 && abs(pdgId)>=1){ jetFlavour=1; }
      if(abs(pdgId)==21){ jetFlavour=21; }
    }      
    if (switchB) {jetFlavour=5;}
    if (switchC && !switchB) {jetFlavour=4;}
  }
  return jetFlavour;
}

int ProcessedTreeProducerBTag::getMatchedHadronGen(edm::Event const& event,GenJetCollection::const_iterator i_gen)
{
  int jetFlavour=-100;

  edm::Handle<reco::GenParticleCollection> genParticles;
  event.getByToken(mGenParticles, genParticles);

  for (size_t i = 0; i < genParticles->size (); ++i) {
    const GenParticle & genIt = (*genParticles)[i];    
    int aid = abs(genIt.pdgId());
    if (aid/100 == 5 || aid/1000==5) {
      // 2J+1 == 1 (mesons) or 2 (baryons)
      if (aid%10 == 1 || aid%10 == 2) {
        // No B decaying to B
        if (aid != 5222 && aid != 5112 && aid != 5212 && aid != 5322) {
          double DeltaR=deltaR(genIt.p4().eta(),genIt.p4().phi(),i_gen->eta(),i_gen->phi());
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

//////////////////////////////////////////////////////////////////////////////////////////
ProcessedTreeProducerBTag::~ProcessedTreeProducerBTag() {}

DEFINE_FWK_MODULE(ProcessedTreeProducerBTag);
