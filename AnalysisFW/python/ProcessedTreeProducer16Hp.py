# -*- coding: utf-8 -*-

import FWCore.ParameterSet.Config as cms
from os import *

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
from PhysicsTools.PatAlgos.patTemplate_cfg import *
from PhysicsTools.PatAlgos.tools.jetTools import *
from RecoJets.JetProducers.QGTagger_cfi import QGTagger
from JetMETCorrections.Type1MET.correctionTermsPfMetType0PFCandidate_cff import *
from JetMETCorrections.Type1MET.correctedMet_cff import pfMetT0pcTxy

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

from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag.globaltag = "80X_mcRun2_asymptotic_2016_TrancheIV_v6"

##-------------------- Import the JEC services -----------------------
process.load('JetMETCorrections.Configuration.DefaultJEC_cff')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Services
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#process.MessageLogger = cms.Service("MessageLogger",
#        destinations = cms.untracked.vstring(                           #1
#                'myOutputFile'                                          #2
#        ),
#        myOutputFile = cms.untracked.PSet(                              #3
#                threshold = cms.untracked.string( 'WARNING' )           #4
#        ),
#)                                                                       #5
process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.load('CommonTools.UtilAlgos.TFileService_cfi')
process.TFileService.fileName=cms.string('DATA.root')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Input
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

inFiles = cms.untracked.vstring( 'root://cms-xrd-global.cern.ch//store/mc/RunIISummer16DR80Premix/QCD_Pt-15to7000_TuneCUETHS1_Flat_13TeV_herwigpp/AODSIM/PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/100000/02A0DA71-14D3-E611-8FEE-A0000420FE80.root' )

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(10))
process.source = cms.Source("PoolSource", fileNames = inFiles )

## Modified version of jetToolBox from https://github.com/cms-jet/jetToolbox
## Options for PUMethod: Puppi, CS, SK, CHS
def jetToolbox( proc, jetType, jetSequence, PUMethod='', bTagDiscriminators = None):
  JETCorrPayload='None'
  JETCorrLevels = [ 'None' ]
  # See PhysicsTools/PatAlgos/python/recoLayer0/bTagging_cff.py for further options
  bTagDiscriminators = ['pfDeepCSVJetTags:probb',
                        'pfDeepCSVJetTags:probc',
                        'pfDeepCSVJetTags:probudsg',
                        'pfDeepCSVJetTags:probbb',
                        #'pfDeepFlavourJetTags:probb',
                        #'pfDeepFlavourJetTags:probc',
                        #'pfDeepFlavourJetTags:probg',
                        #'pfDeepFlavourJetTags:probuds',
                        #'pfDeepFlavourJetTags:probbb',
                        #'pfTrackCountingHighEffBJetTags',
                        'pfJetProbabilityBJetTags',
                        #'pfSimpleSecondaryVertexHighEffBJetTags',
                        'pfCombinedCvsLJetTags',
                        'pfCombinedCvsBJetTags',
                        'pfBoostedDoubleSecondaryVertexAK8BJetTags',
                        #'pfCombinedSecondaryVertexV2BJetTags',
                        'pfCombinedInclusiveSecondaryVertexV2BJetTags',
                        'pfCombinedMVAV2BJetTags',
                        'pfJetProbabilityBJetTags']

  GetJetMCFlavour = ['True']

  algorithm='AntiKt' # CambridgeAachen' , 'Kt'
  size = jetType[-1:] #[-1:] takes the last char from string 'akX'
  jetSize = float('0.'+jetType[-1:])
  jetALGO = jetType.upper()
  jetalgo = jetType.lower()
  elemToKeep = []

  print 'Running processes with: '+str(jetALGO)+' PF '+PUMethod+' jet algorithm with radius parameter '+str(jetSize)

  JETCorrPayload = 'AK'+size+'PF'+PUMethod.lower()
  JETCorrLevels = ['L1FastJet', 'L2Relative', 'L3Absolute']
  JEC = ( JETCorrPayload, JETCorrLevels , 'None')
  
  #################################################################################
  ####### Toolbox start
  #################################################################################
  
  elemToKeep = []
  jetSeq = cms.Sequence()
  genParticlesLabel = ''
  pvLabel = ''
  tvLabel = ''
  toolsUsed = []
  
  proc.load('RecoJets.Configuration.GenJetParticles_cff')
  setattr( proc, jetalgo+'GenJetsNoNu', ak4GenJets.clone( src = 'genParticlesForJetsNoNu', rParam = jetSize, jetAlgorithm = algorithm ) )
  jetSeq += getattr(proc, jetalgo+'GenJetsNoNu' )
  
  proc.load('CommonTools.ParticleFlow.pfNoPileUpJME_cff')
  if( PUMethod=='CHS') :
    setattr( proc, jetalgo+'PFJetsCHS', ak4PFJets.clone( rParam = jetSize, jetAlgorithm = algorithm ) )
    jetSeq += getattr(proc, jetalgo+'PFJetsCHS' )
  
    setattr( proc, jetalgo+'PFJetsCHS',
             ak4PFJetsCHS.clone(
             doAreaFastjet = True,
             rParam = jetSize,
             jetAlgorithm = algorithm ) )
    jetSeq += getattr(proc, jetalgo+'PFJetsCHS' )
  else :
    setattr( proc, jetalgo+'PFJets', ak4PFJets.clone( rParam = jetSize, jetAlgorithm = algorithm ) )
    jetSeq += getattr(proc, jetalgo+'PFJets' )
  
    setattr( proc, jetalgo+'PFJets',
             ak4PFJets.clone(
             doAreaFastjet = True,
             rParam = jetSize,
             jetAlgorithm = algorithm ) )
    jetSeq += getattr(proc, jetalgo+'PFJets' )
    PUMethod=''
  
  ####  Creating PATjets
  addJetCollection( proc,
                    labelName = jetALGO+'PF'+PUMethod,
                    jetSource = cms.InputTag( jetalgo+'PFJets'+PUMethod),
                    algo = jetalgo,
                    btagDiscriminators = bTagDiscriminators,
                    rParam = jetSize,
                    jetCorrections =  JEC,
                    pfCandidates = cms.InputTag( 'particleFlow' ),
                    svSource = cms.InputTag('inclusiveCandidateSecondaryVertices'),
                    genJetCollection = cms.InputTag( jetalgo+'GenJetsNoNu'),
                    pvSource = cms.InputTag( 'offlinePrimaryVertices' ), #'offlineSlimmedPrimaryVertices'),
                    jetTrackAssociation = True)
  
  # srcJets Could be reco::PFJetCollection or pat::JetCollection (both AOD and miniAOD) 
  # Other options for jetsLabel (might need to add an ESSource for it): see https://twiki.cern.ch/twiki/bin/viewauth/CMS/QGDataBaseVersion
  setattr( proc, 'QGTagger'+jetALGO+'PF'+PUMethod,
           QGTagger.clone( srcJets = cms.InputTag(jetalgo+'PFJets'+PUMethod),
                           jetsLabel = cms.string('QGL_AK4PFchs') ) )
  elemToKeep += [ 'keep *_QGTagger'+jetALGO+'PF'+PUMethod+'_*_*' ]
  getattr( proc, 'patJets'+jetALGO+'PF'+PUMethod).userData.userFloats.src += ['QGTagger'+jetALGO+'PF'+PUMethod+':qgLikelihood']
  jetSeq += getattr(proc, 'QGTagger'+jetALGO+'PF'+PUMethod )
  toolsUsed.append( 'QGTagger'+jetALGO+'PF'+PUMethod )

  getattr(proc,'patJetPartons').particles = cms.InputTag( 'genParticles' ) #'prunedGenParticles') 
  setattr(proc, 'selectedPatJets'+jetALGO+'PF'+PUMethod, selectedPatJets.clone( src = 'patJets'+jetALGO+'PF'+PUMethod ) )
  setattr(proc, jetSequence, jetSeq)

jetToolbox( process, 'ak4', 'ak4JetSubs','CHS')

# PAT Layer 1
process.load("PhysicsTools.PatAlgos.producersLayer1.patCandidates_cff")
process.load("PhysicsTools.PatAlgos.selectionLayer1.selectedPatCandidates_cff")
process.patJets.addTagInfos = True
process.patJets.addAssociatedTracks = True
process.patJets.getJetMCFlavour = True
process.patJets.addGenPartonMatch = True

process.load("PhysicsTools.PatAlgos.patSequences_cff")

genJetCollection = 'ak4GenJetsNoNu'
genParticleCollection = 'genParticles'

from PhysicsTools.JetMCAlgos.HadronAndPartonSelector_cfi import selectedHadronsAndPartons
process.selectedHadronsAndPartons = selectedHadronsAndPartons.clone( particles = genParticleCollection,
                                                                     partons = cms.InputTag("selectedHadronsAndPartons","algorithmicPartons"),
                                                                     partonMode = cms.string("Herwig++") )

from PhysicsTools.JetMCAlgos.AK4PFJetsMCFlavourInfos_cfi import ak4JetFlavourInfos
process.genJetFlavourInfos = ak4JetFlavourInfos.clone( jets = genJetCollection )

#You need to create another collection to give the physics definition 
process.genJetFlavourInfosPhysicsDef = ak4JetFlavourInfos.clone(
    jets = genJetCollection,
#    jets = cms.InputTag('selectedPatJetsAK4PFCHS'),
    partons = cms.InputTag("selectedHadronsAndPartons","physicsPartons"),
)

process.ak4 =  cms.EDAnalyzer( 'ProcessedTreeProducerBTag',
                               ## jet collections ###########################
                               AK4                       = cms.untracked.bool(True),
                               pfjetschs                 = cms.InputTag('selectedPatJetsAK4PFCHS'),
                               pfpujetid                 = cms.string('AK4PFpileupJetIdEvaluator:fullDiscriminant'),
                               pfchsjetpuid              = cms.string('AK4PFCHSpileupJetIdEvaluator:fullDiscriminant'),
                               ## MET collection ####
                               pfmetT1                   = cms.InputTag('patMETs'),
                               pfmetT0pc                 = cms.InputTag('patMETsT0pc'),
                               pfmetT0pcT1               = cms.InputTag('patMETsT0pcT1'),
                               ## database entry for the uncertainties ######
                               PFPayloadNameCHS          = cms.string('AK4PFchs'),
                               jecUncSrcNames            = cms.vstring(''),
                               ## set the conditions for good Vtx counting ##
                               offlineVertices           = cms.InputTag('offlinePrimaryVertices'),
                               beamSpot                  = cms.InputTag('offlineBeamSpot'),
                               goodVtxNdof               = cms.double(4),
                               goodVtxZ                  = cms.double(24),
                               ## rho #######################################
                               srcCaloRho                = cms.InputTag('fixedGridRhoFastjetAllCalo'),
                               srcPFRho                  = cms.InputTag('fixedGridRhoFastjetAll'),
                               srcPULabel                = cms.untracked.InputTag('addPileupInfo'),
                               ## preselection cuts #########################
                               maxEta                    = cms.double(5.0),
                               minPFPt                   = cms.double(10.0),
                               minPFPtThirdJet           = cms.double(5.0),
                               minNPFJets                = cms.uint32(1),
                               minGenPt                  = cms.untracked.double(20),
                               runYear                   = cms.untracked.string("2016"),
                               ## trigger ###################################
                               triggerName               = cms.vstring(''), 
                               ## gen services ##############################
                               isMCarlo                  = cms.untracked.bool(True),
                               useGenInfo                = cms.untracked.bool(True),
                               mcType                    = cms.untracked.int32(1),
                               EventInfo       = cms.untracked.InputTag("generator"),
                               genjets         = cms.untracked.InputTag(genJetCollection),
                               GenParticles    = cms.untracked.InputTag(genParticleCollection),
                               jetFlavourInfos = cms.untracked.InputTag("genJetFlavourInfos"),
                               jetFlavourInfosPhysicsDef = cms.untracked.InputTag("genJetFlavourInfosPhysicsDef") )

#jetToolbox( process, 'ak7', 'ak7JetSubs','CHS')
#
#process.ak7 = process.ak4.clone(
#	pfjetschs       = cms.InputTag('selectedPatJetsAK7PFCHS'),
#	pfpujetid       = cms.string('AK7PFpileupJetIdEvaluator:fullDiscriminant'),
#	pfchsjetpuid    = cms.string('AK7PFCHSpileupJetIdEvaluator:fullDiscriminant'),
#    PFPayloadNameCHS= cms.string('AK7PFchs'),
#	AK4             = cms.untracked.bool(False),
#)

#MET Filters
process.load('RecoMET.METFilters.primaryVertexFilter_cfi')
process.load('RecoMET.METFilters.globalTightHalo2016Filter_cfi')
process.load('CommonTools.RecoAlgos.HBHENoiseFilterResultProducer_cfi')
process.load('CommonTools.RecoAlgos.HBHENoiseFilter_cfi')
process.load('RecoMET.METFilters.EcalDeadCellTriggerPrimitiveFilter_cfi')
process.load('RecoMET.METFilters.BadChargedCandidateFilter_cfi')
process.load('RecoMET.METFilters.BadPFMuonFilter_cfi')
#process.load('RecoMET.METFilters.eeBadScFilter_cfi')

process.allMetFilterPaths=cms.Sequence( process.primaryVertexFilter*
                                        process.globalTightHalo2016Filter*
                                        process.HBHENoiseFilter*
                                        process.HBHENoiseIsoFilter*
                                        process.EcalDeadCellTriggerPrimitiveFilter*
                                        process.BadPFMuonFilter*
                                        process.BadChargedCandidateFilter )
                                        #process.eeBadScFilter* # Not in MC

##Type1 patMET Producer
process.load("JetMETCorrections.Type1MET.correctionTermsPfMetType0PFCandidate_cff")
process.load('PhysicsTools.PatAlgos.producersLayer1.metProducer_cfi')
process.patMETs.addGenMET = cms.bool(False)
process.patMETs.metSource = cms.InputTag("pfMetT1") # This is the default, but let's make it explicit
process.patMETsT0pc = process.patMETs.clone(metSource = cms.InputTag("pfMetT0pc"))
process.patMETsT0pcT1 = process.patMETs.clone(metSource = cms.InputTag("pfMetT0pcT1"))

#Try scheduled processs
process.path = cms.Path( process.allMetFilterPaths*
                         process.correctionTermsPfMetType0PFCandidate*
                         process.patMETs*
                         process.patMETsT0pc*
                         process.patMETsT0pcT1*
                         process.ak4 )


#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#! Output and Log
#!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(False) )
process.options.allowUnscheduled = cms.untracked.bool(True)
