#include "SMPJ/AnalysisFW/plugins/ProcessedTreeProducerBTag.h"

ProcessedTreeProducerBTag::ProcessedTreeProducerBTag(edm::ParameterSet const& cfg):
  mAK4(cfg.getUntrackedParameter                                                                                     <bool>("AK4",false)),
  mPrintTriggerMenu(                                                                        cfg.getUntrackedParameter<bool>("printTriggerMenu",false)),
  mIsPFJecUncSet(                                                                                                        false),
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
  mPFPayloadName(                                                                          cfg.getParameter<std::string>("PFPayloadNameCHS")),
  mRunYear(                                                                          cfg.getUntrackedParameter<std::string>("runYear","2016")),
  mPFJetPUID(                                                                              cfg.getParameter<std::string>("pfchsjetpuid")),
  mPFJECUncSrc(                                                                   cfg.getUntrackedParameter<std::string>("jecUncSrcCHS","")),
  mPFJECUncSrcNames(                                                            cfg.getParameter<std::vector<std::string> >("jecUncSrcNames")),
  mOfflineVertices(mayConsume<reco::VertexCollection>(                                      cfg.getParameter<edm::InputTag>("offlineVertices"))),
  mBeamSpot(mayConsume<reco::BeamSpot>(                                                     cfg.getParameter<edm::InputTag>("beamSpot"))),
  mPFJetsName(consumes<edm::View<pat::Jet>>(                                             cfg.getParameter<edm::InputTag>("pfjetschs"))),
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
  mJetFlavourInfosToken(consumes<reco::JetFlavourInfoMatchingCollection>(          cfg.getUntrackedParameter<edm::InputTag>("jetFlavourInfos",edm::InputTag("")))),
  mJetFlavourInfosTokenPhysicsDef(consumes<reco::JetFlavourInfoMatchingCollection>(cfg.getUntrackedParameter<edm::InputTag>("jetFlavourInfosPhysicsDef",edm::InputTag("")))),
  // Trigger
  mProcessName(                                                                      cfg.getUntrackedParameter<std::string>("processName","")),
  mTriggerNames(                                                                cfg.getParameter<std::vector<std::string> >("triggerName")),
  mTriggerResultsTag(mayConsume<edm::TriggerResults>(                              cfg.getUntrackedParameter<edm::InputTag>("triggerResults",edm::InputTag("")))),
  mTriggerEventTag(mayConsume<trigger::TriggerEvent>(                              cfg.getUntrackedParameter<edm::InputTag>("triggerEvent",edm::InputTag("")))),
  mHBHENoiseFilterResultLabel(mayConsume<bool>(                                    cfg.getUntrackedParameter<edm::InputTag>("HBHENoiseFilterResultLabel",edm::InputTag("")))),
  mHBHENoiseFilterResultNoMinZLabel(mayConsume<bool>(                              cfg.getUntrackedParameter<edm::InputTag>("HBHENoiseFilterResultNoMinZLabel",edm::InputTag("")))),
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
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::endJob() {}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup)
{
  if (mIsMCarlo) return;
  bool changed(true);
  if (mHLTConfig.init(iRun,iSetup,mProcessName,changed) and mHLTPrescale.init(iRun, iSetup, mProcessName, changed)) {
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
    cout << "ProcessedTreeProducerBTag::analyze: config extraction failure with process name " << mProcessName << endl;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::analyze(edm::Event const& event, edm::EventSetup const& iSetup)
{
  vector<QCDPFJet>      mPFJets;
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
    event.getByToken(mTriggerResultsTag,mTriggerResultsHandle);
    if (!mTriggerResultsHandle.isValid()) {
      cout << "ProcessedTreeProducerBTag::analyze: Error in getting TriggerResults product from Event!" << endl;
      return;
    }
    event.getByToken(mTriggerEventTag,mTriggerEventHandle);
    if (!mTriggerEventHandle.isValid()) {
      cout << "ProcessedTreeProducerBTag::analyze: Error in getting TriggerEvent product from Event!" << endl;
      return;
    }
    assert(mTriggerResultsHandle->size() == mHLTConfig.size());
    //------ loop over triggers selected and available ---------
    unsigned firecount = 0;
    for (unsigned itrig=0;itrig<mTriggerNames.size();itrig++) {
      auto &trgName = mTriggerNames[itrig];
      vector<LorentzVector> vvL1,vvHLT;
      bool accept = mTriggerResultsHandle->accept(mTriggerIndex[itrig]);
 
      std::pair<std::vector<std::pair<std::string,int> >,int> detailedPrescaleInfo = mHLTPrescale.prescaleValuesInDetail(event, iSetup, trgName);
      int preHLT = detailedPrescaleInfo.second;
  
      // save l1 prescale values in standalone vector
      std::vector <int> l1prescalevals;
      for( size_t varind = 0; varind < detailedPrescaleInfo.first.size(); varind++ )
        l1prescalevals.push_back(detailedPrescaleInfo.first.at(varind).second);
     
      //find and save minimum l1 prescale of any ORed L1 that seeds the HLT
      std::vector<int>::iterator result = std::min_element(std::begin(l1prescalevals), std::end(l1prescalevals));
      size_t minind = std::distance(std::begin(l1prescalevals), result);
      // sometimes there are no L1s associated with a HLT. In that case, this branch stores -1 for the l1prescale
      int preL1 = minind < l1prescalevals.size() ? l1prescalevals.at(minind) : -1 ;
      
      ///end in detail
      int tmpFired(0);
      if (accept) {
        ++firecount;
        mTriggerPassHisto->Fill(trgName.c_str(),1);
        tmpFired = 1;
      }
  
      //--------- modules on this trigger path--------------
      const vector<string>& moduleLabels(mHLTConfig.moduleLabels(mTriggerIndex[itrig]));
      const unsigned int moduleIndex(mTriggerResultsHandle->index(mTriggerIndex[itrig]));
      bool foundL1(false);
      for(unsigned int j=0; j<=moduleIndex; ++j) {
        const string& moduleLabel(moduleLabels[j]);
        const string  moduleType(mHLTConfig.moduleType(moduleLabel));
        //--------check whether the module is packed up in TriggerEvent product
        const unsigned int filterIndex(mTriggerEventHandle->filterIndex(InputTag(moduleLabel,"",mProcessName)));
        if (filterIndex<mTriggerEventHandle->sizeFilters()) {
          const Vids& VIDS (mTriggerEventHandle->filterIds(filterIndex));
          const Keys& KEYS(mTriggerEventHandle->filterKeys(filterIndex));
          const size_type nI(VIDS.size());
          const size_type nK(KEYS.size());
          assert(nI==nK);
          const size_type n(max(nI,nK));
          const TriggerObjectCollection& TOC(mTriggerEventHandle->getObjects());
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
    if (firecount==0) {
      bool nofireoth = true;
      for (unsigned itrig=0;itrig<mTriggerFollows.size();itrig++) {
        if (mTriggerResultsHandle->accept(mTriggerIndex[itrig])) {
          nofireoth = false;
          break;
        }
      }
      if (nofireoth) return;
    }
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
  vector<int> genFlavourHadr;
  vector<int> genFlavourPhys;
  if (mIsMCarlo) {
    // These are for local use - only a subset of the gen jets
    vector<float> GenFlavour;
    vector<float> GenFlavourHadr;
    vector<float> GenFlavourPhys;
    vector<LorentzVector> mGenJets;
    // Fetch the collections!
    event.getByToken(mGenJetsName,genjets);
    edm::Handle<reco::JetFlavourInfoMatchingCollection> theJetFlavourInfos;
    event.getByToken(mJetFlavourInfosToken, theJetFlavourInfos );
    edm::Handle<reco::JetFlavourInfoMatchingCollection> theJetFlavourInfosPhysicsDef;
    event.getByToken(mJetFlavourInfosTokenPhysicsDef, theJetFlavourInfosPhysicsDef );
    // Genjet loop
    auto i_gen = genjets->begin();
    auto j = theJetFlavourInfos->begin();
    auto k = theJetFlavourInfosPhysicsDef->begin();
    for (;i_gen!=genjets->end() and j!=theJetFlavourInfos->end() and k!=theJetFlavourInfosPhysicsDef->end();++i_gen,++j,++k) {
      auto aInfo = j->second;
      auto bInfo = k->second;
      int gflav = (mUseLegacyTag ? getMatchedPartonGen(event,i_gen) : aInfo.getPartonFlavour());
      int gflavhad = aInfo.getHadronFlavour();
      int gflavphy = bInfo.getPartonFlavour();

      genFlavour.push_back(gflav);
      genFlavourPhys.push_back(gflavphy);
      genFlavourHadr.push_back(gflavhad);
      if (i_gen->pt() > mMinGenPt && fabs(i_gen->y()) < mMaxY) {
        mGenJets.push_back(i_gen->p4());
        GenFlavour.push_back(gflav);
        GenFlavourHadr.push_back(gflavhad);
        GenFlavourPhys.push_back(gflavphy);
      }
    }
    mEvent->setGenJets(mGenJets);
    mEvent->setGenFlavour(GenFlavour);
    mEvent->setGenFlavourHadron(GenFlavourHadr);
    mEvent->setGenFlavourPhysicsDef(GenFlavourPhys);
  }
  
  //---------------- Jets ---------------------------------------------
  // --------  Uncertainty part ----------------//
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
      } // for(unsigned isrc=0;isrc<mPFJECUncSrcNames.size();isrc++)
    } // if (mPFJECUncSrc != "")
    mIsPFJecUncSet = true;
  } // if (mPFPayloadName != "" && !mIsPFJecUncSet)


  //----------- PFJets   part -------------------------
  edm::Handle<edm::View<pat::Jet> > patjetschs;
  event.getByToken(mPFJetsName,patjetschs);

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
    
  for (auto ijet=patjetschs->begin(); ijet!=patjetschs->end(); ++ijet) { 
    if (!ijet->isPFJet()) continue;

    //---- preselection -----------------
    int jetNo = ijet-patjetschs->begin();
    if (fabs(ijet->y()) > mMaxY or (ijet->pt() < (jetNo<3 ? mMinPFPtThirdJet : mMinPFPt))) continue;
    
    QCDPFJet qcdpfjetchs;
    double scale = 1./ijet->jecFactor(0); // --- the value of the JEC factor
          
    //---- vertex association -----------
    float sumTrkPt(0.0);
    //---- Practical parameters for  jets
    float sumTrkPtBetaPrime(0.0),sumTrkPtBeta(0.0),sumTrkPtBetaStar(0.0);
    
    // Dunno how useful these are in chs jets...
    int mpuTrk(0), mlvTrk(0); // # of pile-up tracks & lead-vertex tracks ## Juska
    int mjtTrk(0); // multiplicity of _all_ tracks in jet (also vtx-unassociated!) ## Juska
  
    auto PFCandidates = ijet->getPFConstituents();
    //---- loop over the tracks of the jet (take the track associated to jet constituent) ----
    if (trk2vtx.size()>0) {
      vector<reco::TrackRef> usedtrk;
      for (auto &cand : PFCandidates) {
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
          sumTrkPt += trkPt;
          mjtTrk++; //Juska

          // Search for the vertex associated with this track
          auto vtxit = trk2vtx.find(trkRef);
          if (vtxit!=trk2vtx.end()) {
            int currvtx = vtxit->second;
            if (currvtx == 0) {
              sumTrkPtBeta += trkPt;
              mlvTrk++; //Juska
            } else {
              sumTrkPtBetaStar += trkPt;
              mpuTrk++; //Juska
            }
          } // if: track has vertex
        } // if: valid track
      } // for: all chs jet particles

      // A routine for hunting the betastar removed by chs (betaprime)
      auto vTrks(ijet->associatedTracks());
      for (auto i_trkchs = vTrks.begin(); i_trkchs != vTrks.end(); i_trkchs++) {
        // Skip tracks associated with chs jets
        if (std::find(usedtrk.begin(),usedtrk.end(),*i_trkchs)==usedtrk.end()) {
          // Take tracks associated with a vertex other than zero
          auto vtxit = trk2vtx.find(*i_trkchs);
          if (vtxit!=trk2vtx.end() and vtxit->second != 0)
              sumTrkPtBetaPrime += (*i_trkchs)->pt();
        }
      } // for: all tracks (non-chs included)
    } // if: tracks associated with vertices > 0

    qcdpfjetchs.setBeta(sumTrkPt ? sumTrkPtBeta/sumTrkPt : 0.0);
    qcdpfjetchs.setBetaStar(sumTrkPt ? sumTrkPtBetaStar/sumTrkPt : 0.0);
    qcdpfjetchs.setBetaPrime(sumTrkPt ? sumTrkPtBetaPrime/sumTrkPt : 0.0);
  
    //---- jec uncertainty --------------
    double unc(0.0);
    vector<float> uncSrc(0);
    if (mPFPayloadName != "") {
      mPFUnc->setJetEta(ijet->eta());
      mPFUnc->setJetPt(ijet->pt());
      unc = mPFUnc->getUncertainty(true);
    } // if (mPFPayloadName != "")
    if (mPFJECUncSrc != "") {
      for(unsigned isrc=0;isrc<mPFJECUncSrcNames.size();isrc++) {
        mPFUncSrc[isrc]->setJetEta(ijet->eta());
        mPFUncSrc[isrc]->setJetPt(ijet->pt());
        float unc1 = mPFUncSrc[isrc]->getUncertainty(true);
        uncSrc.push_back(unc1);
      } // for(unsigned isrc=0;isrc<mPFJECUncSrcNames.size();isrc++)
    } // if (mPFJECUncSrc != "")
    
    qcdpfjetchs.setP4(ijet->p4());
    qcdpfjetchs.setCor(scale);
    qcdpfjetchs.setUnc(unc);
    qcdpfjetchs.setUncSrc(uncSrc);
    qcdpfjetchs.setArea(ijet->jetArea());
    
    double chf   = ijet->chargedHadronEnergyFraction();
    double nhf   = ijet->neutralHadronEnergyFraction();// hf info is included
    double nemf  = ijet->neutralEmEnergyFraction(); // equals to deprecated phf but has HF info too
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

    qcdpfjetchs.setLooseID(looseID);
    qcdpfjetchs.setTightID(tightID);
    qcdpfjetchs.setFrac(chf,nhf,nemf,cemf,muf);
    qcdpfjetchs.setMulti(npr,chm,nhm,phm,elm,mum,cm);
    qcdpfjetchs.setHFFrac(hf_hf,hf_phf);
    qcdpfjetchs.setHFMulti(hf_hm,hf_phm);
    
    double hof = ijet->hoEnergyFraction(); // Juska
    qcdpfjetchs.setVtxInfo(mpuTrk,mlvTrk,mjtTrk);
    qcdpfjetchs.setHO(hof);

    //if (ijet == patjetschs->begin()) {
    //  auto pdisc = ijet->getPairDiscri();
    //  cout << "Disc" << endl;
    //  for (auto &disc : pdisc) cout << "  " << disc.first << endl;
    //}

    // Jet flavour tagging discriminators
    qcdpfjetchs.pfBoosted_ = ijet->bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags");
    qcdpfjetchs.pfCombinedCvsL_ = ijet->bDiscriminator("pfCombinedCvsLJetTags");
    qcdpfjetchs.pfCombinedCvsB_ = ijet->bDiscriminator("pfCombinedCvsBJetTags");

    qcdpfjetchs.pfDeepCSVb_  = ijet->bDiscriminator("pfDeepCSVJetTags:probb");
    qcdpfjetchs.pfDeepCSVc_  = ijet->bDiscriminator("pfDeepCSVJetTags:probc");
    qcdpfjetchs.pfDeepCSVl_  = ijet->bDiscriminator("pfDeepCSVJetTags:probudsg");
    qcdpfjetchs.pfDeepCSVbb_ = ijet->bDiscriminator("pfDeepCSVJetTags:probbb");

    //if (mRunYear!="2016") {
    //  qcdpfjetchs.pfDeepFlavourb_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probb");
    //  qcdpfjetchs.pfDeepFlavourc_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probc");
    //  qcdpfjetchs.pfDeepFlavourg_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probg");
    //  qcdpfjetchs.pfDeepFlavourl_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probuds");
    //  qcdpfjetchs.pfDeepFlavourbb_ = ijet->bDiscriminator("pfDeepFlavourJetTags:probbb");
    //}

    qcdpfjetchs.pfBTag_JetProb_ = ijet->bDiscriminator("pfJetProbabilityBJetTags");
    qcdpfjetchs.pfBTag_CombInclSecVtxV2_ = ijet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
    qcdpfjetchs.pfBTag_CombMVAV2_ = ijet->bDiscriminator("pfCombinedMVAV2BJetTags");
    
    qcdpfjetchs.setQGTagger((mAK4 ? ijet->userFloat("QGTaggerAK4PFCHS:qgLikelihood") : -100 )); 
    
    qcdpfjetchs.SetPUJetId((ijet->hasUserFloat(mPFJetPUID) ? ijet->userFloat(mPFJetPUID) : -999));
    
    if (mIsMCarlo) {
      float partonFlavour=0;
      float partonFlavourPhysicsDef=0;
      float hadronFlavour=0;
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
        qcdpfjetchs.setGen(genjets->at(imin).p4(),rmin);
        // Patch the PF jet flavours
        if (partonFlavour==0) partonFlavour = genFlavour[imin];
        if (partonFlavourPhysicsDef==0) partonFlavourPhysicsDef = genFlavourPhys[imin];
        if (hadronFlavour==0) hadronFlavour = genFlavourHadr[imin];
      } else {
        // Empty gen jet if no match
        qcdpfjetchs.setGen(LorentzVector(0,0,0,0),0);
      }
      qcdpfjetchs.setFlavour(partonFlavour,hadronFlavour,partonFlavourPhysicsDef);
    } else {
      // Empty gen jet for data
      qcdpfjetchs.setGen(LorentzVector(0,0,0,0),0);
      qcdpfjetchs.setFlavour(-999,-999,-999);
    }
    mPFJets.push_back(qcdpfjetchs);
  } // for: chs Jets
  sort(mPFJets.begin(),mPFJets.end(),sort_pfjets);
  mEvent->setPFJetsCHS(mPFJets); // -- later substitute chs jets
  
  //---------------- MET ---------------------------------------------
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


/////////////// Matching Flavour (deprecated - corresponds to algo flav) ////

int ProcessedTreeProducerBTag::getMatchedPartonGen(edm::Event const& event,GenJetCollection::const_iterator i_gen)
{
  int jetFlavour=-100;
  bool switchB=false;
  bool switchC=false;

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
