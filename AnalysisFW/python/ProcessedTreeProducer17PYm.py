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
#from PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi import patMETs
from PhysicsTools.PatAlgos.patSequences_cff import *
#nnfrom PhysicsTools.PatAlgos.tools.metTools import addMETCollection
#from RecoJets.JetProducers.pileupjetidproducer_cfi import *
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from RecoJets.JetProducers.QGTagger_cfi import QGTagger

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
process.QGTagger.srcJets = cms.InputTag('ak4PFJetsCHS')
process.QGTagger.jetsLabel  = cms.string('QGL_AK4PFchs')

process.GlobalTag.globaltag = "94X_mc2017_realistic_v12"

##-------------------- Import the JEC services -----------------------
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Input
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

inFiles = cms.untracked.vstring(
'/store/mc/RunIISummer16MiniAODv2/QCD_Pt-15to7000_TuneCUETP8M1_Flat_13TeV_pythia8/MINIAODSIM/PUMoriond17_magnetOn_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/100000/08278E4E-E4EF-E611-8BD7-FA163E3ABA64.root'
   )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10000))
process.source = cms.Source("PoolSource", fileNames = inFiles )

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Services
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.MessageLogger = cms.Service("MessageLogger",
        destinations = cms.untracked.vstring(                           #1
                'myOutputFile'                                          #2
        ),
        myOutputFile = cms.untracked.PSet(                              #3
                threshold = cms.untracked.string( 'WARNING' )           #4
        ),
)                                                                       #5

process.load('CommonTools.UtilAlgos.TFileService_cfi')
process.TFileService.fileName=cms.string('DATA.root')

process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")

process.patJets.addTagInfos = True
process.patJets.addAssociatedTracks = True

from RecoJets.Configuration.RecoGenJets_cff import ak4GenJets, ak8GenJets

genParticleCollection = 'prunedGenParticles'

from RecoJets.JetProducers.ak4GenJets_cfi import ak4GenJets
from RecoJets.JetProducers.GenJetParameters_cfi import *

process.ak4GenJetsCustom = ak4GenJets.clone(
    src = genParticleCollection,
    rParam = cms.double(0.4),
    jetAlgorithm = cms.string("AntiKt")
)

genParticleCollection = 'prunedGenParticles'
genJetCollection = 'slimmedGenJets'

from PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi import selectedHadronsAndPartons
process.selectedHadronsAndPartons = selectedHadronsAndPartons.clone(
    particles = genParticleCollection
)

from PhysicsTools.JetMCAlgos.AK4PFJetsMCFlavourInfos_cfi import ak4JetFlavourInfos
process.genJetFlavourInfos = ak4JetFlavourInfos.clone(
    jets = genJetCollection,
    rParam = cms.double(0.4),
)

process.load("PhysicsTools.PatAlgos.patSequences_cff")

process.ak4 =  cms.EDAnalyzer('ProcessedTreeProducerBTag',
  ## jet collections ###########################
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
  isMCarlo        = cms.untracked.bool(True),
  useGenInfo      = cms.untracked.bool(True),
  AK4             = cms.untracked.bool(True),      
  ## trigger ###################################
  printTriggerMenu = cms.untracked.bool(False),
  processName     = cms.string('HLT'),
  triggerName     = cms.vstring(''),
  triggerResults  = cms.InputTag("TriggerResults","","HLT"),
  triggerEvent    = cms.InputTag("hltTriggerSummaryAOD","","HLT"),
  prescalesL1Min  = cms.InputTag("patTrigger","l1min"),
  prescalesL1Max  = cms.InputTag("patTrigger","l1max"),
  prescales       = cms.InputTag("patTrigger"),
  triggerObjects  = cms.InputTag("slimmedPatTrigger"),
  ## jec services ##############################
  EventInfo       = cms.InputTag("generator"),
  GenParticles    = cms.InputTag("prunedGenParticles"),
  jetFlavourInfos = cms.InputTag("genJetFlavourInfos"),
  saveWeights     = cms.bool(False)                      
)

process.goodVertices = cms.EDFilter("VertexSelector",
   filter = cms.bool(False),
   src = cms.InputTag("offlineSlimmedPrimaryVertices"),
   cut = cms.string("!isFake && ndof >= 4 && abs(z) <= 24 && position.rho <= 2"),
)

#Try scheduled processs
process.path = cms.Path(process.goodVertices*
                        process.selectedHadronsAndPartons*
                        process.genJetFlavourInfos*
                        #process.patMETCorrections*process.patMETs
                        process.ak4)


#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Output and Log
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
