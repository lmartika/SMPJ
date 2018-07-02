import FWCore.ParameterSet.Config as cms

flt16DT = cms.vstring(
 "Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter",
 "Flag_goodVertices","Flag_eeBadScFilter","Flag_globalTightHalo2016Filter",
 "Flag_BadChargedCandidateFilter","Flag_BadPFMuonFilter"
)
flt16MC = cms.vstring(
 "Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter",
 "Flag_goodVertices","Flag_globalTightHalo2016Filter",
 "Flag_BadChargedCandidateFilter","Flag_BadPFMuonFilter"
)

flt17DT = cms.vstring(
 "Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter",
 "Flag_goodVertices","Flag_eeBadScFilter","Flag_ecalBadCalibFilter",
 "Flag_globalTightHalo2016Filter","Flag_BadChargedCandidateFilter","Flag_BadPFMuonFilter"
)
flt17MC = cms.vstring(
 "Flag_HBHENoiseFilter","Flag_HBHENoiseIsoFilter","Flag_EcalDeadCellTriggerPrimitiveFilter",
 "Flag_goodVertices","Flag_ecalBadCalibFilter",
 "Flag_globalTightHalo2016Filter","Flag_BadChargedCandidateFilter","Flag_BadPFMuonFilter"
)

