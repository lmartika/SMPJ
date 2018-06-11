#include "SMPJ/AnalysisFW/plugins/ProcessedTreeProducerBTag.h"

ProcessedTreeProducerBTag::ProcessedTreeProducerBTag(edm::ParameterSet const& cfg):
  mSaveWeights(                                                                                      cfg.getParameter<bool>("saveWeights")),
  mAK4(                                                                                     cfg.getUntrackedParameter<bool>("AK4",false)),
  mZB(                                                                                      cfg.getUntrackedParameter<bool>("ZB",false)),
  mPrintTriggerMenu(                                                                        cfg.getUntrackedParameter<bool>("printTriggerMenu",false)),
  mIsPFJecUncSet(                                                                                                           false),
  // Cut params
  mGoodVtxNdof(                                                                                    cfg.getParameter<double>("goodVtxNdof")),
  mGoodVtxZ(                                                                                       cfg.getParameter<double>("goodVtxZ")),
  mMinNPFJets(                                                                               cfg.getParameter<unsigned int>("minNPFJets")),
  mMinPFPt(                                                                                        cfg.getParameter<double>("minPFPt")),
  mMinPFPtThirdJet(                                                                                cfg.getParameter<double>("minPFPtThirdJet")),
  mMinGenPt(                                                                              cfg.getUntrackedParameter<double>("minGenPt",20)),
  mMaxEta(                                                                                         cfg.getParameter<double>("maxEta")),
  // Misc
  mPFPayloadName(                                                                             cfg.getParameter<std::string>("PFPayloadName")),
  mRunYear(                                                                          cfg.getUntrackedParameter<std::string>("runYear","2016")),
  mPFJetPUID(                                                                                 cfg.getParameter<std::string>("pfchsjetpuid")),
  mPFJECUncSrc(                                                                      cfg.getUntrackedParameter<std::string>("jecUncSrc","")),
  mPFJECUncSrcNames(                                                            cfg.getParameter<std::vector<std::string> >("jecUncSrcNames")),
  mOfflineVertices(mayConsume<reco::VertexCollection>(                                      cfg.getParameter<edm::InputTag>("offlineVertices"))),
  mBeamSpot(mayConsume<reco::BeamSpot>(                                                     cfg.getParameter<edm::InputTag>("beamSpot"))),
  mPFJetsName(consumes<edm::View<pat::Jet>>(                                                cfg.getParameter<edm::InputTag>("pfjetschs"))),
  // Rho
  mSrcCaloRho(mayConsume<double>(                                                           cfg.getParameter<edm::InputTag>("srcCaloRho"))),
  mSrcPFRho(mayConsume<double>(                                                             cfg.getParameter<edm::InputTag>("srcPFRho"))),
  // MET
  mPFMETt1(mayConsume<pat::METCollection>(                                                  cfg.getParameter<edm::InputTag>("pfmetT1"))),
  // GEN
  mIsMCarlo(                                                                                cfg.getUntrackedParameter<bool>("isMCarlo",false)),
  mUseGenInfo(                                                                              cfg.getUntrackedParameter<bool>("useGenInfo",false)),
  mMCType(                                                                                   cfg.getUntrackedParameter<int>("mcType",0)), // 0 for Pythia, 1 for Herwig++
  mGenJetsName(mayConsume<GenJetCollection>(                                       cfg.getUntrackedParameter<edm::InputTag>("genjets",edm::InputTag("")))),
  mGenParticles(consumes<reco::GenParticleCollection>(                             cfg.getUntrackedParameter<edm::InputTag>("GenParticles",edm::InputTag("")))),
  mEventInfo(consumes<GenEventInfoProduct>(                                        cfg.getUntrackedParameter<edm::InputTag>("EventInfo",edm::InputTag("")))),
  mSrcPU(mayConsume<std::vector<PileupSummaryInfo> >(                              cfg.getUntrackedParameter<edm::InputTag>("srcPULabel",edm::InputTag("")))),
  mJetFlavourInfosToken(consumes<reco::JetFlavourInfoMatchingCollection>(          cfg.getUntrackedParameter<edm::InputTag>("jetFlavInfos",edm::InputTag("")))),
  mJetFlavourInfosTokenPhysicsDef(consumes<reco::JetFlavourInfoMatchingCollection>(cfg.getUntrackedParameter<edm::InputTag>("jetFlavInfosPD",edm::InputTag("")))),
  mQGLToken(consumes<                                                                   edm::ValueMap<float>>(edm::InputTag("QGTagger", "qgLikelihood"))),
  mQGAx2Token(consumes<                                                                 edm::ValueMap<float>>(edm::InputTag("QGTagger", "axis2"))),
  mQGMulToken(consumes<                                                                   edm::ValueMap<int>>(edm::InputTag("QGTagger", "mult"))),
  mQGPtDToken(consumes<                                                                 edm::ValueMap<float>>(edm::InputTag("QGTagger", "ptD"))),
  // Trigger
  mProcessName(                                                                      cfg.getUntrackedParameter<std::string>("processName","")),
  mFilterNames(                                                                 cfg.getParameter<std::vector<std::string> >("filterName")),
  mTriggerNames(                                                                cfg.getParameter<std::vector<std::string> >("triggerName")),
  mTriggerFlwNames(                                                             cfg.getParameter<std::vector<std::string> >("triggerFollow")),
  mFilterBits(mayConsume<edm::TriggerResults>(                                     cfg.getUntrackedParameter<edm::InputTag>("filterFlags",edm::InputTag("")))),
  mTriggerBits(mayConsume<edm::TriggerResults>(                                    cfg.getUntrackedParameter<edm::InputTag>("triggerResults",edm::InputTag("")))),
  mTriggerHLTObjs(mayConsume<pat::TriggerObjectStandAloneCollection>(              cfg.getUntrackedParameter<edm::InputTag>("triggerHLTObjs",edm::InputTag("")))),
  mTriggerL1Objs(mayConsume<BXVector<l1t::Jet> >(                                  cfg.getUntrackedParameter<edm::InputTag>("triggerL1Objs",edm::InputTag("")))),
  mTriggerL1HTObjs(mayConsume<BXVector<l1t::EtSum> >(                              cfg.getUntrackedParameter<edm::InputTag>("triggerL1HTObjs",edm::InputTag("")))),
  mTriggerPrescales(consumes<pat::PackedTriggerPrescales>(                                  cfg.getParameter<edm::InputTag>("prescales"))),
  mTriggerPrescalesL1Min(consumes<pat::PackedTriggerPrescales>(                             cfg.getParameter<edm::InputTag>("prescalesL1Min"))),
  mTriggerPrescalesL1Max(consumes<pat::PackedTriggerPrescales>(                             cfg.getParameter<edm::InputTag>("prescalesL1Max"))),
  mHBHENoiseFilterResultNoMinZLabel(mayConsume<bool>(                              cfg.getUntrackedParameter<edm::InputTag>("HBHENoiseFilterResultNoMinZLabel",edm::InputTag("")))),
  mCands(mayConsume<pat::PackedCandidateCollection>(                                                          edm::InputTag("packedPFCandidates"))),
  mHLTPrescale(cfg, consumesCollector(), *this)
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
  mULimCEF = 0; mULimNEF = 0; mLLimNEF = 0; mULimNHF = 0; mLLimNHF = 0;
  if (mRunYear=="2016") {
    mULimCEF = 0.99;
    mULimNEF = 1.01;
    mULimNHF = 0.98;
    mLLimNEF = 0.01;
    mLLimNHF = -1.00;
  } else if (mRunYear=="2017") {
    mULimCEF = 1.01;
    mULimNEF = 0.99;
    mULimNHF = 1.01;
    mLLimNEF = 0.02;
    mLLimNHF = 0.02;
  }
  cout << "Run year " << mRunYear << " using the following JetID limit parameter values:" << endl;
  cout << "Up cef " << mULimCEF << endl;
  cout << "Up nef " << mULimNEF << endl;
  cout << "Up nhf " << mULimNHF << endl;
  cout << "Lo nef " << mLLimNEF << endl;
  cout << "Lo nhf " << mLLimNHF << endl;
  mNewTrigs = false;
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::endJob() {}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup)
{
  bool changed(true);
  if (!(mHLTConfig.init(iRun,iSetup,mProcessName,changed) and mHLTPrescale.init(iRun, iSetup, mProcessName, changed))) {
    cout << "ProcessedTreeProducerBTag::analyze: config extraction failure with process name " << mProcessName << endl;
    return;
  }
  if (!changed) return;
  cout << "Running on global tag " << mHLTConfig.globalTag() << "!!!" << endl;
  if (mIsMCarlo) return;
  cout << "New trigger menu found!!!" << endl;

  mNewTrigs = true;
  mTriggerIndex.clear();
  mTriggerFlwIndex.clear();
  const unsigned int n = mHLTConfig.size();
  // We check out what triggers are to be actively monitored
  for (unsigned itrig=0; itrig<mTriggerNames.size(); ++itrig) {
    auto &trgName = mTriggerNames[itrig];
    auto trgIdx = mHLTConfig.triggerIndex(trgName);
    cout << "Actively monitored trigger " << mTriggerNames[itrig] << " " << trgIdx << " ";
    if (trgIdx >= n) {
      cout << "does not exist in the current menu" << endl;
      mTriggerIndex.push_back(-1);
      mTriggerNamesHisto->Fill(trgName.c_str(),0);
    } else {
      cout << "exists" <<endl;
      mTriggerIndex.push_back(trgIdx);
      mTriggerNamesHisto->Fill(trgName.c_str(),1);
    }
    mTriggerPassHisto->Fill(trgName.c_str(),0);
  }
  // These triggers are not saved in the current run, but we save the events with these for compatibility reasons.
  // Usage example: when AK4 and AK8 jets are saved into separate files, we still want the same events to be available in both.
  for (unsigned itrig=0; itrig<mTriggerFlwNames.size(); ++itrig) {
    auto &trgName = mTriggerFlwNames[itrig];
    auto trgIdx = mHLTConfig.triggerIndex(trgName);
    cout << "Passively monitored trigger " << mTriggerFlwNames[itrig] << " " << trgIdx << " ";
    if (trgIdx >= n) {
      cout << "does not exist in the current menu" << endl;
      mTriggerFlwIndex.push_back(-1);
    } else {
      cout << "exists" <<endl;
      mTriggerFlwIndex.push_back(trgIdx);
    }
  }

  if (mPrintTriggerMenu) {
    cout << "Available TriggerNames are: " << endl;
    mHLTConfig.dump("Triggers");
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::analyze(edm::Event const& event, edm::EventSetup const& iSetup)
{
  vector<QCDPFJet>      qPFJets;
  QCDEventHdr           qEvtHdr;
  QCDMET                qPFMet_t1, qPFMet_t0, qPFMet_t0t1;

  bool save_event=false;
  
  //-------------- Basic Event Info ------------------------------
  qEvtHdr.setRun(event.id().run());
  qEvtHdr.setEvt(event.id().event());
  qEvtHdr.setLumi(event.luminosityBlock());
  qEvtHdr.setBunch(event.bunchCrossing());
  
  //-------------- Beam Spot --------------------------------------
  Handle<reco::BeamSpot> beamSpot;
  event.getByToken(mBeamSpot,beamSpot);
  if (beamSpot.isValid()) qEvtHdr.setBS(beamSpot->x0(),beamSpot->y0(),beamSpot->z0());
  else qEvtHdr.setBS(-999,-999,-999);

  //-------------- HCAL Noise Summary -----------------------------
  if (mIsMCarlo) {
    qEvtHdr.setHCALNoiseNoMinZ(true);
  } else {
    Handle<bool> noiseSummary_NoMinZ;
    event.getByToken(mHBHENoiseFilterResultNoMinZLabel, noiseSummary_NoMinZ);
    qEvtHdr.setHCALNoiseNoMinZ(*noiseSummary_NoMinZ);
  }

  //-------------- Trigger Info ----------------------------------- 
  if (!mIsMCarlo) {
    vector<int>                         L1Prescales, HLTPrescales, Fired;
    vector<vector<LorentzVector> >      qL1Objs, qHLTObjs;  
    map<string, vector<LorentzVector> > vvHLT;

    edm::Handle<edm::TriggerResults>                    filterBits;
    edm::Handle<edm::TriggerResults>                    triggerBits;
    edm::Handle<pat::TriggerObjectStandAloneCollection> triggerHLTObjs;
    edm::Handle<BXVector<l1t::Jet> >                    triggerL1Objs;
    edm::Handle<BXVector<l1t::EtSum> >                  triggerL1HTObjs;
    edm::Handle<pat::PackedTriggerPrescales>            triggerPrescales;
    edm::Handle<pat::PackedTriggerPrescales>            triggerPrescalesL1Max;
    edm::Handle<pat::PackedTriggerPrescales>            triggerPrescalesL1Min;

    event.getByToken(mFilterBits,                       filterBits);
    event.getByToken(mTriggerBits,                      triggerBits);
    event.getByToken(mTriggerHLTObjs,                   triggerHLTObjs);
    event.getByToken(mTriggerL1Objs,                    triggerL1Objs);
    event.getByToken(mTriggerL1HTObjs,                  triggerL1HTObjs);
    event.getByToken(mTriggerPrescales,                 triggerPrescales);
    event.getByToken(mTriggerPrescalesL1Min,            triggerPrescalesL1Min);
    event.getByToken(mTriggerPrescalesL1Max,            triggerPrescalesL1Max);
    
    const edm::TriggerNames &names       = event.triggerNames(*triggerBits);
    const edm::TriggerNames &filterNames = event.triggerNames(*filterBits);
    if (mNewTrigs) {
      // Update the filter positions 
      mFilterIndex.clear();
      for (auto &flt : mFilterNames) {
        int fltIdx = -1;
        for (unsigned int itrig=0; itrig<filterBits->size(); ++itrig) {
          string filterName = filterNames.triggerName(itrig);
          if (filterName==flt) {
            fltIdx = itrig;
            break;
          }
        }
        mFilterIndex.push_back(fltIdx);
        cout << "MET filter " << flt;
        if (fltIdx==-1) cout << " not found! :(" << endl;
        else cout << " found! :)" << endl;
      }

      // When the triggers are changed, check that the info we got from hltConfig is good
      // Acitve filters:
      assert(mTriggerIndex.size()==mTriggerNames.size());
      for (auto itrig = 0u; itrig<mTriggerIndex.size(); ++itrig) {
        int trgIdx = mTriggerIndex[itrig];
        if (trgIdx<0) continue; // Trigger not present in the file, OK!

        string trgName1 = mTriggerNames[itrig];
        string trgName2 = names.triggerName(trgIdx);
        if (trgName1!=trgName2) cout << "Mismatch in trigger names: " << trgName1 << " " << trgName2 << endl;
      }
      cout << "Actively monitored triggers checked!" << endl;
      // Passive filters:
      assert(mTriggerFlwIndex.size()==mTriggerFlwNames.size());
      for (auto itrig = 0u; itrig<mTriggerFlwIndex.size(); ++itrig) {
        int trgFlwIdx = mTriggerFlwIndex[itrig];
        if (trgFlwIdx<0) continue; // Trigger not present in the file, OK!

        string trgName1 = mTriggerFlwNames[itrig];
        string trgName2 = names.triggerName(trgFlwIdx);
        if (trgName1!=trgName2) cout << "Mismatch in followed trigger names: " << trgName1 << " " << trgName2 << endl;
      }
      cout << "Passively monitored triggers checked!" << endl;
      mNewTrigs = false;
    }

    // Go through the filters and check that all of them are good
    assert(mFilterIndex.size()==mFilterNames.size());
    for (auto iflt = 0u; iflt<mFilterIndex.size(); ++iflt) {
      int fltIdx = mFilterIndex[iflt];
      if (fltIdx<0) continue; // If a filter was not found, we skip it 
      
      string fltName1 = mFilterNames[iflt];
      string fltName2 = filterNames.triggerName(fltIdx);
      if (fltName1!=fltName2) {
        cout << "Mismatch in filter names: " << fltName1 << " " << fltName2 << endl;
      } else if (!filterBits->accept(fltIdx)) {
        // A filter is allowed to reject the event
        return;
      }
    }

    // Trigger counts
    unsigned fire = 0;
    // Primary triggers
    for (auto itrig = 0u; itrig<mTriggerIndex.size(); ++itrig) {
      int trgIdx = mTriggerIndex[itrig]; 
      if (trgIdx<0) continue; // We skip the triggers not present (perfectly normal!)

      if (triggerBits->accept(trgIdx)) {
        const string &trgName = mTriggerNames[itrig];      
        L1Prescales.push_back(max(triggerPrescalesL1Max->getPrescaleForIndex(trgIdx),triggerPrescalesL1Min->getPrescaleForIndex(trgIdx)));
        HLTPrescales.push_back(triggerPrescales->getPrescaleForIndex(trgIdx));
        mTriggerPassHisto->Fill(trgName.c_str(),1);
        Fired.push_back(itrig);
        ++fire;
        if (vvHLT.find(trgName)==vvHLT.end()) vvHLT[trgName] = vector<LorentzVector>();
      }
    }
    // When the primary trigger has not fired, check if a "followed" secondary trigger has fired
    if (fire==0) {
      bool nofireoth = true;
      for (auto itrig = 0u; itrig<mTriggerFlwIndex.size(); ++itrig) {
        int trgIdx = mTriggerFlwIndex[itrig]; 
        if (trgIdx<0) continue; // Skip the triggers not present (perfectly normal!)

        if (triggerBits->accept(trgIdx)) {
          nofireoth = false;
          break;
        }
      }
      // If none of the actively/passively monitored triggers have fired, skip this as a redundant event
      if (nofireoth) return;
    }

    // The trigger objects are saved only when the actively monitored trigger has fired (in ZB events we pay no attention)
    if (fire>0 and !mZB) {
      // HLT sector
      regex pfjet(Form("HLT_%sPFJet([0-9]*)_v([0-9]*)",mAK4 ? "" : "AK8"));
      regex jetht("HLT_PFHT([0-9]*)_v[0-9]*");
      for (pat::TriggerObjectStandAlone obj : *triggerHLTObjs) { // note: not "const &" since we want to call unpackPathNames
        obj.unpackPathNames(names);
        vector<string> pathNamesAll  = obj.pathNames(false);
        vector<string> pathNamesLast = obj.pathNames(true);
        if (pathNamesAll.size()==0) continue; 
        
        for (unsigned hpn = 0, npn = pathNamesAll.size(); hpn < npn; ++hpn) {
          string trgName = pathNamesAll[hpn];
          if (regex_match(trgName,pfjet)) {
            TLorentzVector P4;
            P4.SetPtEtaPhiM(obj.pt(),obj.eta(),obj.phi(),obj.mass());
            vvHLT[trgName].emplace_back(P4.Px(),P4.Py(),P4.Pz(),P4.E());
          } else if (!mAK4 and regex_match(trgName,jetht)) {
            // We store JetHT stuff into AK8 tuples
            TLorentzVector P4;
            P4.SetPtEtaPhiM(obj.pt(),obj.eta(),obj.phi(),obj.mass());
            vvHLT[trgName].emplace_back(P4.Px(),P4.Py(),P4.Pz(),P4.E());
          }
        }
      }
      // HLT objects are added collectively, after we have went through all the objects
      for (auto &trg : Fired) qHLTObjs.push_back(vvHLT[mTriggerNames[trg]]); 

      // L1 sector: jets
      vector<LorentzVector> vvL1;
      for (auto obj = triggerL1Objs->begin(0); obj != triggerL1Objs->end(0); ++obj) {
        TLorentzVector P4;
        P4.SetPtEtaPhiM(obj->pt(),obj->eta(),obj->phi(),obj->mass());
        vvL1.emplace_back(P4.Px(),P4.Py(),P4.Pz(),P4.E());
      }
      qL1Objs.push_back(vvL1);

      // L1 sector: JetHT (only stored into AK8 tuples)
      if (!mAK4) {
        vector<LorentzVector> vvL1HT;
        for (auto obj = triggerL1HTObjs->begin(0); obj != triggerL1HTObjs->end(0); ++obj) {
          if (obj->getType()==l1t::EtSum::kTotalHt) {
            TLorentzVector P4;
            P4.SetPtEtaPhiM(obj->pt(),obj->eta(),obj->phi(),obj->mass());
            vvL1HT.emplace_back(P4.Px(),P4.Py(),P4.Pz(),P4.E());
          }
        }
        qL1Objs.push_back(vvL1HT);
      }
    }
    mEvent->setTrigDecision(Fired);
    mEvent->setPrescales(L1Prescales,HLTPrescales);
    mEvent->setL1Obj(qL1Objs);
    mEvent->setHLTObj(qHLTObjs);
  }
  
  //-------------- Vertex Info -----------------------------------
  Handle<reco::VertexCollection> recVtxs;
  event.getByToken(mOfflineVertices,recVtxs);
  
  //------------- reject events without reco vertices ------------
  int VtxGood(0);
  bool isPVgood(false);
  float PVx(0),PVy(0),PVz(0),PVndof(0);
  for (auto ivtx = recVtxs->begin(); ivtx != recVtxs->end(); ++ivtx) {
    int index = ivtx-recVtxs->begin();
    if (index == 0) {
      PVx    = ivtx->x();
      PVy    = ivtx->y();
      PVz    = ivtx->z();
      PVndof = ivtx->ndof();
    }
    if (!(ivtx->isFake()) and ivtx->ndof() >= mGoodVtxNdof and fabs(ivtx->z()) <= mGoodVtxZ) {
      if (index == 0) isPVgood = true;
      ++VtxGood;
    }
  }
  qEvtHdr.setVertices(recVtxs->size(),VtxGood);
  qEvtHdr.setPV(isPVgood,PVndof,PVx,PVy,PVz);
  
  //-------------- Rho ------------------------------------------------
  Handle<double> rhoCalo;
  event.getByToken(mSrcCaloRho,rhoCalo);
  Handle<double> rhoPF;
  event.getByToken(mSrcPFRho,rhoPF);
  qEvtHdr.setRho(*rhoCalo,*rhoPF);

  //-------------- Generator (incl. simulated PU) Info -------------------------------------
  if (mIsMCarlo and mUseGenInfo) {
    Handle<GenEventInfoProduct> hEventInfo;
    Handle<std::vector<PileupSummaryInfo> > PupInfo;
    event.getByToken(mEventInfo, hEventInfo);
    if (hEventInfo->hasBinningValues()) qEvtHdr.setPthat(hEventInfo->binningValues()[0]);
    else qEvtHdr.setPthat(0);
  
    qEvtHdr.setWeight(hEventInfo->weight());
    event.getByToken(mSrcPU, PupInfo);
    int nbx = PupInfo->size();
    int ootpuEarly(0),ootpuLate(0),intpu(0);
    float Tnpv = 0.; // new variable for computing pileup weight factor for the event
  
    for(auto PUI = PupInfo->begin(); PUI != PupInfo->end(); ++PUI) {
      if (PUI->getBunchCrossing() < 0) {
        ootpuEarly += PUI->getPU_NumInteractions();      
      } else if (PUI->getBunchCrossing() > 0) {
        ootpuLate += PUI->getPU_NumInteractions();
      } else {
        intpu += PUI->getPU_NumInteractions();
        Tnpv += PUI->getTrueNumInteractions();
      }
    }
    qEvtHdr.setPU(nbx,ootpuEarly,ootpuLate,intpu);
    qEvtHdr.setTrPu(Tnpv);
  } else {
    qEvtHdr.setPthat(0);
    qEvtHdr.setWeight(0);
    qEvtHdr.setPU(0,0,0,0);
    qEvtHdr.setTrPu(0);
  }
  mEvent->setEvtHdr(qEvtHdr);
  
  //---------------- GenJets ---------------------------------------------
  if (mIsMCarlo) {
    // Setting up handy handles
    Handle<GenJetCollection> genJets;
    event.getByToken(mGenJetsName,genJets);
    Handle<reco::JetFlavourInfoMatchingCollection> theJetFlavourInfos;
    event.getByToken(mJetFlavourInfosToken, theJetFlavourInfos );
    Handle<reco::JetFlavourInfoMatchingCollection> theJetFlavourInfosPhys;
    event.getByToken(mJetFlavourInfosTokenPhysicsDef, theJetFlavourInfosPhys );
    event.getByToken(mGenParticles, mGenParts);
    // Genjet loop
    auto igen = genJets->begin();
    auto j = theJetFlavourInfos->begin();
    auto k = theJetFlavourInfosPhys->begin();
    mGenJets.clear();
    mGenFlavour.clear();
    mGenFlavourHadr.clear();
    mGenFlavourPhys.clear();
    mGenBPt.clear();
    for (;igen!=genJets->end() and j!=theJetFlavourInfos->end() and k!=theJetFlavourInfosPhys->end();++igen,++j,++k) {
      if (fabs(igen->eta()) > mMaxEta) continue;

      auto aInfo = j->second;
      auto bInfo = k->second;

      mGenJets.push_back(igen->p4());
      mGenFlavour.push_back((mUseLegacyTag ? get_gjetpartonflav(event,igen) : aInfo.getPartonFlavour()));
      mGenFlavourHadr.push_back(aInfo.getHadronFlavour());
      mGenFlavourPhys.push_back(bInfo.getPartonFlavour());
    }
    map<int,float> jet2bpt;
    // Loop through all prunder particles and then the jets
    for (auto gidx = 0u; gidx < mGenParts->size(); ++gidx) {
      auto &gPart = mGenParts->at(gidx);
      int absId = abs(gPart.pdgId());
      if (absId<100) continue; 
      int code1 = (absId/ 100)%10;
      int code2 = (absId/1000)%10;
      if (code1 == 5 || code2 == 5) {
        int imin = -1;
        float rmin = -1.;
        std::tie(imin,rmin) = best_jet4prtn(gPart);
        if (rmin<0.4 and (jet2bpt.find(imin)==jet2bpt.end() or jet2bpt[imin]<gPart.pt()))
          jet2bpt[imin] = gPart.pt();
      }
    }
    // Fill the best b hadron pt
    for (unsigned igen = 0; igen < mGenJets.size(); ++igen) {
      if (jet2bpt.find(igen)==jet2bpt.end()) mGenBPt.push_back(-1.);
      else mGenBPt.push_back(jet2bpt[igen]);
    }
  }
  
  //---------------- Jets ---------------------------------------------

  // Uncertainties
  edm::ESHandle<JetCorrectorParametersCollection> PFJetCorParColl;
  if (mPFPayloadName != "" and !mIsPFJecUncSet) {
    iSetup.get<JetCorrectionsRecord>().get(mPFPayloadName,PFJetCorParColl);
    JetCorrectorParameters const& PFJetCorPar = (*PFJetCorParColl)["Uncertainty"];
    mPFUnc = new JetCorrectionUncertainty(PFJetCorPar);
    if (mPFJECUncSrc != "") {
      for(unsigned isrc=0;isrc<mPFJECUncSrcNames.size();isrc++) {
        JetCorrectorParameters *parchs = new JetCorrectorParameters(mPFJECUncSrc,mPFJECUncSrcNames[isrc]);
        JetCorrectionUncertainty *tmpUnc = new JetCorrectionUncertainty(*parchs);
        mPFUncSrc.push_back(tmpUnc);
      } // Loop: uncertainty sources 
    } // If uncertainty sources are given
    mIsPFJecUncSet = true;
  } // PF payload is given and uncertainty sources not set
  
  // PFJets
  edm::Handle<edm::View<pat::Jet> > patJets;
  event.getByToken(mPFJetsName,patJets);
  // PF Candidates (for beta calculus)
  edm::Handle<pat::PackedCandidateCollection> cands;
  event.getByToken(mCands, cands);

  // Partons within the jet
  vector< vector<int> > jet2pu;
  for (auto jet=patJets->begin(); jet!=patJets->end(); ++jet) jet2pu.emplace_back(vector<int>());

  // Pick the PF candidates removed by CHS (fromPV==0)
  for (auto cidx = 0u; cidx<cands->size(); ++cidx) {
    auto &c = cands->at(cidx);
    if (c.fromPV()!=0) continue;
    float dRMin = 999.;
    int bestjet = -1;
    // Find the jet that best matches this candidate
    for (auto ijet=patJets->begin(); ijet!=patJets->end(); ++ijet) {
      float dR = reco::deltaR(*ijet,c);
      if (dR<dRMin) {
        dRMin = dR;
        bestjet = ijet-patJets->begin();
      }
    }
    // If the candidate is closer than the jet radius to the jet axis, this is a PU particle for the selected jet
    if (dRMin<0.4) jet2pu[bestjet].push_back(cidx);
  }

  // QG stuff needs to be taken as a separate entity for MiniAOD
  edm::Handle<edm::ValueMap<float>> qglHandle, qgax2Handle, qgptdHandle;
  edm::Handle<edm::ValueMap<int>> qgmulHandle;
  event.getByToken(mQGLToken, qglHandle);
  event.getByToken(mQGAx2Token, qgax2Handle);
  event.getByToken(mQGMulToken, qgmulHandle);
  event.getByToken(mQGPtDToken, qgptdHandle);
  
  // Jet loop
  int maxGenMatch = -1;
  for (auto ijet=patJets->begin(); ijet!=patJets->end(); ++ijet) {
    // Preselection
    if (!ijet->isPFJet()) continue;
    if (fabs(ijet->eta()) > mMaxEta) continue;
    int jetNo = int(ijet-patJets->begin());
    if (ijet->pt() < (jetNo<=2 ? mMinPFPtThirdJet : mMinPFPt)) continue;
    double scale = 1./ijet->jecFactor(0);

    // Keep track of pt and energy for PU stuff
    double che = 0.0;
    double pue = 0.0; 
    // Track parameters by Juska. Not-so-useful for chs jets.
    int mpuTrk(0), mlvTrk(0), mjtTrk(0); // # of pile-up tracks & lead-vertex tracks & all tracks ## Juska

    // Loop through the PF candidates within the jet.
    vector<double> used;
    for (auto pidx = 0u; pidx < ijet->numberOfDaughters(); ++pidx) {
      auto dtr = dynamic_cast<const pat::PackedCandidate*>(ijet->daughter(pidx));
      if (dtr->charge()!=0) {
        che += dtr->energy();
        
        ++mjtTrk; 
        if (dtr->fromPV()==0) {
          // Note: dtr->pvAssociationQuality() is the modern alt, but fromPV is the one used for CHS.
          // Still for some reason, not all fromPV==0 cases are removed. These events fit the old "betaStar" definition (not-from-PV).
          // Due to CHS, these are a vanishing fraction (1/10k), so we don't store 'em anymore.
          ++mpuTrk;
          used.push_back(dtr->energy());
        } else {
          ++mlvTrk;
        }
      }
    }
    // Loop through the pileup PF candidates within the jet.
    for (auto &pidx : jet2pu[jetNo]) {
      auto dtr = cands->at(pidx);
      // We take the candidates that have not appeared before: these were removed by CHS
      if (dtr.charge()!=0 and std::find(used.begin(),used.end(),dtr.energy())==used.end())
        pue += dtr.energy();
    }

    QCDPFJet qcdJet;

    float bPrime = (che ? pue/che : 0.);
    qcdJet.setBetaPrime(bPrime);
    
    
    save_event=true;
     
    //---- jec uncertainty --------------
    double unc(0.0);
    vector<float> uncSrc(0);
    if (mPFPayloadName != "") {
      mPFUnc->setJetEta(ijet->eta());
      mPFUnc->setJetPt(ijet->pt());
      unc = mPFUnc->getUncertainty(true);
    }
    if (mPFJECUncSrc != "") {
      for(unsigned isrc=0;isrc<mPFJECUncSrcNames.size();isrc++) {
        mPFUncSrc[isrc]->setJetEta(ijet->eta());
        mPFUncSrc[isrc]->setJetPt(ijet->pt());
        float unc1 = mPFUncSrc[isrc]->getUncertainty(true);
        uncSrc.push_back(unc1);
      }
    }

    qcdJet.setP4(ijet->p4());
    qcdJet.setCor(scale);
    qcdJet.setUnc(unc);
    qcdJet.setUncSrc(uncSrc);
    qcdJet.setArea(ijet->jetArea());
    
    double chf   = ijet->chargedHadronEnergyFraction();
    double nhf   = ijet->neutralHadronEnergyFraction();// + ijet->HFHadronEnergyFraction();
    double nemf   = ijet->neutralEmEnergyFraction(); // equals to deprecated phf but has HF info too
    double cemf  = ijet->chargedEmEnergyFraction(); // equals to deprecated elf
    double muf   = ijet->muonEnergyFraction();
    double hf_hf = ijet->HFHadronEnergyFraction();
    double hf_phf= ijet->HFEMEnergyFraction();
    int hf_hm    = ijet->HFHadronMultiplicity();
    int hf_phm   = ijet->HFEMMultiplicity();
    int chm      = ijet->chargedHadronMultiplicity();
    int nhm      = ijet->neutralHadronMultiplicity();
    int phm      = ijet->photonMultiplicity();
    int elm      = ijet->electronMultiplicity();
    int mum      = ijet->muonMultiplicity();
    int npr      = ijet->chargedMultiplicity() + ijet->neutralMultiplicity();

    float abseta = fabs(ijet->eta());
    int nm       = ijet->neutralMultiplicity();
    int cm       = ijet->chargedMultiplicity();
    // https://twiki.cern.ch/twiki/bin/viewauth/CMS/JetID
    bool looseID, tightID;
    if (abseta <= 2.7) {
       looseID = nhf<0.99 and nemf<0.99 and npr>1 and (abseta>2.4 || (chf>0 and chm>0 and cemf<mULimCEF));
       tightID = looseID and nhf<0.90 and nemf<0.90;
    } else if (abseta <= 3.0) {
       looseID = nemf<mULimNEF and nemf>mLLimNEF and nm>2 and nhf<mULimNHF;
       tightID = looseID;
    } else {
       looseID = nemf<0.90 and nm>10 and nhf>mLLimNHF;
       tightID = looseID;
    }
     
    qcdJet.setLooseID(looseID);
    qcdJet.setTightID(tightID);
    qcdJet.setFrac(chf,nhf,nemf,cemf,muf);
    qcdJet.setMulti(npr,chm,nhm,phm,elm,mum,cm);
    qcdJet.setHFFrac(hf_hf,hf_phf);
    qcdJet.setHFMulti(hf_hm,hf_phm);
     
    double hof   = ijet->hoEnergyFraction(); // Juska
    qcdJet.setVtxInfo(mpuTrk,mlvTrk,mjtTrk);
    qcdJet.setHO(hof);
    
    //auto pdisc = ijet->getPairDiscri();
    //for (auto &disc : pdisc) cout << "  " << disc.first << endl;

    // Jet flavour tagging discriminators
    qcdJet.pfCombinedCvsL_ = ijet->bDiscriminator("pfCombinedCvsLJetTags");
    qcdJet.pfCombinedCvsB_ = ijet->bDiscriminator("pfCombinedCvsBJetTags");

    qcdJet.pfDeepCSVb_  = ijet->bDiscriminator("pfDeepCSVJetTags:probb");
    qcdJet.pfDeepCSVc_  = ijet->bDiscriminator("pfDeepCSVJetTags:probc");
    qcdJet.pfDeepCSVl_  = ijet->bDiscriminator("pfDeepCSVJetTags:probudsg");
    qcdJet.pfDeepCSVbb_ = ijet->bDiscriminator("pfDeepCSVJetTags:probbb");

    qcdJet.pfDeepFlavourb_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probb");
    qcdJet.pfDeepFlavourc_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probc");
    qcdJet.pfDeepFlavourg_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probg");
    qcdJet.pfDeepFlavourl_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probuds");
    qcdJet.pfDeepFlavourbb_ = ijet->bDiscriminator("pfDeepFlavourJetTags:probbb");

    qcdJet.pfBTag_JetProb_ = ijet->bDiscriminator("pfJetProbabilityBJetTags");
    qcdJet.pfBTag_CombInclSecVtxV2_ = ijet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
    qcdJet.pfBTag_CombMVAV2_ = ijet->bDiscriminator("pfCombinedMVAV2BJetTags");

    float QGL = -1;
    float QGAx2 = -1;
    int QGMul = -1;
    float QGPtD = -1;
    if (mAK4) {
      edm::RefToBase<pat::Jet> jetRef(edm::Ref<edm::View<pat::Jet> >(patJets, jetNo));
      QGL = (*qglHandle)[jetRef];
      QGAx2 = (*qgax2Handle)[jetRef];
      QGMul = (*qgmulHandle)[jetRef];
      QGPtD = (*qgptdHandle)[jetRef];
    }
    qcdJet.setQGTagger(QGL,QGAx2,QGMul,QGPtD);
    qcdJet.SetPUJetId((ijet->hasUserFloat(mPFJetPUID) ? ijet->userFloat(mPFJetPUID) : -999));
    
    if (mIsMCarlo) {
      float partonFlavour=0;
      float hadronFlavour=0;
      float partonFlavourPhys=0;

      if (mUseGenInfo) {
        partonFlavour = ijet->partonFlavour();
        hadronFlavour = ijet->hadronFlavour();
        if (ijet->genParton() != NULL) partonFlavourPhys = ijet->genParton()->pdgId();
      }
    
      float rmin;
      int imin;
      std::tie(imin,rmin) = best_genjet(*ijet);
      if (imin>maxGenMatch) maxGenMatch = imin;

      if (imin!=-1 and rmin<0.4) {
        qcdJet.setGen(imin,rmin);
        // Patch the PF jet flavours if these were not found earlier
        if (mUseGenInfo) {
          if (partonFlavour==0) partonFlavour = mGenFlavour[imin]; 
          if (partonFlavourPhys==0) partonFlavourPhys = mGenFlavourPhys[imin];
          if (hadronFlavour==0) hadronFlavour = mGenFlavourHadr[imin];
        }
      } else {
        // Empty gen jet if no match
        qcdJet.setGen(-1,-1.);
      }
      qcdJet.setFlavour(partonFlavour,hadronFlavour,partonFlavourPhys);
    } else {
      // Empty gen jet for data
      qcdJet.setGen(-1,-1.);
      qcdJet.setFlavour(0,0,0);
    }
    qPFJets.push_back(qcdJet);
  } // jet loop
  sort(qPFJets.begin(),qPFJets.end(),sort_pfjets);
  mEvent->setPFJetsCHS(qPFJets);

  if (mIsMCarlo) {
    // Find the first jet with too small pt (but include all matched gen jets)
    unsigned starter = (maxGenMatch==-1 ? 0 : maxGenMatch+1);
    unsigned limit = mGenJets.size();
    for (unsigned igen = starter; igen<mGenJets.size(); ++igen) {
      auto &genP = mGenJets[igen];
      if (genP.pt()<mMinGenPt) {
        limit = igen;
        break;
      }
    }
    // Remove all the trailing gen info
    while (mGenJets.size()>limit) {
      mGenJets.pop_back();
      mGenBPt.pop_back();
      mGenFlavour.pop_back();
      mGenFlavourHadr.pop_back();
      mGenFlavourPhys.pop_back();
    }
    // Save only the interesting gen info
    mEvent->setGenJets(mGenJets);
    mEvent->setGenBPt(mGenBPt);
    mEvent->setGenFlavour(mGenFlavour);
    mEvent->setGenFlavourHadron(mGenFlavourHadr);
    mEvent->setGenFlavourPhysicsDef(mGenFlavourPhys);
  }
  
  //---------------- MET ---------------------------------------------
  Handle<pat::METCollection> pfmett1;
  event.getByToken(mPFMETt1, pfmett1);
  // MET T1 is available by default in MINIAOD
  const pat::MET &mett1 = pfmett1->front();
  qPFMet_t1.setVar(mett1.et(),mett1.sumEt(),mett1.phi());
  // MET T0 is obtained through a custom patch
  auto mett0 = mett1.corP2(pat::MET::Type0);
  qPFMet_t0.setVar(mett0.pt(),mett1.corSumEt(pat::MET::Type0),mett0.phi());
  // MET T0T1 is found using standard routines
  auto mett0t1 = mett1.corP2(pat::MET::Type01);
  qPFMet_t0t1.setVar(mett0t1.pt(),mett1.corSumEt(pat::MET::Type01),mett0t1.phi());
  mEvent->setPFMET(qPFMet_t1,qPFMet_t0,qPFMet_t0t1);
 
  //-------------- fill the tree -------------------------------------
  if (save_event and mEvent->nPFJetsCHS() >= mMinNPFJets) mTree->Fill();
}

//////////////////////////////////////////////////////////////////////////////////////////
DEFINE_FWK_MODULE(ProcessedTreeProducerBTag);
