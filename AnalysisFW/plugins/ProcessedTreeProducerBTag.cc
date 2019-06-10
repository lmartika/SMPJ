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
  // Trigger
  mDiscardFilter(                                                                           cfg.getUntrackedParameter<bool>("discardFilter",true)),
  mTrigSimple(                                                                              cfg.getUntrackedParameter<bool>("trigSimple",false)),
  mTrigObjs(                                                                                cfg.getUntrackedParameter<bool>("trigObjs",false)),
  mProcessName(                                                                      cfg.getUntrackedParameter<std::string>("processName","")),
  mFilterNames(                                                                 cfg.getParameter<std::vector<std::string> >("filterName")),
  mTriggerNames(                                                                cfg.getParameter<std::vector<std::string> >("triggerName")),
  mTriggerNamesFlw(                                                             cfg.getParameter<std::vector<std::string> >("triggerFollow")),
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
{
  if (mAK4) {
    mQGLToken = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "qgLikelihood"));
    mQGAx2Token = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "axis2"));
    mQGMulToken = consumes<edm::ValueMap<int>>(edm::InputTag("QGTagger", "mult"));
    mQGPtDToken = consumes<edm::ValueMap<float>>(edm::InputTag("QGTagger", "ptD"));
  }
  mTree = fs->make<TTree>("ProcessedTree","ProcessedTree");
  mEvent = new QCDEvent();
  mTree->Branch("events","QCDEvent",&mEvent);
  mTriggerNamesHisto = fs->make<TH1F>("TriggerNames","TriggerNames",1,0,1);
  mTriggerNamesHisto->SetBit(TH1::kUserContour);  
  mTriggerPassHisto = fs->make<TH1F>("TriggerPass","TriggerPass",1,0,1);
  mTriggerPassHisto->SetBit(TH1::kUserContour);
  mFilterActiveHisto = fs->make<TH1F>("FilterActive","FilterActive",1,0,1); 
  mFilterActiveHisto->SetBit(TH1::kUserContour);
  mULimCEF = 0; mULimNEF = 0; mLLimNEF = 0; mULimNHF = 0; mLLimNHF = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::beginJob() {
  // Jet ID settings. General page:
  // https://twiki.cern.ch/twiki/bin/view/CMS/JetID#Recommendations_for_13_TeV_2018
  // These should be checked from time to time.
  // We prefer using (Tight)LepVeto, which is the most conservative JetID.
  if (mRunYear=="2016") {
    // See, https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVRun2016
    mULimCEF = 0.90;
    mLLimNEF = 0.01;
    mULimNEF = 1.01; // Dummy value
    mLLimNHF = -0.01; // Dummy value
    mULimNHF = 0.98;
  } else if (mRunYear=="2017") {
    // See, https://twiki.cern.ch/twiki/bin/view/CMS/JetID13TeVRun2017
    mULimCEF = 0.80;
    mLLimNEF = 0.02;
    mULimNEF = 0.99;
    mLLimNHF = 0.02;
    mULimNHF = 1.01; // Dummy value
  } else if (mRunYear=="2018") {
    mULimCEF = 0.80;
    mLLimNEF = 0.02;
    mULimNEF = 0.99;
    mLLimNHF = 0.20;
    mULimNHF = 1.01; // Dummy value
  }
  cout << "Run year " << mRunYear << " using the following JetID limit parameter values:" << endl;
  cout << "Up cef " << mULimCEF << endl;
  cout << "Up nef " << mULimNEF << endl;
  cout << "Up nhf " << mULimNHF << endl;
  cout << "Lo nef " << mLLimNEF << endl;
  cout << "Lo nhf " << mLLimNHF << endl;
  // Generic boolean indicators
  mSatisfactory = true;
  mNewTrigs = false;
  mRedoPhysDef = false;
  mRedoAlgoDef = false;
  if (mIsMCarlo) {
    if (mMCType==1) { // Herwig++
      if (mRunYear=="2016") {
        mRedoPhysDef = true;
        mRedoAlgoDef = true;
      }
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::endJob() {
  cout << "Happily finished processing! :)" << endl;
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::beginRun(edm::Run const & iRun, edm::EventSetup const& iSetup) {
  bool changed(true);
  if (!(mHLTConfig.init(iRun,iSetup,mProcessName,changed) and mHLTPrescale.init(iRun, iSetup, mProcessName, changed))) {
    cout << "ProcessedTreeProducerBTag::analyze: config extraction failure with process name " << mProcessName << endl;
    return;
  }
  if (!changed) return;
  cout << "Running on global tag " << mHLTConfig.globalTag() << "!!!" << endl;
  if (mIsMCarlo) return;

  // Additional steps to be performed first time in the analysis loop!
  mNewTrigs = true;
  cout << "New trigger menu found!!!" << endl;

  // The triggers that are actively and passively monitored will be set up according to the wishes of the user.
  // mTriggerNames(Flw) is the trigger name list set by the user.
  // We check mHLTConfig, which triggers of these are actually present.
  // Usage example for the passively monitored triggers: when AK4 and AK8 jets are saved into separate files, we still want the same events to be available in both.
  mSatisfactory = mSatisfactory and trigUpdate(mTriggerNames,mTriggerIndex,true) and trigUpdate(mTriggerNamesFlw,mTriggerIndexFlw,false); 

  // Save MET filter pass information
  mFilterActiveHisto->Fill("PassAll",0);
  for (auto &fName : mFilterNames) {
    mFilterActiveHisto->Fill(fName.c_str(),0);
  }

  if (mPrintTriggerMenu) {
    cout << "Available TriggerNames are: " << endl;
    mHLTConfig.dump("Triggers");
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
bool ProcessedTreeProducerBTag::trigUpdate(const vector<string> &tNames, vector<int> &tIndex, bool active) {
  tIndex.clear();
  const auto &hltNames = mHLTConfig.triggerNames();
  const unsigned int nHLTs = mHLTConfig.size();
  assert(hltNames.size()==nHLTs);

  if (mTrigSimple and active) {
    mTriggerNamesIndexMap.clear();
    mTriggerNamesMap.clear();
    mTriggerIndexMap.clear();
  }
  for (const auto &tName : tNames) {
    // For active triggers, we will add the base information in any case.
    if (active) {
      mTriggerPassHisto->Fill(tName.c_str(),0);
      mTriggerNamesHisto->Fill(tName.c_str(),0);
    }
    if (mTrigSimple) {
      // Create a temporary string and strip the zero from the end, in order to use this with regex.
      string tmpName = tName;
      tmpName.pop_back();
      regex trgversions(Form("%s([0-9]*)",tmpName.c_str()));
      if (tName.back()!='0' and tmpName.back()!='v') {
        cout << "In the 'simpleTrigs = True' mode the trigger names should end with 'v0'! Please correct!" << endl;
        return false;
      }
      // Loop through all the hlt names with any version number matching the generic trigger type.
      int counter = 0;
      for (const auto &hName : hltNames) {
        if (regex_match(hName,trgversions)) {
          ++counter;
          if (mTriggerNamesMap.count(tName)==0) {
            mTriggerNamesMap[tName] = vector<string>();
            mTriggerIndexMap[tName] = vector<int>();
          }

          mTriggerNamesIndexMap[hName] = tIndex.size();
          mTriggerNamesMap[tName].emplace_back(hName); 
          mTriggerIndexMap[tName].emplace_back(mHLTConfig.triggerIndex(hName)); 
          if (active) mTriggerNamesHisto->Fill(tName.c_str(),1);
          cout << (active ? "Actively" : "Passively") << " monitored trigger " << hName << " " << mTriggerIndexMap[tName].back() << " exists" << endl;
        }
      }
      tIndex.push_back(counter==0 ? -1 : counter);
    } else {
      unsigned tIdx = mHLTConfig.triggerIndex(tName);
      bool accept = tIdx < nHLTs;
      cout << (active ? "Actively" : "Passively") << " monitored trigger " << tName << " " << tIdx << " ";
      cout << (accept ? "exists" : "does not exist in the current menu") << endl; 
      tIndex.push_back(accept ? tIdx : -1);
      if (active and accept) mTriggerNamesHisto->Fill(tName.c_str(),1);
    }
  }
  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
bool ProcessedTreeProducerBTag::trigCheck(const edm::TriggerNames &names, const vector<string> &tNames, vector<int> &tIndex, bool active) {
  // Go trough all monitored triggers and check that these have the same names.
  assert(tIndex.size()==tNames.size());
  for (auto itrig = 0u; itrig<tIndex.size(); ++itrig) {
    int tIdx = tIndex[itrig];
    if (tIdx<0) continue; // Trigger not monitored or present, OK!
    const string &tName = tNames[itrig];

    if (mTrigSimple) {
      const auto &tNames2 = mTriggerNamesMap[tName];
      const auto &tIndex2 = mTriggerIndexMap[tName];
      assert(tNames2.size()==tIndex2.size());
      for (unsigned itrig2 = 0; itrig2 < tIndex2.size(); ++itrig2) {
        const string &tName2  = tNames2[itrig2];
        const auto &tIdx2 = tIndex2[itrig2];
        if (tName2!=names.triggerName(tIdx2)) {
          cout << "Mismatch in trigger names: " << tName << " " << tName2 << endl;
          return false;
        }
      }
    } else {
      const auto &tName2 = names.triggerName(tIdx);
      if (tName != tName2) {
        cout << "Mismatch in trigger names: " << tName << " " << tName2 << endl;
        return false;
      }
    }
  }
  cout << (active ? "Actively" : "Passively") << " monitored triggers checked!" << endl;
  return true;
}
//////////////////////////////////////////////////////////////////////////////////////////
void ProcessedTreeProducerBTag::analyze(edm::Event const& event, edm::EventSetup const& iSetup) {
  // If the trigger information is not satisfactory, we should not enter the event analysis.
  if (!mSatisfactory) return;

  vector<QCDPFJet>      qPFJets;
  QCDEventHdr           qEvtHdr;
  QCDMET                qPFMet_t1, qPFMet_t0, qPFMet_t0t1;
  
  //-------------- Basic Event Info ------------------------------
  qEvtHdr.setRun(event.id().run());
  qEvtHdr.setEvt(event.id().event());
  qEvtHdr.setLumi(event.luminosityBlock());
  qEvtHdr.setBunch(event.bunchCrossing());
  float refR = 0.4;
  if (!mAK4) refR = 0.8;
  
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

    // Fetching data using tokens.
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
    
    // Update the filter positions only when the trigger menu has changed.
    // These steps cannot be taken within "beginRun", as event.triggerNames() is not available there. 
    if (mNewTrigs) {
      mNewTrigs = false;

      mFilterIndex.clear();
      for (auto &flt : mFilterNames) { // Loop through the requested filter names
        int fltIdx = -1;
        for (unsigned int iflt=0; iflt<filterBits->size(); ++iflt) { // Try to find this
          string filterName = filterNames.triggerName(iflt);
          if (filterName==flt) {
            fltIdx = iflt;
            break;
          }
        }
        mFilterIndex.push_back(fltIdx);
        cout << "MET filter " << flt;
        if (fltIdx==-1) cout << " not found! :(" << endl;
        else cout << " found! :)" << endl;
      }

      // When the triggers are changed, check that the info we got from hltConfig matches with that from event.triggerNames
      if (!trigCheck(names,mTriggerNames,mTriggerIndex,true) or !trigCheck(names,mTriggerNamesFlw,mTriggerIndexFlw,false)) {
        mSatisfactory = false;
        return;
      }
    }

    // Go through the filters and check that all of them are good
    assert(mFilterIndex.size()==mFilterNames.size());
    bool passMET = true;
    for (auto iflt = 0u; iflt<mFilterIndex.size(); ++iflt) {
      int fltIdx = mFilterIndex[iflt];
      if (fltIdx<0) continue; // If a filter was not found, we skip it 
      
      string fName1 = mFilterNames[iflt];
      string fName2 = filterNames.triggerName(fltIdx);
      if (fName1!=fName2) {
        cout << "Mismatch in filter names: " << fName1 << " " << fName2 << endl;
        mSatisfactory = false;
        return;
      }
      if (!filterBits->accept(fltIdx)) {
        passMET = false;
        mFilterActiveHisto->Fill(fName1.c_str(),1);
      }
    }
    if (passMET) mFilterActiveHisto->Fill("PassAll",1);
    // A filter is allowed to reject the event
    if (mDiscardFilter and !passMET) return;

    // Trigger counts
    unsigned fire = 0;
    // Primary triggers
    for (auto itrig = 0u; itrig<mTriggerNames.size(); ++itrig) {
      int tIdx = mTriggerIndex[itrig]; 
      if (tIdx<0) continue; // We skip the triggers not present (perfectly normal!)
      const string &tName = mTriggerNames[itrig];

      if (mTrigSimple) {
        const auto &tIndex2 = mTriggerIndexMap[tName];
        for (unsigned itrig2 = 0; itrig2 < tIndex2.size(); ++itrig2) {
          const auto &tIdx2 = tIndex2[itrig2];
          if (triggerBits->accept(tIdx2)) {
            L1Prescales.push_back(max(triggerPrescalesL1Max->getPrescaleForIndex(tIdx2),triggerPrescalesL1Min->getPrescaleForIndex(tIdx2)));
            HLTPrescales.push_back(triggerPrescales->getPrescaleForIndex(tIdx2));
            mTriggerPassHisto->Fill(tName.c_str(),1);
            Fired.push_back(itrig);
            ++fire;
            if (mTrigObjs and vvHLT.find(tName)==vvHLT.end()) vvHLT[tName] = vector<LorentzVector>();
          }
        } 
      } else {
        if (triggerBits->accept(tIdx)) {
          L1Prescales.push_back(max(triggerPrescalesL1Max->getPrescaleForIndex(tIdx),triggerPrescalesL1Min->getPrescaleForIndex(tIdx)));
          HLTPrescales.push_back(triggerPrescales->getPrescaleForIndex(tIdx));
          mTriggerPassHisto->Fill(tName.c_str(),1);
          Fired.push_back(itrig);
          ++fire;
          if (mTrigObjs and vvHLT.find(tName)==vvHLT.end()) vvHLT[tName] = vector<LorentzVector>();
        }
      }
    }
    // When the primary trigger has not fired, check if a "followed" secondary trigger has fired
    if (fire==0) {
      bool nofireoth = true;
      for (auto itrig = 0u; itrig<mTriggerIndexFlw.size(); ++itrig) {
        int tIdx = mTriggerIndexFlw[itrig]; 
        if (tIdx<0) continue; // Skip the triggers not present (perfectly normal!)

        if (mTrigSimple) {
          const auto &tIndex2 = mTriggerIndexMap[mTriggerNamesFlw[itrig]];
          for (unsigned itrig2 = 0; itrig2 < tIndex2.size(); ++itrig2) {
            const auto &tIdx2 = tIndex2[itrig2];
            if (triggerBits->accept(tIdx2)) {
              nofireoth = false;
              break;
            }
          }
          if (!nofireoth) break;
        } else {
          if (triggerBits->accept(tIdx)) {
            nofireoth = false;
            break;
          }
        }
      }
      // If none of the actively/passively monitored triggers have fired, skip this as a redundant event
      if (nofireoth) return;
    }

    // The trigger objects are saved only when the actively monitored trigger has fired (in ZB events we pay no attention)
    if (mTrigObjs and !mZB and fire>0) {
      // HLT sector
      regex pfjet(Form("HLT_%sPFJet([0-9]*)_v([0-9]*)",mAK4 ? "" : "AK8"));
      for (pat::TriggerObjectStandAlone obj : *triggerHLTObjs) { // note: not "const &" since we want to call unpackPathNames
        obj.unpackPathNames(names);
        vector<string> pathNamesAll  = obj.pathNames(false);
        vector<string> pathNamesLast = obj.pathNames(true);
        if (pathNamesAll.size()==0) continue; 
        
        for (unsigned hpn = 0, npn = pathNamesAll.size(); hpn < npn; ++hpn) {
          string tName = pathNamesAll[hpn];
          if (regex_match(tName,pfjet)) {
            TLorentzVector P4;
            P4.SetPtEtaPhiM(obj.pt(),obj.eta(),obj.phi(),obj.mass());
            string refName = tName;
            if (mTrigSimple) {
              if (mTriggerNamesIndexMap.count(tName)==0) {
                cout << "Trigger name not found in trigger name map!" << endl;
                mSatisfactory = false;
                return;
              }
              unsigned refIdx = mTriggerNamesIndexMap[tName];
              if (refIdx<mTriggerNames.size()) {
                refName = mTriggerNames[refIdx];
              } else {
                cout << "Trigger index too big in trigger name map!" << endl;
                mSatisfactory = false;
                return;
              }
            }
            vvHLT[refName].emplace_back(P4.Px(),P4.Py(),P4.Pz(),P4.E());
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
      mGenFlavour.push_back(aInfo.getPartonFlavour());
      mGenFlavourHadr.push_back(aInfo.getHadronFlavour());
      mGenFlavourPhys.push_back(bInfo.getPartonFlavour());
    }
    map<int,float> jet2bpt;
    // Loop through all prunder particles and then the jets
    for (auto gidx = 0u; gidx < mGenParts->size(); ++gidx) {
      auto &gPart = mGenParts->at(gidx);
      int absId = abs(gPart.pdgId());
      if (absId<100) continue; 
      if (is_bhadr(absId)) {
        int imin = -1;
        float rmin = -1.;
        std::tie(imin,rmin) = best_jet4prtn(gPart);
        if (rmin<refR and (jet2bpt.find(imin)==jet2bpt.end() or jet2bpt[imin]<gPart.pt()))
          jet2bpt[imin] = gPart.pt();
      }
    }
    // Fill the best b hadron pt
    for (unsigned igen = 0; igen < mGenJets.size(); ++igen) {
      if (jet2bpt.find(igen)==jet2bpt.end()) mGenBPt.push_back(-1.);
      else mGenBPt.push_back(jet2bpt[igen]);
    }
    // Hand-made flavor definitions for when the built-in methods fail
    if (mRedoPhysDef) {
      flavs_physdef();
      for (auto &res : mGenJetPhysFlav) {
        int igen = res.first;
        if (mGenFlavourPhys[igen]==0 and res.second!=0)
          mGenFlavourPhys[igen] = res.second;
      }
    }
    if (mRedoAlgoDef) {
      flavs_newalgodef(); 
      for (auto &res : mGenJetAlgoFlav) {
        int igen = res.first;
        if (mGenFlavour[igen]==0)
          mGenFlavour[igen] = res.second;
      }
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
  if (mAK4) {
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
      if (dRMin<refR) jet2pu[bestjet].push_back(cidx);
    }
  }

  // QG stuff needs to be taken as a separate entity for MiniAOD
  edm::Handle<edm::ValueMap<float>> qglHandle, qgax2Handle, qgptdHandle;
  edm::Handle<edm::ValueMap<int>> qgmulHandle;
  if (mAK4) {
    event.getByToken(mQGLToken, qglHandle);
    event.getByToken(mQGAx2Token, qgax2Handle);
    event.getByToken(mQGMulToken, qgmulHandle);
    event.getByToken(mQGPtDToken, qgptdHandle);
  }
  
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

    if (mAK4) {
      // Loop through the PF candidates within the jet.
      vector<double> used;
      for (auto pidx = 0u; pidx < ijet->numberOfDaughters(); ++pidx) {
        auto dtr = dynamic_cast<const pat::PackedCandidate*>(ijet->daughter(pidx));
        if (dtr->charge()!=0) {
          che += dtr->energy();
          
          ++mjtTrk; 
          if (dtr->fromPV()==0) {
            // Note: dtr->pvAssociationQuality() is the modern alternative, but fromPV is the one used for CHS.
            // Still for some reason, not all fromPV==0 cases are removed. These events fit the old "betaStar" definition (not-from-PV).
            // Due to CHS, the trailing betaStar is a vanishing fraction (1/10k), so we don't store it anymore.
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
    }

    QCDPFJet qcdJet;

    float bPrime = (che ? pue/che : 0.);
    qcdJet.setBetaPrime(bPrime);
     
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
    bool looseID = true, tightID = true;
    if (abseta <= 2.7) {
      tightID = nhf<0.90 and muf<0.80 and ((mRunYear=="2018") ?
                (chm>0 and cemf<mULimCEF and nemf<0.99 and (abseta>=2.6 or (nemf<0.90 and npr>1 and chf>0))) :
                (npr>1 and nemf<0.90 and (abseta>=2.4 or (chf>0 and chm>0 and cemf<mULimCEF))));
      looseID = (mRunYear=="2016") ? (npr>1 and nemf<0.99 and nhf<0.99 and (abseta>2.4 or (chf>0 and chm>0 and cemf<0.99))) : tightID; 
    } else if (abseta <= 3.0) {
      tightID = nemf<mULimNEF and nemf>mLLimNEF and nm>2 and nhf<mULimNHF;
      looseID = tightID;
    } else {
      tightID = nemf<0.90 and nm>10 and nhf>mLLimNHF;
      looseID = tightID;
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
    
    // Check what is available
    //auto pdisc = ijet->getPairDiscri();
    //for (auto &disc : pdisc) cout << "  " << disc.first << endl;

    // Jet flavour tagging discriminators
    qcdJet.pfCombinedCvsL_ = ijet->bDiscriminator("pfCombinedCvsLJetTags");
    qcdJet.pfCombinedCvsB_ = ijet->bDiscriminator("pfCombinedCvsBJetTags");

    qcdJet.pfDeepCSVb_  = ijet->bDiscriminator("pfDeepCSVJetTags:probb");
    qcdJet.pfDeepCSVc_  = ijet->bDiscriminator("pfDeepCSVJetTags:probc");
    qcdJet.pfDeepCSVl_  = ijet->bDiscriminator("pfDeepCSVJetTags:probudsg");
    qcdJet.pfDeepCSVbb_ = ijet->bDiscriminator("pfDeepCSVJetTags:probbb");

    qcdJet.pfBTag_JetProb_ = ijet->bDiscriminator("pfJetProbabilityBJetTags");
    qcdJet.pfBTag_CombInclSecVtxV2_ = ijet->bDiscriminator("pfCombinedInclusiveSecondaryVertexV2BJetTags");
    qcdJet.pfBTag_CombMVAV2_ = ijet->bDiscriminator("pfCombinedMVAV2BJetTags");

    float QGL = -1;
    float QGAx2 = -1;
    int QGMul = -1;
    float QGPtD = -1;
    // QGL variables only relevant for AK4
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

      if (imin!=-1 and rmin<refR) {
        qcdJet.setGen(imin,rmin);
        if (mUseGenInfo) {
          // Patch the PF jet flavours if these were not found earlier
          if (partonFlavour==0)     partonFlavour     = mGenFlavour[imin]; 
          if (partonFlavourPhys==0) partonFlavourPhys = mGenFlavourPhys[imin];
          if (hadronFlavour==0)     hadronFlavour     = mGenFlavourHadr[imin];
          // Patch the gen flavours if these were not found earlier
          if (mGenFlavour[imin]==0)     mGenFlavour[imin]     = partonFlavour;
          if (mGenFlavourPhys[imin]==0) mGenFlavourPhys[imin] = partonFlavourPhys;
          if (mGenFlavourHadr[imin]==0) mGenFlavourHadr[imin] = hadronFlavour;
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
  if (qPFJets.size()<mMinNPFJets) return;

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
  auto mett0 = mett1.corP2(pat::MET::RawChs);
  qPFMet_t0.setVar(mett0.pt(),mett1.corSumEt(pat::MET::RawChs),mett0.phi());
  // The old T0 for 8029
  //auto mett0 = mett1.corP2(pat::MET::Type0);
  //qPFMet_t0.setVar(mett0.pt(),mett1.corSumEt(pat::MET::Type0),mett0.phi());
  // MET T0T1 is found using standard routines
  auto mett0t1 = mett1.corP2(pat::MET::Type01);
  qPFMet_t0t1.setVar(mett0t1.pt(),mett1.corSumEt(pat::MET::Type01),mett0t1.phi());
  mEvent->setPFMET(qPFMet_t1,qPFMet_t0,qPFMet_t0t1);
 
  //-------------- fill the tree -------------------------------------
  mTree->Fill();
}

//////////////////////////////////////////////////////////////////////////////////////////
DEFINE_FWK_MODULE(ProcessedTreeProducerBTag);
