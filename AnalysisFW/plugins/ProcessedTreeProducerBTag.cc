#include "SMPJ/AnalysisFW/plugins/ProcessedTreeProducerBTag.h"

ProcessedTreeProducerBTag::ProcessedTreeProducerBTag(edm::ParameterSet const& cfg):
  mAK4(cfg.getUntrackedParameter                                                                                     <bool>("AK4",false)),
  mPrintTriggerMenu(                                                                        cfg.getUntrackedParameter<bool>("printTriggerMenu",false)),
  mIsPFJecUncSet(                                                                                                        false),
  // Cut params
  mGoodVtxNdof(                                                                                    cfg.getParameter<double>("goodVtxNdof")),
  mGoodVtxZ(                                                                                       cfg.getParameter<double>("goodVtxZ")),
  mMinNPFJets(                                                                               cfg.getParameter<unsigned int>("minNPFJets")),
  mMinPFPt(                                                                                        cfg.getParameter<double>("minPFPt")),
  mMinPFPtThirdJet(                                                                                cfg.getParameter<double>("minPFPtThirdJet")),
  mMinGenPt(                                                                              cfg.getUntrackedParameter<double>("minGenPt",20)),
  mMaxEta(                                                                                         cfg.getParameter<double>("maxEta")),
  // Misc
  mPFPayloadName(                                                                             cfg.getParameter<std::string>("PFPayloadNameCHS")),
  mRunYear(                                                                          cfg.getUntrackedParameter<std::string>("runYear","2016")),
  mPFJetPUID(                                                                                 cfg.getParameter<std::string>("pfchsjetpuid")),
  mPFJECUncSrc(                                                                      cfg.getUntrackedParameter<std::string>("jecUncSrcCHS","")),
  mPFJECUncSrcNames(                                                            cfg.getParameter<std::vector<std::string> >("jecUncSrcNames")),
  mOfflineVertices(mayConsume<reco::VertexCollection>(                                      cfg.getParameter<edm::InputTag>("offlineVertices"))),
  mBeamSpot(mayConsume<reco::BeamSpot>(                                                     cfg.getParameter<edm::InputTag>("beamSpot"))),
  mPFJetsName(consumes<edm::View<pat::Jet>>(                                                cfg.getParameter<edm::InputTag>("pfjetschs"))),
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
  mTriggerFlwNames(                                                             cfg.getParameter<std::vector<std::string> >("triggerFollow")),
  mTriggerResultsTag(mayConsume<edm::TriggerResults>(                              cfg.getUntrackedParameter<edm::InputTag>("triggerResults",edm::InputTag("")))),
  mTriggerEventTag(mayConsume<trigger::TriggerEvent>(                              cfg.getUntrackedParameter<edm::InputTag>("triggerEvent",edm::InputTag("")))),
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
  bool changed = true;
  if (!(mHLTConfig.init(iRun,iSetup,mProcessName,changed) and mHLTPrescale.init(iRun, iSetup, mProcessName, changed))) {
    cout << "ProcessedTreeProducerBTag::analyze: config extraction failure with process name " << mProcessName << endl;
    return;
  }
  if (!changed) return;
  cout << "Running on global tag " << mHLTConfig.globalTag() << "!!!" << endl;
  if (mIsMCarlo) return;
  cout<<"New trigger menu found !!!"<<endl;

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
    auto trgFlwIdx = mHLTConfig.triggerIndex(trgName);
    cout << "Passively monitored trigger " << mTriggerFlwNames[itrig] << " " << trgFlwIdx << " ";
    if (trgFlwIdx >= n) {
      cout << "does not exist in the current menu" << endl;
      mTriggerFlwIndex.push_back(-1);
    } else {
      cout << "exists" <<endl;
      mTriggerFlwIndex.push_back(trgFlwIdx);
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
  QCDMET                qPFMet_t1, qPFMet_t0pc, qPFMet_t0pct1;

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
    vector<int> L1Prescales,HLTPrescales,Fired;
    vector<vector<LorentzVector> > qL1Objs, qHLTObjs;

    edm::Handle<edm::TriggerResults>   hTriggerResults;
    edm::Handle<trigger::TriggerEvent> hTriggerEvent;

    event.getByToken(mTriggerResultsTag,hTriggerResults);
    event.getByToken(mTriggerEventTag,hTriggerEvent);
    // Sanity checks
    if (!hTriggerResults.isValid()) {
      cout << "Error: in fetching TriggerResults!" << endl;
      return;
    }
    if (!hTriggerEvent.isValid()) {
      cout << "Error in fetching TriggerEvent!" << endl;
      return;
    }
    assert(hTriggerResults->size()==mHLTConfig.size());

    //------ loop over triggers selected and available ---------
    unsigned fire = 0;
    for (auto itrig = 0u; itrig<mTriggerNames.size(); ++itrig) {
      int trgIdx = mTriggerIndex[itrig];
      if (trgIdx<0) continue; // Trigger not present in file, OK!

      auto &trgName = mTriggerNames[itrig];
      vector<LorentzVector> vvL1,vvHLT;
      bool accept = hTriggerResults->accept(trgIdx);
 
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
      if (accept) {
        L1Prescales.push_back(preL1);
        HLTPrescales.push_back(preHLT);
        mTriggerPassHisto->Fill(trgName.c_str(),1);
        Fired.push_back(itrig);
        ++fire;
        // Trigger objects are added if the trigger has fired:
        //--------- modules on this trigger path--------------
        const vector<string>& moduleLabels(mHLTConfig.moduleLabels(trgIdx));
        const unsigned int moduleIndex(hTriggerResults->index(trgIdx));
        bool foundL1(false);
        for (unsigned int j=0; j<=moduleIndex; ++j) {
          const string& moduleLabel(moduleLabels[j]);
          const string  moduleType(mHLTConfig.moduleType(moduleLabel));
          //--------check whether the module is packed up in TriggerEvent product
          const unsigned int filterIndex(hTriggerEvent->filterIndex(InputTag(moduleLabel,"",mProcessName)));
          if (filterIndex<hTriggerEvent->sizeFilters()) {
            const Vids& VIDS(hTriggerEvent->filterIds(filterIndex));
            const Keys& KEYS(hTriggerEvent->filterKeys(filterIndex));
            const size_type nI(VIDS.size());
            const size_type nK(KEYS.size());
            assert(nI==nK);
            const size_type n(max(nI,nK));
            const TriggerObjectCollection& TOC(hTriggerEvent->getObjects());
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
        }
        qL1Objs.push_back(vvL1);
        qHLTObjs.push_back(vvHLT);
      } // Trigger has fired 
    } // Loop over trigger names
    if (fire==0) {
      bool nofireoth = true;
      for (auto itrig = 0u; itrig<mTriggerFlwNames.size() ;++itrig) {
        int trgIdx = mTriggerFlwIndex[itrig];
        if (trgIdx<0) continue; // Skip the triggers not present (perfectly normal!)

        if (hTriggerResults->accept(trgIdx)) {
          nofireoth = false;
          break;
        }
      }
      // If none of the actively/passively monitored triggers have fired, skip this as a redundant event
      if (nofireoth) return;
    }
    mEvent->setTrigDecision(Fired);
    mEvent->setPrescales(L1Prescales,HLTPrescales);
    mEvent->setL1Obj(qL1Objs);
    mEvent->setHLTObj(qHLTObjs);
  } // !isMCarlo

  //-------------- Vertex Info -----------------------------------
  Handle<reco::VertexCollection> recVtxs;
  event.getByToken(mOfflineVertices,recVtxs);

  //------------- reject events without reco vertices ------------
  int VtxGood(0);
  bool isPVgood(false);
  float PVx(0),PVy(0),PVz(0),PVndof(0);
  for (VertexCollection::const_iterator ivtx = recVtxs->begin(); ivtx != recVtxs->end(); ++ivtx) {
    int index = ivtx-recVtxs->begin();
    if (index == 0) {
      PVx    = ivtx->x();
      PVy    = ivtx->y();
      PVz    = ivtx->z();
      PVndof = ivtx->ndof();
    }
    if (!(ivtx->isFake()) and ivtx->ndof() >= mGoodVtxNdof and fabs(ivtx->z()) <= mGoodVtxZ) {
      if (index==0) isPVgood = true;
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

  //-------------- Generator (incl. simulated PU) Info ----------------
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
    qEvtHdr.setPU(nbx,ootpuEarly,ootpuLate,intpu);
    qEvtHdr.setTrPu(Tnpv);
  } else {
    qEvtHdr.setPthat(0);
    qEvtHdr.setWeight(0);
    qEvtHdr.setPU(0,0,0,0);
    qEvtHdr.setTrPu(0);
  }
  mEvent->setEvtHdr(qEvtHdr);

  //---------------- GenJets ------------------------------------------
  if (mUseLegacyTag) event.getByToken(mGenParticles, mGenParts);
  if (mIsMCarlo) {
    // Fetch the collections!
    Handle<GenJetCollection>  genJets;
    event.getByToken(mGenJetsName,genJets);
    edm::Handle<reco::JetFlavourInfoMatchingCollection> theJetFlavourInfos;
    event.getByToken(mJetFlavourInfosToken, theJetFlavourInfos );
    edm::Handle<reco::JetFlavourInfoMatchingCollection> theJetFlavourInfosPhysicsDef;
    event.getByToken(mJetFlavourInfosTokenPhysicsDef, theJetFlavourInfosPhysicsDef );
    // Genjet loop
    auto igen = genJets->begin();
    auto j = theJetFlavourInfos->begin();
    auto k = theJetFlavourInfosPhysicsDef->begin();
    for (;igen!=genJets->end() and j!=theJetFlavourInfos->end() and k!=theJetFlavourInfosPhysicsDef->end();++igen,++j,++k) {
      if (fabs(igen->eta()) > mMaxEta) continue;

      auto aInfo = j->second;
      auto bInfo = k->second;

      mGenJets.push_back(igen->p4());
      mGenFlavour.push_back((mUseLegacyTag ? get_gjetpartonflav(event,igen) : aInfo.getPartonFlavour()));
      mGenFlavourHadr.push_back(aInfo.getHadronFlavour());
      mGenFlavourPhys.push_back(bInfo.getPartonFlavour());
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

  //----------- PFJets   part -------------------------
  edm::Handle<edm::View<pat::Jet> > patJets;
  event.getByToken(mPFJetsName,patJets);

  // A store for track-vertex association
  map<reco::TrackRef,int> trk2vtx;
  for(unsigned ivtx = 0; ivtx < recVtxs->size(); ivtx++) {
    if (!((*recVtxs)[ivtx].isFake()) and (*recVtxs)[ivtx].ndof() >= mGoodVtxNdof && fabs((*recVtxs)[ivtx].z()) <= mGoodVtxZ) {
      for (auto ivtxTrk = (*recVtxs)[ivtx].tracks_begin(); ivtxTrk != (*recVtxs)[ivtx].tracks_end(); ++ivtxTrk) {
        // Match the chsjet track to the track from the vertex
        reco::TrackRef trkRef(ivtxTrk->castTo<reco::TrackRef>());
        trk2vtx[trkRef] = ivtx;
      } // Vertex track loop
    } // Good vertex
  } // Vertex loop
    
  // Jet loop
  int maxGenMatch = -1;
  for (auto ijet = patJets->begin(); ijet != patJets->end(); ++ijet) { 
    if (!ijet->isPFJet()) continue;

    // Preselection
    int jetNo = ijet-patJets->begin();
    if (fabs(ijet->eta()) > mMaxEta or (ijet->pt() < (jetNo<3 ? mMinPFPtThirdJet : mMinPFPt))) continue;
    
    QCDPFJet qcdJet;
    double scale = 1./ijet->jecFactor(0);
          
    //---- vertex association -----------
    float sumTrkPt(0.0);
    //---- Practical parameters for  jets
    float sumTrkPtBetaPrime(0.0), sumTrkPtBeta(0.0), sumTrkPtBetaStar(0.0);
    
    // Track parameters by Juska. Might not be useful for chs jets.
    int mpuTrk(0), mlvTrk(0); // # of pile-up tracks & lead-vertex tracks ## Juska
    int mjtTrk(0); // multiplicity of _all_ tracks in jet (also vtx-unassociated!) ## Juska
  
    // Loop: tracks of the jet
    auto PFCandidates = ijet->getPFConstituents();
    if (trk2vtx.size()>0) {
      vector<reco::TrackRef> usedtrk;
      // Track fetched from PF Candidate
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

    qcdJet.setBeta(sumTrkPt ? sumTrkPtBeta/sumTrkPt : 0.0);
    qcdJet.setBetaStar(sumTrkPt ? sumTrkPtBetaStar/sumTrkPt : 0.0);
    qcdJet.setBetaPrime(sumTrkPt ? sumTrkPtBetaPrime/sumTrkPt : 0.0);
  
    //---- jec uncertainty --------------
    double unc(0.0);
    vector<float> uncSrc(0);
    if (mPFPayloadName != "") {
      mPFUnc->setJetEta(ijet->eta());
      mPFUnc->setJetPt(ijet->pt());
      unc = mPFUnc->getUncertainty(true);
    } // if (mPFPayloadName != "")
    if (mPFJECUncSrc != "") {
      for(unsigned isrc=0;isrc<mPFJECUncSrcNames.size();++isrc) {
        mPFUncSrc[isrc]->setJetEta(ijet->eta());
        mPFUncSrc[isrc]->setJetPt(ijet->pt());
        float unc1 = mPFUncSrc[isrc]->getUncertainty(true);
        uncSrc.push_back(unc1);
      } // Loop: uncertainty sources
    } // If: uncertainty sources
    
    qcdJet.setP4(ijet->p4());
    qcdJet.setCor(scale);
    qcdJet.setUnc(unc);
    qcdJet.setUncSrc(uncSrc);
    qcdJet.setArea(ijet->jetArea());
    
    double chf   = ijet->chargedHadronEnergyFraction();
    double nhf   = ijet->neutralHadronEnergyFraction();// hf info is included
    double nemf  = ijet->neutralEmEnergyFraction();    // equals to deprecated phf but has HF info too
    double cemf  = ijet->chargedEmEnergyFraction();    // equals to deprecated elf
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
    
    double hof = ijet->hoEnergyFraction(); // Juska
    qcdJet.setVtxInfo(mpuTrk,mlvTrk,mjtTrk);
    qcdJet.setHO(hof);

    //if (ijet == patJets->begin()) {
    //  auto pdisc = ijet->getPairDiscri();
    //  cout << "Disc" << endl;
    //  for (auto &disc : pdisc) cout << "  " << disc.first << endl;
    //}

    // Jet flavour tagging discriminators
    qcdJet.pfBoosted_ = ijet->bDiscriminator("pfBoostedDoubleSecondaryVertexAK8BJetTags");
    qcdJet.pfCombinedCvsL_ = ijet->bDiscriminator("pfCombinedCvsLJetTags");
    qcdJet.pfCombinedCvsB_ = ijet->bDiscriminator("pfCombinedCvsBJetTags");

    qcdJet.pfDeepCSVb_  = ijet->bDiscriminator("pfDeepCSVJetTags:probb");
    qcdJet.pfDeepCSVc_  = ijet->bDiscriminator("pfDeepCSVJetTags:probc");
    qcdJet.pfDeepCSVl_  = ijet->bDiscriminator("pfDeepCSVJetTags:probudsg");
    qcdJet.pfDeepCSVbb_ = ijet->bDiscriminator("pfDeepCSVJetTags:probbb");

    if (mRunYear!="2016") {
      qcdJet.pfDeepFlavourb_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probb");
      qcdJet.pfDeepFlavourc_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probc");
      qcdJet.pfDeepFlavourg_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probg");
      qcdJet.pfDeepFlavourl_  = ijet->bDiscriminator("pfDeepFlavourJetTags:probuds");
      qcdJet.pfDeepFlavourbb_ = ijet->bDiscriminator("pfDeepFlavourJetTags:probbb");
    }

    qcdJet.pfBTag_JetProb_ = ijet->bDiscriminator("pfJetProbabilityBJetTags");
    qcdJet.pfBTag_CombInclSecVtxV2_ = ijet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
    qcdJet.pfBTag_CombMVAV2_ = ijet->bDiscriminator("pfCombinedMVAV2BJetTags");
    
    qcdJet.setQGTagger((mAK4 ? ijet->userFloat("QGTaggerAK4PFCHS:qgLikelihood") : -100 )); 
    qcdJet.SetPUJetId((ijet->hasUserFloat(mPFJetPUID) ? ijet->userFloat(mPFJetPUID) : -999));
    
    if (mIsMCarlo) {
      float partonFlavour=0;
      float partonFlavourPhysicsDef=0;
      float hadronFlavour=0;
      if (mUseGenInfo) {
        partonFlavour = ijet->partonFlavour();
        hadronFlavour = ijet->hadronFlavour();
        if (ijet->genParton() != NULL) partonFlavourPhysicsDef = ijet->genParton()->pdgId();
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
          if (partonFlavourPhysicsDef==0) partonFlavourPhysicsDef = mGenFlavourPhys[imin];
          if (hadronFlavour==0) hadronFlavour = mGenFlavourHadr[imin];
        }
      } else {
        // Empty gen jet if no match
        qcdJet.setGen(-1,-1.);
      }
      qcdJet.setFlavour(partonFlavour,hadronFlavour,partonFlavourPhysicsDef);
    } else {
      // Empty gen jet for data
      qcdJet.setGen(-1,-1.);
      qcdJet.setFlavour(0,0,0);
    }
    qPFJets.push_back(qcdJet);
  } // for: chs Jets
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
      mGenFlavour.pop_back();
      mGenFlavourHadr.pop_back();
      mGenFlavourPhys.pop_back();
    }
    // Save only the interesting gen info
    mEvent->setGenJets(mGenJets);
    mEvent->setGenFlavour(mGenFlavour);
    mEvent->setGenFlavourHadron(mGenFlavourHadr);
    mEvent->setGenFlavourPhysicsDef(mGenFlavourPhys);
  }
  
  //---------------- MET ---------------------------------------------
  Handle<pat::METCollection> pfmet_t1, pfmet_t0pc, pfmet_t0pct1;
  event.getByToken(mPFMETt1, pfmet_t1);
  event.getByToken(mPFMETt0pc, pfmet_t0pc);
  event.getByToken(mPFMETt0pct1, pfmet_t0pct1);
  const pat::MET &met_t1 = pfmet_t1->front();
  qPFMet_t1.setVar(met_t1.et(),met_t1.sumEt(),met_t1.phi());
  const pat::MET &met_t0pc = pfmet_t0pc->front();
  qPFMet_t0pc.setVar(met_t0pc.et(),met_t0pc.sumEt(),met_t0pc.phi());
  const pat::MET &met_t0pct1 = pfmet_t0pct1->front();
  qPFMet_t0pct1.setVar(met_t0pct1.et(),met_t0pct1.sumEt(),met_t0pct1.phi());
  mEvent->setPFMET(qPFMet_t1,qPFMet_t0pc,qPFMet_t0pct1);

  //-------------- fill the tree -------------------------------------
  if (mEvent->nPFJetsCHS() >= mMinNPFJets) mTree->Fill();
}

//////////////////////////////////////////////////////////////////////////////////////////
DEFINE_FWK_MODULE(ProcessedTreeProducerBTag);
