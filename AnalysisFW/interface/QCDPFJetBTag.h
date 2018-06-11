//Author K. Kousouris
//Modified by: S. Ganguly

#ifndef QCDPFJetBTag_h
#define QCDPFJetBTag_h
#include "SMPJ/AnalysisFW/interface/QCDJet.h"
#include "TLorentzVector.h"
class QCDPFJetBTag : public QCDJet {
  public:
    //------------ Constructor ------------------------------
    QCDPFJetBTag() {}
    //------------ Destructor -------------------------------
    ~QCDPFJetBTag() {}
    //------------ Set methods ------------------------------

    void setFlavour(int fpartonflavour, int fhadronflavour, int fpartonflavourPhysicsDef) { partonFlavour_ = fpartonflavour; hadronFlavour_ = fhadronflavour; partonFlavourPhysicsDef_ = fpartonflavourPhysicsDef;}

    void setQGTagger (float fQGL, float fQGAx2, int fQGMul, float fQGPtD) { QGL_ = fQGL; QGAx2_ = fQGAx2; QGMul_ = fQGMul; QGPtD_ = fQGPtD; }

    int partonflavour()      const {return partonFlavour_;}
    int partonflavourPhysicsDef()      const {return partonFlavourPhysicsDef_;}
    int hadronflavour()      const {return hadronFlavour_;}

    float qglikelihood()      const { return QGL_; }
    float qgaxis2()           const { return QGAx2_; }
    int   qgmult()            const { return QGMul_; }
    float qgptd()             const { return QGPtD_; }

    float pfCombinedCvsL() const {return pfCombinedCvsL_;}
    float pfCombinedCvsB() const {return pfCombinedCvsB_;}

    float pfCombinedCvsL_, pfCombinedCvsB_;
    float pfDeepCSVb_, pfDeepCSVc_, pfDeepCSVl_, pfDeepCSVbb_;
    float pfDeepFlavourb_, pfDeepFlavourc_, pfDeepFlavourg_, pfDeepFlavourl_, pfDeepFlavourbb_;
    float pfBTag_JetProb_;
    float pfBTag_CombInclSecVtxV2_;
    float pfBTag_CombMVAV2_; 

    float QGL_;
    float QGAx2_;
    int   QGMul_;
    float QGPtD_;

    int partonFlavour_;
    int partonFlavourPhysicsDef_;
    int hadronFlavour_;
};
#endif
