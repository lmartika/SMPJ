# -*- coding: utf-8 -*-

import FWCore.ParameterSet.Config as cms

from RecoJets.Configuration.RecoPFJets_cff import ak4PFJets, ak4PFJetsCHS
from RecoJets.Configuration.RecoGenJets_cff import ak4GenJets
from RecoJets.JetProducers.SubJetParameters_cfi import SubJetParameters
from RecoJets.JetProducers.PFJetParameters_cfi import *
from RecoJets.JetProducers.GenJetParameters_cfi import *
from RecoJets.JetProducers.AnomalousCellParameters_cfi import *
from PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff import *
from PhysicsTools.PatAlgos.selectionLayer1.jetSelector_cfi import selectedPatJets
from PhysicsTools.PatAlgos.tools.jetTools import *
from PhysicsTools.PatAlgos.patSequences_cff import *
#from RecoJets.JetProducers.pileupjetidproducer_cfi import *
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from RecoJets.JetProducers.QGTagger_cfi import QGTagger

## Modified version of jetToolBox from https://github.com/cms-jet/jetToolbox
## Options for PUMethod: Puppi, CS, SK, CHS

# -*- coding: utf-8 -*-
import FWCore.ParameterSet.Config as cms

process = cms.Process("Ntuplizer")
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Conditions
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff")
process.load("Configuration.EventContent.EventContent_cff")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('RecoJets.Configuration.GenJetParticles_cff')
process.load('RecoJets.Configuration.RecoGenJets_cff')
process.load('RecoJets.JetProducers.TrackJetParameters_cfi')
process.load('RecoJets.JetProducers.PileupJetIDParams_cfi')
process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.load('RecoJets.JetProducers.QGTagger_cfi')
process.QGTagger.srcJets = cms.InputTag('ak4PFJetsCHS')    # Could be reco::PFJetCollection or pat::JetCollection (both AOD and miniAOD)
process.QGTagger.jetsLabel  = cms.string('QGL_AK4PFchs')        # Other options: see https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion

process.GlobalTag.globaltag = "94X_dataRun2_ReReco_EOY17_v2"

##-------------------- Import the JEC services -----------------------
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Input
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

inFiles = cms.untracked.vstring('/store/data/Run2017C/JetHT/MINIAOD/31Mar2018-v1/00000/046FD327-5138-E811-AF93-AC162DA6E2F8.root')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(100))
process.source = cms.Source("PoolSource", fileNames = inFiles )

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Services
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.load('CommonTools.UtilAlgos.TFileService_cfi')
process.TFileService.fileName=cms.string('DATA.root')

process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")

process.patJets.addTagInfos = True
process.patJets.addAssociatedTracks = True

#process.out.outputCommands += ['keep *_QGTagger_*_*']
process.load("PhysicsTools.PatAlgos.patSequences_cff")

triggers = cms.vstring( 
  "HLT_PFJet40_v19", "HLT_PFJet60_v19", "HLT_PFJet450_v19", "HLT_PFJet500_v19"
  "HLT_PFJet40_v18", "HLT_PFJet60_v18", "HLT_PFJet80_v18", "HLT_PFJet260_v18", "HLT_PFJet320_v18", "HLT_PFJet400_v18", "HLT_PFJet450_v18", "HLT_PFJet500_v18",
  "HLT_PFJet40_v17", "HLT_PFJet60_v17", "HLT_PFJet80_v17", "HLT_PFJet140_v17", "HLT_PFJet200_v17", "HLT_PFJet260_v17", "HLT_PFJet320_v17", "HLT_PFJet400_v17", "HLT_PFJet450_v17", "HLT_PFJet500_v17",
  "HLT_PFJet40_v16", "HLT_PFJet60_v16", "HLT_PFJet80_v16", "HLT_PFJet140_v16", "HLT_PFJet200_v16", "HLT_PFJet260_v16", "HLT_PFJet320_v16", "HLT_PFJet400_v16", "HLT_PFJet450_v16", "HLT_PFJet500_v16",
  "HLT_PFJet40_v15", "HLT_PFJet60_v15", "HLT_PFJet80_v15", "HLT_PFJet140_v15", "HLT_PFJet200_v15", "HLT_PFJet260_v15", "HLT_PFJet320_v15", "HLT_PFJet400_v15", "HLT_PFJet450_v15", "HLT_PFJet500_v15",
  "HLT_PFJet40_v14", "HLT_PFJet60_v14", "HLT_PFJet80_v14", "HLT_PFJet140_v14", "HLT_PFJet200_v14", "HLT_PFJet260_v14", "HLT_PFJet320_v14", "HLT_PFJet400_v14", "HLT_PFJet450_v14", "HLT_PFJet500_v14",
  "HLT_PFJet40_v13", "HLT_PFJet60_v13", "HLT_PFJet80_v13", "HLT_PFJet140_v13", "HLT_PFJet200_v13", "HLT_PFJet260_v13", "HLT_PFJet320_v13", "HLT_PFJet400_v13", "HLT_PFJet450_v13", "HLT_PFJet500_v13",
  "HLT_PFJet40_v12", "HLT_PFJet60_v12", "HLT_PFJet80_v12", "HLT_PFJet140_v12", "HLT_PFJet200_v12", "HLT_PFJet260_v12", "HLT_PFJet320_v12", "HLT_PFJet400_v12", "HLT_PFJet450_v12", "HLT_PFJet500_v12",
  "HLT_PFJet40_v11", "HLT_PFJet60_v11", "HLT_PFJet80_v11", "HLT_PFJet140_v11", "HLT_PFJet200_v11", "HLT_PFJet260_v11", "HLT_PFJet320_v11", "HLT_PFJet400_v11", "HLT_PFJet450_v11", "HLT_PFJet500_v11",
  "HLT_PFJet40_v10", "HLT_PFJet60_v10", "HLT_PFJet80_v10", "HLT_PFJet140_v10", "HLT_PFJet200_v10", "HLT_PFJet260_v10", "HLT_PFJet320_v10", "HLT_PFJet400_v10", "HLT_PFJet450_v10", "HLT_PFJet500_v10",
  'HLT_PFJet40_v9','HLT_PFJet60_v9','HLT_PFJet80_v9','HLT_PFJet140_v9','HLT_PFJet200_v9','HLT_PFJet260_v9','HLT_PFJet320_v9','HLT_PFJet400_v9','HLT_PFJet450_v9','HLT_PFJet500_v9',
  'HLT_PFJet40_v8','HLT_PFJet60_v8','HLT_PFJet80_v8','HLT_PFJet140_v8','HLT_PFJet200_v8','HLT_PFJet260_v8','HLT_PFJet320_v8','HLT_PFJet400_v8','HLT_PFJet450_v8','HLT_PFJet500_v8', 
  'HLT_PFJet40_v7','HLT_PFJet60_v7','HLT_PFJet80_v7','HLT_PFJet140_v7','HLT_PFJet200_v7','HLT_PFJet260_v7','HLT_PFJet320_v7','HLT_PFJet400_v7','HLT_PFJet450_v7','HLT_PFJet500_v7',
  'HLT_PFJet40_v6','HLT_PFJet60_v6','HLT_PFJet80_v6','HLT_PFJet140_v6','HLT_PFJet200_v6','HLT_PFJet260_v6','HLT_PFJet320_v6','HLT_PFJet400_v6','HLT_PFJet450_v6','HLT_PFJet500_v6',
  'HLT_PFJet40_v5','HLT_PFJet60_v5','HLT_PFJet80_v5','HLT_PFJet140_v5','HLT_PFJet200_v5','HLT_PFJet260_v5','HLT_PFJet320_v5','HLT_PFJet400_v5','HLT_PFJet450_v5','HLT_PFJet500_v5',
  'HLT_PFJet40_v4','HLT_PFJet60_v4', 'HLT_PFJet80_v4', 'HLT_PFJet140_v4','HLT_PFJet200_v4','HLT_PFJet260_v4','HLT_PFJet320_v4','HLT_PFJet400_v4','HLT_PFJet450_v4','HLT_PFJet500_v4'
)


process.ak4 =  cms.EDAnalyzer('ProcessedTreeProducerBTag',
  ## jet collections ###########################
  pfjets          = cms.InputTag('slimmedJets'),
  pfjetschs       = cms.InputTag('slimmedJets'),
  pfpujetid       = cms.string('AK4PFpileupJetIdEvaluator:fullDiscriminant'),
  pfchsjetpuid    = cms.string('AK4PFCHSpileupJetIdEvaluator:fullDiscriminant'),
  ## MET collection ####
  pfmet           = cms.InputTag('slimmedMETs'),
  genjets         = cms.untracked.InputTag('slimmedGenJets'),
  ## database entry for the uncertainties ######
  PFPayloadName   = cms.string(''),
  PFPayloadNameCHS= cms.string(''),
  jecUncSrc       = cms.string(''),
  jecUncSrcCHS    = cms.string(''),
  jecUncSrcNames  = cms.vstring(''),
  ## set the conditions for good Vtx counting ##
  offlineVertices = cms.InputTag('offlineSlimmedPrimaryVertices'),
  beamSpot        = cms.InputTag('offlineBeamSpot'),
  goodVtxNdof     = cms.double(4),
  goodVtxZ        = cms.double(24),
  ## rho #######################################
  srcCaloRho      = cms.InputTag('fixedGridRhoFastjetAllCalo'),
  srcPFRho        = cms.InputTag('fixedGridRhoFastjetAll'),
  srcPULabel      = cms.untracked.InputTag('slimmedAddPileupInfo'),
  ## preselection cuts #########################
  maxEta          = cms.double(5.0),
  minPFPt         = cms.double(30),
  minNPFJets      = cms.uint32(1),
  minGenPt        = cms.untracked.double(20),
  isMCarlo        = cms.untracked.bool(False),
  useGenInfo      = cms.untracked.bool(False),
  AK4             = cms.untracked.bool(True),      
  ## trigger ###################################
  printTriggerMenu = cms.untracked.bool(True),
  processName     = cms.string('HLT'),
  triggerName     = triggers,
  triggerResults  = cms.InputTag("TriggerResults","","HLT"),
  triggerEvent    = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
  prescales       = cms.InputTag("patTrigger"),
  prescalesL1Min  = cms.InputTag("patTrigger","l1min"), 
  prescalesL1Max  = cms.InputTag("patTrigger","l1max"), 
  triggerObjects  = cms.InputTag("slimmedPatTrigger"),
  ## jec services ##############################
  EventInfo       = cms.InputTag("generator"),
  GenParticles    = cms.InputTag("genparticles"),
  jetFlavourInfos = cms.InputTag("genJetFlavourInfos"),
  saveWeights     = cms.bool(False),                      
)

process.goodVertices = cms.EDFilter("VertexSelector",
  filter = cms.bool(False),
  src = cms.InputTag("offlineSlimmedPrimaryVertices"),
  cut = cms.string("!isFake && ndof > 4 && abs(z) <= 24 && position.Rho <= 2"),
)

#Try scheduled processs
process.path = cms.Path(process.goodVertices*
                        process.ak4)

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Output and Log
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
