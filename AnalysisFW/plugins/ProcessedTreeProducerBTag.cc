#include "SMPJ/AnalysisFW/plugins/ProcessedTreeProducerBTag.h"

ProcessedTreeProducerBTag::ProcessedTreeProducerBTag(edm::ParameterSet const& cfg):
  mSaveWeights(                                                                                      cfg.getParameter<bool>("saveWeights")),
  mAK4(                                                                                     cfg.getUntrackedParameter<bool>("AK4",false)),
  mPrintTriggerMenu(                                                                        cfg.getUntrackedParameter<bool>("printTriggerMenu",false)),
  mIsPFJecUncSet(                                                                                                           false),
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
  //mPFMETt0pc(mayConsume<pat::METCollection>(                                                cfg.getParameter<edm::InputTag>("pfmetT0pc"))),
  //mPFMETt0pct1(mayConsume<pat::METCollection>(                                              cfg.getParameter<edm::InputTag>("pfmetT0pcT1"))),
  //mIsolatedTracks(consumes<pat::IsolatedTrackCollection>(                                                     edm::InputTag("isolatedTracks"))),
  // GEN
  mIsMCarlo(                                                                                cfg.getUntrackedParameter<bool>("isMCarlo",false)),
  mUseGenInfo(                                                                              cfg.getUntrackedParameter<bool>("useGenInfo",false)),
  mMCType(                                                                                   cfg.getUntrackedParameter<int>("mcType",0)), // 0 for Pythia, 1 for Herwig++
  mGenJetsName(mayConsume<GenJetCollection>(                                       cfg.getUntrackedParameter<edm::InputTag>("genjets",edm::InputTag("")))),
  mGenParticles(consumes<reco::GenParticleCollection>(                             cfg.getUntrackedParameter<edm::InputTag>("GenParticles",edm::InputTag("")))),
  mEventInfo(consumes<GenEventInfoProduct>(                                        cfg.getUntrackedParameter<edm::InputTag>("EventInfo",edm::InputTag("")))),
  mGenEvtInfo(consumes<GenEventInfoProduct>(                                                                  edm::InputTag("generator"))),
  mSrcPU(mayConsume<std::vector<PileupSummaryInfo> >(                              cfg.getUntrackedParameter<edm::InputTag>("srcPULabel",edm::InputTag("")))),
  mJetFlavourInfosToken(consumes<reco::JetFlavourInfoMatchingCollection>(          cfg.getUntrackedParameter<edm::InputTag>("jetFlavourInfos",edm::InputTag("")))),
  mJetFlavourInfosTokenPhysicsDef(consumes<reco::JetFlavourInfoMatchingCollection>(cfg.getUntrackedParameter<edm::InputTag>("jetFlavourInfosPhysicsDef",edm::InputTag("")))),
  mQGLToken(consumes<                                                                   edm::ValueMap<float>>(edm::InputTag("QGTagger", "qgLikelihood"))),
  mQGAx2Token(consumes<                                                                 edm::ValueMap<float>>(edm::InputTag("QGTagger", "axis2"))),
  mQGMulToken(consumes<                                                                   edm::ValueMap<int>>(edm::InputTag("QGTagger", "mult"))),
  mQGPtDToken(consumes<                                                                 edm::ValueMap<float>>(edm::InputTag("QGTagger", "ptD"))),

  // Trigger
  mProcessName(                                                                      cfg.getUntrackedParameter<std::string>("processName","")),
  mTriggerNames(                                                                cfg.getParameter<std::vector<std::string> >("triggerName")),
  mTriggerFollows(                                                              cfg.getParameter<std::vector<std::string> >("triggerFollow")),
  mTriggerBits(mayConsume<edm::TriggerResults>(                                    cfg.getUntrackedParameter<edm::InputTag>("triggerResults",edm::InputTag("")))),
  mTriggerHLTObjs(mayConsume<pat::TriggerObjectStandAloneCollection>(              cfg.getUntrackedParameter<edm::InputTag>("triggerHLTObjs",edm::InputTag("")))),
  mTriggerL1Objs(mayConsume<BXVector<l1t::Jet> >(                                  cfg.getUntrackedParameter<edm::InputTag>("triggerL1Objs",edm::InputTag("")))),
  mTriggerL1HTObjs(mayConsume<BXVector<l1t::EtSum> >(                              cfg.getUntrackedParameter<edm::InputTag>("triggerL1HTObjs",edm::InputTag("")))),
  mTriggerPrescales(consumes<pat::PackedTriggerPrescales>(                                  cfg.getParameter<edm::InputTag>("prescales"))),
  mTriggerPrescalesL1Min(consumes<pat::PackedTriggerPrescales>(                             cfg.getParameter<edm::InputTag>("prescalesL1Min"))),
  mTriggerPrescalesL1Max(consumes<pat::PackedTriggerPrescales>(                             cfg.getParameter<edm::InputTag>("prescalesL1Max"))),
  //mHBHENoiseFilterResultLabel(mayConsume<bool>(                                    cfg.getUntrackedParameter<edm::InputTag>("HBHENoiseFilterResultLabel",edm::InputTag(""))    )),
  //mHBHENoiseFilterResultNoMinZLabel(mayConsume<bool>(                              cfg.getUntrackedParameter<edm::InputTag>("HBHENoiseFilterResultNoMinZLabel",edm::InputTa    g("")))),
  mCands(mayConsume<pat::PackedCandidateCollection>(edm::InputTag("packedPFCandidates"))),
  mHLTPrescale(cfg, consumesCollector(), *this)
{
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
}
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
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::endJob() {}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup)
{
  if (mIsMCarlo) return;
  bool changed(true);
  if (mHLTConfig.init(iRun,iSetup,mProcessName,changed) and mHLTPrescale.init(iRun, iSetup, mProcessName, changed) ) {
    if (!changed) return;
    cout<<"New trigger menu found !!!"<<endl;
    mTriggerIndex.clear();
    const unsigned int n(mHLTConfig.size());
    // We select the triggers that are studied later on
    for (unsigned itrig=0; itrig<mTriggerNames.size(); ++itrig) {
      auto &trgName = mTriggerNames[itrig];
      auto trgIdx = mHLTConfig.triggerIndex(trgName);
      mTriggerIndex.push_back(trgIdx);
      mTriggerNamesHisto->Fill(trgName.c_str(),1);
      mTriggerPassHisto->Fill(trgName.c_str(),0);
      cout<<mTriggerNames[itrig]<<" "<<trgIdx<<" ";
      if (trgIdx >= n) cout<<"does not exist in the current menu"<<endl;
      else cout<<"exists"<<endl;
    }

    if (mPrintTriggerMenu) {
      cout << "Available TriggerNames are: " << endl;
      mHLTConfig.dump("Triggers");
    }
  } else {
    cout << "ProcessedTreeProducerBTag::analyze:"
         << " config extraction failure with process name "
         << mProcessName << endl;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::analyze(edm::Event const& event, edm::EventSetup const& iSetup)
{
  vector<QCDPFJet>      mPFJets;
  vector<LorentzVector> mGenJets;
  vector<float> GenFlavour;
  vector<float> GenHadronFlavour;
  QCDEventHdr mEvtHdr;
  QCDMET mPFMet_t1, mPFMet_t0pc, mPFMet_t0pct1;

  bool save_event=false;
  
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
  
  //-------------- Trigger Info ----------------------------------- 
  edm::Handle<edm::TriggerResults> triggerBits;
  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerHLTObjs;
  edm::Handle<BXVector<l1t::Jet> > triggerL1Objs;
  edm::Handle<BXVector<l1t::EtSum> > triggerL1HTObjs;
  edm::Handle<pat::PackedTriggerPrescales> triggerPrescales;
  edm::Handle<pat::PackedTriggerPrescales> triggerPrescalesL1Max;
  edm::Handle<pat::PackedTriggerPrescales> triggerPrescalesL1Min;
  
  if (!mIsMCarlo){
    map<string, vector<LorentzVector> > vvHLT;
    vector<vector<LorentzVector> > mL1Objs, mHLTObjs;  
    vector<int> L1Prescales,HLTPrescales,Fired;                                                                                                                          

    event.getByToken(mTriggerBits, triggerBits);
    event.getByToken(mTriggerHLTObjs, triggerHLTObjs);
    event.getByToken(mTriggerL1Objs, triggerL1Objs);
    event.getByToken(mTriggerL1HTObjs, triggerL1HTObjs);
    event.getByToken(mTriggerPrescales, triggerPrescales);
    event.getByToken(mTriggerPrescalesL1Min, triggerPrescalesL1Min);
    event.getByToken(mTriggerPrescalesL1Max, triggerPrescalesL1Max);
    
    //Variables
    unsigned fireprim = 0;
    unsigned emptyprim = 0; 
    unsigned fireoth = 0;
    unsigned emptyoth = 0;
    const edm::TriggerNames &names = event.triggerNames(*triggerBits);
    for (unsigned int itrig=0; itrig<triggerBits->size(); ++itrig) {
      // Stop when all trigger names are found
      if ((fireprim+emptyprim==mTriggerNames.size()) and (emptyoth+fireoth==mTriggerFollows.size())) break;
      // Look for the current trigger within interesting triggers
      string trgName = string(names.triggerName(itrig));
      auto place = std::find(mTriggerNames.begin(),mTriggerNames.end(),trgName);
      if (place!=mTriggerNames.end()) {
        int pidx = place-mTriggerNames.begin();
        
        if (triggerBits->accept(itrig)) {
          L1Prescales.push_back(max(triggerPrescalesL1Max->getPrescaleForIndex(itrig),triggerPrescalesL1Min->getPrescaleForIndex(itrig)));
          HLTPrescales.push_back(triggerPrescales->getPrescaleForIndex(itrig));
          mTriggerPassHisto->Fill(trgName.c_str(),1);
          Fired.push_back(pidx);
          if (vvHLT.find(trgName)==vvHLT.end()) vvHLT[trgName] = vector<LorentzVector>();
          ++fireprim;
        } else {
          ++emptyprim;
        }
      } else { 
        // Check if a "followed" trigger has fired
        auto fplace = std::find(mTriggerFollows.begin(),mTriggerFollows.end(),trgName);
        if (fplace!=mTriggerFollows.end()) {
          if (triggerBits->accept(itrig)) ++fireoth;
          else ++emptyoth;
        }
      }
    }
    sort(Fired.begin(),Fired.end());
    // If none of the monitored triggers have fired, skip this as a redundant event
    if (fireprim==0 and fireoth==0) return;
    
    if (fireprim>0) {
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
            TLorentzVector P4;
            P4.SetPtEtaPhiM(obj.pt(),obj.eta(),obj.phi(),obj.mass());
            vvHLT[trgName].emplace_back(P4.Px(),P4.Py(),P4.Pz(),P4.E());
          }
        }
      }
      for (auto &trg : Fired) mHLTObjs.push_back(vvHLT[mTriggerNames[trg]]); 

      // L1 sector
      vector<LorentzVector> vvL1;
      for (auto obj = triggerL1Objs->begin(0); obj != triggerL1Objs->end(0); ++obj) {
        TLorentzVector P4;
        P4.SetPtEtaPhiM(obj->pt(),obj->eta(),obj->phi(),obj->mass());
        vvL1.emplace_back(P4.Px(),P4.Py(),P4.Pz(),P4.E());
      }
      mL1Objs.push_back(vvL1);

      if (!mAK4) {
        // We store JetHT stuff into AK8 tuples
        vector<LorentzVector> vvL1HT;
        for (auto obj = triggerL1HTObjs->begin(0); obj != triggerL1HTObjs->end(0); ++obj) {
          if (obj->getType()==l1t::EtSum::kTotalHt) {
            TLorentzVector P4;
            P4.SetPtEtaPhiM(obj->pt(),obj->eta(),obj->phi(),obj->mass());
            vvL1HT.emplace_back(P4.Px(),P4.Py(),P4.Pz(),P4.E());
          }
        }
        mL1Objs.push_back(vvL1HT);
      }
    }
  
    mEvent->setTrigDecision(Fired);
    mEvent->setPrescales(L1Prescales,HLTPrescales);
    mEvent->setL1Obj(mL1Objs);
    mEvent->setHLTObj(mHLTObjs);
  }
  
  //-------------- Vertex Info -----------------------------------
  Handle<reco::VertexCollection> recVtxs;
  event.getByToken(mOfflineVertices,recVtxs);
  
  //------------- reject events without reco vertices ------------
  int VtxGood(0);
  bool isPVgood(false);
  float PVx(0),PVy(0),PVz(0),PVndof(0);
  for (auto i_vtx = recVtxs->begin(); i_vtx != recVtxs->end(); i_vtx++) {
    int index = i_vtx-recVtxs->begin();
    if (index == 0) {
      PVx    = i_vtx->x();
      PVy    = i_vtx->y();
      PVz    = i_vtx->z();
      PVndof = i_vtx->ndof();
    }
    if (!(i_vtx->isFake()) && i_vtx->ndof() >= mGoodVtxNdof && fabs(i_vtx->z()) <= mGoodVtxZ) {
      if (index == 0) isPVgood = true;
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
  //-------------- Generator Info -------------------------------------
  Handle<GenEventInfoProduct> hEventInfo;
  //-------------- Simulated PU Info ----------------------------------
  Handle<std::vector<PileupSummaryInfo> > PupInfo;
  if (mIsMCarlo && mUseGenInfo) {
    event.getByToken(mEventInfo, hEventInfo);
    if(hEventInfo->hasBinningValues())
      mEvtHdr.setPthat(hEventInfo->binningValues()[0]);
    else
      mEvtHdr.setPthat(0);
  
    mEvtHdr.setWeight(hEventInfo->weight());
    event.getByToken(mSrcPU, PupInfo);
    std::vector<PileupSummaryInfo>::const_iterator PUI;
    int nbx = PupInfo->size();
    int ootpuEarly(0),ootpuLate(0),intpu(0);
    float Tnpv = -1.; // new variable for computing pileup weight factor for the event
  
    for(PUI = PupInfo->begin(); PUI != PupInfo->end(); ++PUI) {
      if (PUI->getBunchCrossing() < 0)
        ootpuEarly += PUI->getPU_NumInteractions();      
      else if (PUI->getBunchCrossing() > 0)
        ootpuLate += PUI->getPU_NumInteractions();
      else {
        intpu += PUI->getPU_NumInteractions();
        Tnpv = PUI->getTrueNumInteractions();
       }
    }
    mEvtHdr.setPU(nbx,ootpuEarly,ootpuLate,intpu);
    mEvtHdr.setTrPu(Tnpv);
  
    event.getByToken(mGenEvtInfo,mGenEvtInfoHandle);
    mEvtHdr.setWeight(mGenEvtInfoHandle->weight());
    if (mGenEvtInfoHandle->hasBinningValues()) mEvtHdr.setPthat(mGenEvtInfoHandle->binningValues()[0]);
    else mEvtHdr.setPthat(0);
  } else {
    mEvtHdr.setPthat(0);
    mEvtHdr.setWeight(0);
    mEvtHdr.setPU(0,0,0,0);
    mEvtHdr.setTrPu(0);
  }
  
  //---------------- Gen Jets ---------------------------------------------
  Handle<GenJetCollection>  genjets;
  if (mIsMCarlo) {
    event.getByToken(mGenJetsName,genjets);
    for(GenJetCollection::const_iterator i_gen = genjets->begin(); i_gen != genjets->end(); i_gen++) {
      if (i_gen->pt() > mMinGenPt && fabs(i_gen->y()) < mMaxY) {
        mGenJets.push_back(i_gen->p4());
  
        //ADD FLAVOUR AT GEN LEVEL
        int FlavourGen = getMatchedPartonGen(event,i_gen);
        //if(FlavourGen<-100) cout<<FlavourGen<<" "<<i_gen->pt()<<" "<<i_gen->eta()<<" "<<i_gen->phi()<<endl;
        GenFlavour.push_back(FlavourGen);
      }
    }
  
    edm::Handle<reco::JetFlavourInfoMatchingCollection> theJetFlavourInfos;
    event.getByToken(mJetFlavourInfosToken, theJetFlavourInfos );
    
    for ( reco::JetFlavourInfoMatchingCollection::const_iterator j  = theJetFlavourInfos->begin();j != theJetFlavourInfos->end();++j ) {
      reco::JetFlavourInfo aInfo = (*j).second;
      int FlavourGenHadron = aInfo.getHadronFlavour();
      //if(FlavourGenHadron==5) cout<<FlavourGenHadron<<" "<<aJet->pt()<<" "<<aJet->eta()<<" "<<aJet->phi()<<" HADRONFLAV"<<endl;
      GenHadronFlavour.push_back(FlavourGenHadron);
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
      }
    }
    mIsPFJecUncSet = true;
  }
  
  // PF Candidates
  edm::Handle<edm::View<pat::Jet> > patjetschs;
  event.getByToken(mPFJetsName,patjetschs);
  edm::Handle<pat::PackedCandidateCollection> cands;
  event.getByToken(mCands, cands);

  // Partons within the jet
  vector< vector<int> > jet2pu;
  for (auto jet=patjetschs->begin(); jet!=patjetschs->end(); ++jet) jet2pu.emplace_back(vector<int>());

  for (auto cidx = 0u; cidx<cands->size(); ++cidx) {
    auto &c = cands->at(cidx);
    if (c.fromPV()!=0) continue;
    float dRMin = 999.;
    int bestjet = -1;
    for (auto ijet=patjetschs->begin(); ijet!=patjetschs->end(); ++ijet) {
      float dR = reco::deltaR(*ijet,c);
      if (dR<dRMin) {
        dRMin = dR;
        bestjet = ijet-patjetschs->begin();
      }
    }
    if (dRMin<0.4) jet2pu[bestjet].push_back(cidx);
  }

  // QG stuff
  edm::Handle<edm::ValueMap<float>> qglHandle, qgax2Handle, qgptdHandle;
  edm::Handle<edm::ValueMap<int>> qgmulHandle;
  event.getByToken(mQGLToken, qglHandle);
  event.getByToken(mQGAx2Token, qgax2Handle);
  event.getByToken(mQGMulToken, qgmulHandle);
  event.getByToken(mQGPtDToken, qgptdHandle);
  
  // Jet loop
  for (auto ijet=patjetschs->begin(); ijet!=patjetschs->end(); ++ijet) {
    QCDPFJet qcdjet;
    if (!ijet->isPFJet()) continue;

    int jidx = int(ijet-patjetschs->begin());
    if (qcdjet.pt() < (jidx<=2 ? mMinPFPtThirdJet : mMinPFPt)) continue;

    //cout << ijet->numberOfDaughters() << endl;
    double chpt = 0.0;
    double pupt = 0.0;
    double che = 0.0;
    double pue = 0.0; 
    // Dunno how useful these are in chs jets...
    int mpuTrk(0), mlvTrk(0); // # of pile-up tracks & lead-vertex tracks ## Juska
    int mjtTrk(0); // multiplicity of _all_ tracks in jet (also vtx-unassociated!) ## Juska

    for (auto pidx = 0u; pidx<ijet->numberOfDaughters(); ++pidx) {
      auto *dtr = dynamic_cast<const pat::PackedCandidate*>(ijet->daughter(pidx));
      //auto oldassoc = dtr->fromPV();
      //auto assoc = dtr->pvAssociationQuality();
      if (dtr->charge()!=0) {
        ++mjtTrk;
        ++mlvTrk;
        chpt += dtr->pt();
        che += dtr->energy();
      }
      //cout << "  " << dtr->pdgId() << " " << dtr->charge() << " " << pidx << " " << oldassoc << " " << assoc << endl;
    }
    //cout << "Found:" << endl;
    for (auto &pidx : jet2pu[jidx]) {
      auto dtr = cands->at(pidx);
      //auto oldassoc = dtr.fromPV();
      //auto assoc = dtr.pvAssociationQuality();
      if (dtr.charge()!=0) {
        ++mjtTrk;
        ++mpuTrk;
        pupt += dtr.pt();
        pue += dtr.energy();
      }
      //cout << "  " << dtr.pdgId() << " " << dtr.charge() << " " << pidx << " " << oldassoc << " " << assoc << endl;
    }

    //bPrime = (chpt ? pupt/chpt : 0.);
    float bPrime = (che ? pue/che : 0.);
    qcdjet.setBeta(1.-bPrime);
    qcdjet.setBetaStar(0.);
    qcdjet.setBetaPrime(bPrime);

    double scale = 1./ijet->jecFactor(0); // --- the value of the JEC factor
    //cout << "Jet " << jidx << endl;
    //cout << pupt/chpt << " " << chpt << " " << pupt << endl;
    //cout << pue/che << " " << che << " " << pue << endl;
    
    //---- preselection -----------------
    if (fabs(ijet->y()) > mMaxY) continue;
    if (fabs(ijet->pt()) < mMinPFPt) continue;
    
    save_event=true;
    
    qcdjet.setBeta(0);
    qcdjet.setBetaStar(0);
     
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

    
    qcdjet.setP4(ijet->p4());
    qcdjet.setCor(scale);
    qcdjet.setUnc(unc);
    qcdjet.setUncSrc(uncSrc);
    qcdjet.setArea(ijet->jetArea());
    
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
     
    qcdjet.setLooseID(looseID);
    qcdjet.setTightID(tightID);
    qcdjet.setFrac(chf,nhf,nemf,cemf,muf);
    qcdjet.setMulti(npr,chm,nhm,phm,elm,mum,cm);
    qcdjet.setHFFrac(hf_hf,hf_phf);
    qcdjet.setHFMulti(hf_hm,hf_phm);
     
    double hof   = ijet->hoEnergyFraction(); // Juska
    qcdjet.setVtxInfo(mpuTrk,mlvTrk,mjtTrk);
    qcdjet.setHO(hof);
    
    //if (ijet == patjetschs->begin()) {
    //  auto pdisc = ijet->getPairDiscri();
    //  cout << "Disc" << endl;
    //  for (auto &disc : pdisc) cout << "  " << disc.first << endl;
    //}

    // Jet flavour tagging discriminators
    qcdjet.pfBoosted_ = ijet->bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags");
    qcdjet.pfCombinedCvsL_ = ijet->bDiscriminator("pfCombinedCvsLJetTags");
    qcdjet.pfCombinedCvsB_ = ijet->bDiscriminator("pfCombinedCvsBJetTags");

    qcdjet.pfDeepCSVb_  = ijet->bDiscriminator("pfDeepCSVJetTags:probb");
    qcdjet.pfDeepCSVc_  = ijet->bDiscriminator("pfDeepCSVJetTags:probc");
    qcdjet.pfDeepCSVl_  = ijet->bDiscriminator("pfDeepCSVJetTags:probudsg");
    qcdjet.pfDeepCSVbb_ = ijet->bDiscriminator("pfDeepCSVJetTags:probbb");

    if (mRunYear!="2016") {
      qcdjet.pfDeepFlavourb_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probb");
      qcdjet.pfDeepFlavourc_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probc");
      qcdjet.pfDeepFlavourg_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probg");
      qcdjet.pfDeepFlavourl_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probuds");
      qcdjet.pfDeepFlavourbb_ = ijet->bDiscriminator("pfDeepFlavourJetTags:probbb");
    }

    qcdjet.pfBTag_JetProb_ = ijet->bDiscriminator("pfJetProbabilityBJetTags");
    qcdjet.pfBTag_CombInclSecVtxV2_ = ijet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
    qcdjet.pfBTag_CombMVAV2_ = ijet->bDiscriminator("pfCombinedMVAV2BJetTags");

    float QGL = -1;
    float QGAx2 = -1;
    int QGMul = -1;
    float QGPtD = -1;
    if (mAK4) {
      edm::RefToBase<pat::Jet> jetRef(edm::Ref<edm::View<pat::Jet> >(patjetschs, jidx));
      QGL = (*qglHandle)[jetRef];
      QGAx2 = (*qgax2Handle)[jetRef];
      QGMul = (*qgmulHandle)[jetRef];
      QGPtD = (*qgptdHandle)[jetRef];
    }
    qcdjet.setQGTagger(QGL,QGAx2,QGMul,QGPtD);
    qcdjet.SetPUJetId((ijet->hasUserFloat(mPFJetPUID) ? ijet->userFloat(mPFJetPUID) : -999));
    
    if (mIsMCarlo) {
      float partonFlavour=0;
      float hadronFlavour=0;
      float partonFlavourPhysicsDef=0;

      if (mUseGenInfo) {
        partonFlavour = ijet->partonFlavour();
        hadronFlavour = ijet->hadronFlavour();
        if (ijet->genParton() != NULL) partonFlavourPhysicsDef = ijet->genParton()->pdgId();
      }
    
      float rmin(999);
      int imin = -1;
      if (genjets->size()>0) {
        float ratiouplim = (ijet->pt()>20 ? 1.5 : 2.5);
        float ratiololim = (ijet->pt()>20 ? 0.4 : 0.25);
        // Find approximately the closest pt and then iterate up and down
        // If a good match is found, iteration stops
        int dwn = genorder(ijet->pt(),genjets,0,genjets->size()-1);
        int up = dwn+1;
        int limit = genjets->size();
        while (dwn>=0 or up<limit) {
          if (dwn>=0) {
            if ((genjets->at(dwn).pt())/(ijet->pt())<ratiouplim) {
              float deltaR = reco::deltaR(*ijet,genjets->at(dwn));
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
            if ((genjets->at(up).pt())/(ijet->pt())>ratiololim) {
              float deltaR = reco::deltaR(*ijet,genjets->at(up));
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
      }
      if (imin!=-1 and rmin<0.4) {
        qcdjet.setGen(genjets->at(imin).p4(),rmin);
        // Patch the PF jet flavours
        //if (partonFlavour==0) partonFlavour = genFlavour[imin]; 
        //if (partonFlavourPhysicsDef==0) partonFlavourPhysicsDef = genFlavourPhys[imin];
        //if (hadronFlavour==0) hadronFlavour = genFlavourHadr[imin];
      } else {
        // Empty gen jet if no match
        qcdjet.setGen(LorentzVector(0,0,0,0),0);
      }
      qcdjet.setFlavour(partonFlavour,hadronFlavour,partonFlavourPhysicsDef);
    } else {
      // Empty gen jet for data
      qcdjet.setGen(LorentzVector(0.0,0.0,0.0,0.0),0);
      qcdjet.setFlavour(0,0,0);
    }
    mPFJets.push_back(qcdjet);
  } // jet loop
  
  //---------------- met ---------------------------------------------
  Handle<pat::METCollection> pfmett1;
  event.getByToken(mPFMETt1, pfmett1);
  const pat::MET &mett1 = pfmett1->front();
  mPFMet_t1.setVar(mett1.et(),mett1.sumEt(),mett1.phi());
  cout << "T1: " << mett1.et() << endl;
  auto v1 = mett1.corP2(pat::MET::Type1);
  cout << "T1: " << v1.pt() << endl;
  auto v2 = mett1.corP2(pat::MET::Raw);
  cout << "RAW: " << v2.pt() << endl;
  auto v3 = mett1.corP2(pat::MET::Type01);
  cout << "T01: " << v3.pt() << endl; 
  auto v4 = mett1.corP2(pat::MET::Type01XY);
  cout << "T01XY: " << v4.pt() << endl;
 
  //-------------- fill the tree -------------------------------------
  sort(mPFJets.begin(),mPFJets.end(),sort_pfjets);
  mEvent->setEvtHdr(mEvtHdr);
  mEvent->setPFJetsCHS(mPFJets);
  if (mIsMCarlo) {
    mEvent->setGenJets(mGenJets);
    //mEvent->setGenFlavour(GenFlavour);
    //mEvent->setGenHadronFlavour(GenHadronFlavour);
  }
  mEvent->setPFMET(mPFMet_t1,mPFMet_t1,mPFMet_t1);

  if (save_event) {
    mTree->Fill();
  } 
}


/////////////// Matching Flavour ///////////////////////////////

int ProcessedTreeProducerBTag::getMatchedPartonGen(edm::Event const& event,GenJetCollection::const_iterator i_gen)
{
  int jetFlavour=-100;
  bool switchB=0;
  bool switchC=0;

  edm::Handle<reco::GenParticleCollection> genParticles;
  event.getByToken(mGenParticles, genParticles);

  for (size_t i = 0; i < genParticles->size (); ++i) {
      const GenParticle & genIt = (*genParticles)[i];
      int pdgId = genIt.pdgId();
      double DeltaR=deltaR(genIt.p4().eta(),genIt.p4().phi(),i_gen->eta(),i_gen->phi());
      double DeltaRmin=0.3;
      if (DeltaR < DeltaRmin ) {
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
